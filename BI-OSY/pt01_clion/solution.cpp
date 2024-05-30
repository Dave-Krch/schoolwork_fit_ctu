#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <iterator>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <compare>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <condition_variable>
#include <pthread.h>
#include <semaphore.h>
#include "progtest_solver.h"
#include "sample_tester.h"

using namespace std;
#endif /* __PROGTEST__ */

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

struct ProcessedPack {
    AProblemPack pack;
    size_t problems = 0;
    size_t solved_problems = 0;

    mutex & proccesed_packs_queue_mtx;
    condition_variable & cv_problems_solved;

    bool is_last_pack = false;

    ProcessedPack(const AProblemPack& pack,
                  mutex & company_mutex,
                  condition_variable & company_cv,
                  bool is_last_pack) :
                      pack(pack),
                      proccesed_packs_queue_mtx(company_mutex),
                      cv_problems_solved(company_cv),
                      is_last_pack(is_last_pack) {
        problems = pack->m_ProblemsCnt.size() + pack->m_ProblemsMin.size();
        solved_problems = 0;
    }

    ProcessedPack(bool is_last,
                  mutex & company_mutex,
                  condition_variable & company_cv) :
                    proccesed_packs_queue_mtx(company_mutex),
                    cv_problems_solved(company_cv),
                    is_last_pack(is_last) {
        pack = nullptr;
    }
};

struct SolverWrapper {
    AProgtestSolver solver;

    //problem pack, a pocet problemu co se z nej resi
    vector<pair<shared_ptr<ProcessedPack>, int>> associated_problem_packs;

    bool is_ending_message;

    SolverWrapper(AProgtestSolver solver_ptr, bool is_ending_message) : solver(solver_ptr), is_ending_message(is_ending_message) {

    }
};

//----- spravuje komunikaci s workerama
class WorkerClass {
public:

    queue<shared_ptr<SolverWrapper>> solvers_queue;

    //solvery k plneni
    shared_ptr<SolverWrapper> min_pt_solver;
    shared_ptr<SolverWrapper> cnt_pt_solver;

    size_t producers = 0;
    size_t workers = 0;

    //mutex na frontu ready solveru
    mutex solvers_queue_mtx;
    condition_variable cv_solver_queue;

    //na lockovani kdyz se do nich zapisuje/posilaj se/tvorej se novy
    mutex solver_mtx;

    //zamyka pocet aktivnich producers
    mutex active_producers_mtx;

    //po vyreseni solveru dostanou notify company consumers
    mutex solver_solved_mtx;
    condition_variable solver_solved_cv;

    WorkerClass() {
        min_pt_solver = make_shared<SolverWrapper>(createProgtestMinSolver(), false);
        cnt_pt_solver = make_shared<SolverWrapper>(createProgtestCntSolver(), false);
    }

    void sendMinSolver() {
        //zamknout solvery a frontu
        unique_lock<mutex> queue_ul(solvers_queue_mtx);

        //solver uz je locekd pred volanim!!!
        //unique_lock<mutex> solvers_ul(solver_mtx);

        //posle solver do fronty
        solvers_queue.push(min_pt_solver);
        //novy solver
        min_pt_solver = make_shared<SolverWrapper>(createProgtestMinSolver(), false);

        //notify workerum
        cv_solver_queue.notify_one();
    }

    void sendCntSolver() {
        //zamknout solvery a frontu
        unique_lock<mutex> queue_ul(solvers_queue_mtx);

        //solver uz je locekd pred volanim!!!
        //unique_lock<mutex> solvers_ul(solver_mtx);

        solvers_queue.push(cnt_pt_solver);
        cnt_pt_solver = make_shared<SolverWrapper>(createProgtestCntSolver(), false);

        cv_solver_queue.notify_one();
    }

    void set_active_producers(size_t x) {
        unique_lock<mutex> ul(active_producers_mtx);
        producers = x;
    }

    bool remove_active_producer() {
        unique_lock<mutex> ul(active_producers_mtx);
        producers -= 1;
        if(producers > 0)
            return false;
        return true;
    }

    void send_end_messages() {
        unique_lock<mutex> queue_ul(solvers_queue_mtx);
        unique_lock<mutex> solvers_ul(solver_mtx);

        solvers_queue.push(min_pt_solver);
        solvers_queue.push(cnt_pt_solver);

        for(size_t i = 0; i < workers; i++) {
            solvers_queue.push(make_shared<SolverWrapper>(createProgtestMinSolver(), true));
        }

        cv_solver_queue.notify_all();
    }

    void worker_thread(int id) {
//        printf("Worker thread %d ready\n", id);
//        fflush(stdout);

        while (true) {
            unique_lock<mutex> solver_q_ul(solvers_queue_mtx);
            cv_solver_queue.wait(solver_q_ul, [this](){return !solvers_queue.empty(); } );

            shared_ptr<SolverWrapper> current_solver = solvers_queue.front();
            solvers_queue.pop();

            solver_q_ul.unlock();

            if(current_solver->is_ending_message)
                break;

            current_solver->solver->solve();

            for(const auto & [pack, count]: current_solver->associated_problem_packs) {
                unique_lock<mutex> company_ul(pack->proccesed_packs_queue_mtx);
                pack->solved_problems += count;

                pack->cv_problems_solved.notify_one();
            }

        }

//        printf("Worker thread %d ending\n", id);
//        fflush(stdout);
    }


};

//----- spravuje komunikacni vlakna companies
class CompanyWrapper {
public:
    ACompany company;

    int company_id;

    WorkerClass & worker_class_obj_ref;

    //Fronta packu vybranych producerem - solver ma pointery do te fronty s poctem problemu co k packu resi
    // - po solvenuti da notify consumerovi, a ten odebora z fronty pokud jsou na zacatku solved
    queue<shared_ptr<ProcessedPack>> processed_packs;

