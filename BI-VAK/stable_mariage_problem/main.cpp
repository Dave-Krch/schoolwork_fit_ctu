#include <iostream>
#include <queue>
#include <ranges>
#include <vector>

struct StableMarriageProblem {

    //number of men/women
    int N = 0;

    // preferences
    // 0 empty, 1 -> N men, N+1 -> 2*n women
    std::vector<std::vector<int>> preferences;

    //queue of free men
    std::queue<int> free_men_queue;

    //vector for results
    std::vector<int> pairs;

    StableMarriageProblem() {

        //empty index 0
        preferences.emplace_back();

        //loading twice because of weird given input
        std::cin >> N >> N;

        //initializing results
        pairs = std::vector<int>((2 * N) + 1, -1);

        //loading preferences
        for(int i = 1; i <= 2*N; i++) {
            int preference_count;
            std::cin >> preference_count;
            preferences.emplace_back(preference_count);

            for(int j = 0; j < preference_count; j++) {
                int preference;
                std::cin >> preference;
                preferences.back()[j] = preference;
            }
        }

        //filling queue
        for(int i = 1; i <= N; i++) {
            free_men_queue.push(i);
        }

    }

    void Solve() {
        while (!free_men_queue.empty()) {
            int free_man = free_men_queue.front();
            free_men_queue.pop();

            for(int prefered_woman: preferences[free_man]) {

                //if prefered woman is free, they get engaged
                if (pairs[prefered_woman] == -1) {
                    pairs[prefered_woman] = free_man;
                    pairs[free_man] = prefered_woman;
                    break;
                }

                //if prefered woman is not free, she can choose free_man, if he is higher in her preference list than her current man

                int current_man = pairs[prefered_woman];

                for(int prefered_man: preferences[prefered_woman]) {

                    //free man is higher in preference list
                    if(prefered_man == free_man) {

                        // :( rip bro
                        pairs[current_man] = -1;
                        free_men_queue.push(current_man);

                        pairs[free_man] = prefered_woman;
                        pairs[prefered_woman] = free_man;

                        break;
                    }

                    //current man is higher in preference list
                    if(prefered_man == current_man) {
                        break;
                    }
                }

                //break loop if paired
                if(pairs[free_man] != -1) {
                    break;
                }

            }
        }
    }

    void PrintPairs() {
        for(int i = 1; i <= (2 * N); i++) {
            //std::cout << i << " " << pairs[i] << std::endl;

            std::cout << pairs[i];

            if(i != 2 * N)
                std::cout << std::endl;
        }
    }
};

int main() {
    StableMarriageProblem p;

    p.Solve();

    p.PrintPairs();

}



int count = 0;

std::vector<int> pairs;

StableMarriageProblem() {

    pairs = std::vector<int>(N);

    std::cin >> N;