    //vlastni mutex pro kazdou company
    mutex processed_packs_queue_mtx;

    condition_variable cv_problems_solved;

    //
    CompanyWrapper(ACompany company_in, int id, WorkerClass &wclobjref) :   company(company_in),
                                                                            company_id(id),
                                                                            worker_class_obj_ref(wclobjref) {
//        printf("Creating company %d\n", company_id);
//        fflush(stdout);
    }

    //Vybere pack a plni s nim solvery
    void producer_thread() {
        while (true) {
            AProblemPack current_pack = company->waitForPack();

            if(current_pack != nullptr) {

                //hodi se do fronty
                shared_ptr<ProcessedPack> current_processed_pack = make_shared<ProcessedPack>(current_pack, processed_packs_queue_mtx, cv_problems_solved, false);
                unique_lock<mutex> ul_proccesed_pack_queue(processed_packs_queue_mtx);
                processed_packs.push(current_processed_pack);
                ul_proccesed_pack_queue.unlock();

                //plneni solver wrapperu
                unique_lock<mutex> ul_solvers(worker_class_obj_ref.solver_mtx);

                //plneni min solveru
                worker_class_obj_ref.min_pt_solver->associated_problem_packs.emplace_back(current_processed_pack, 0);

                for(const auto & polygon: current_pack->m_ProblemsMin) {
                    worker_class_obj_ref.min_pt_solver->solver->addPolygon(polygon);
                    worker_class_obj_ref.min_pt_solver->associated_problem_packs.back().second ++;

                    if(!worker_class_obj_ref.min_pt_solver->solver->hasFreeCapacity()) {
                        worker_class_obj_ref.sendMinSolver();
                        //znovu se musi nahodit soucasny problem pack
                        worker_class_obj_ref.min_pt_solver->associated_problem_packs.emplace_back(current_processed_pack, 0);
                    }
                }

                //plneni cnt soleru
                worker_class_obj_ref.cnt_pt_solver->associated_problem_packs.emplace_back(current_processed_pack, 0);

                for(const auto & polygon: current_pack->m_ProblemsCnt) {
                    worker_class_obj_ref.cnt_pt_solver->solver->addPolygon(polygon);
                    worker_class_obj_ref.cnt_pt_solver->associated_problem_packs.back().second ++;

                    if(!worker_class_obj_ref.cnt_pt_solver->solver->hasFreeCapacity()) {

                        worker_class_obj_ref.sendCntSolver();
                        //znovu se musi nahodit soucasny problem pack
                        worker_class_obj_ref.cnt_pt_solver->associated_problem_packs.emplace_back(current_processed_pack, 0);

                    }
                }

            }
            else {
                if(worker_class_obj_ref.remove_active_producer()) {
                    worker_class_obj_ref.send_end_messages();
                }

//                printf("Company %d PRODUCER shutting down\n", company_id);
//                fflush(stdout);

                unique_lock<mutex> ul_proccesed_pack_queue(processed_packs_queue_mtx);
                shared_ptr<ProcessedPack> end_pack = make_shared<ProcessedPack>(true, processed_packs_queue_mtx, cv_problems_solved);
                processed_packs.push(end_pack);

                return;
            }
        }
    }

    //sklada vyreseny packy a posila back
    void consumer_thread() {
        while (true) {
            unique_lock<mutex> ul_proccesed_pack_queue(processed_packs_queue_mtx);

            cv_problems_solved.wait(ul_proccesed_pack_queue, [this](){return !processed_packs.empty() && processed_packs.front()->problems == processed_packs.front()->solved_problems; });

            shared_ptr<ProcessedPack> solved_pack = processed_packs.front();
            processed_packs.pop();
            ul_proccesed_pack_queue.unlock();


            if(solved_pack->is_last_pack)
                break;

            company->solvedPack(solved_pack->pack);

        }

    }


};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
class COptimizer {
public:

    static bool usingProgtestSolver(void) {
        return true;
    }

    static void checkAlgorithmMin(APolygon p) {
        // dummy implementation if usingProgtestSolver() returns true
    }

    static void checkAlgorithmCnt(APolygon p) {
        // dummy implementation if usingProgtestSolver() returns true
    }

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

    vector<shared_ptr<CompanyWrapper>> companies;

    WorkerClass worker_class_obj;

    vector<thread> workers;
    vector<thread> company_producers;
    vector<thread> company_consumers;


    COptimizer() : worker_class_obj() {

    }

    void start(int threadCount) {

        worker_class_obj.set_active_producers(companies.size());
        worker_class_obj.workers = threadCount;

        for (auto &company: companies)
            company_consumers.emplace_back(&CompanyWrapper::consumer_thread, &(*company) );

        for (int i = 0; i < threadCount; i++)
            workers.emplace_back(&WorkerClass::worker_thread, &worker_class_obj, i);

        for (auto &company: companies)
            company_producers.emplace_back(&CompanyWrapper::producer_thread, &(*company));

    }

    void stop(void) {
        for (auto &t: company_producers)
            t.join();
        for (auto &t: workers)
            t.join();
        for (auto &t: company_consumers)
            t.join();
    }

    void addCompany(ACompany company) {
        companies.emplace_back(make_shared<CompanyWrapper>(company,companies.size() , worker_class_obj));
    }

};


//-------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__

int main(void) {
    COptimizer optimizer;

    ACompanyTest company = std::make_shared<CCompanyTest>();

    optimizer.addCompany(company);

    optimizer.start(4);

    optimizer.stop();
    if (!company->allProcessed())
        throw std::logic_error("(some) problems were not correctly processsed");
    return 0;
}

#endif /* __PROGTEST__ */
