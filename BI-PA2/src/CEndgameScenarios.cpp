//
// Created by dxve on 5.6.23.
//

#include "CEndgameScenarios.h"

CEndgameScenarios::CEndgameScenarios(const nlohmann::json &saved_conditions,
                                     const std::map<std::string, CPlayerStats *> &players) :
                                     players(players){
    if( !CJsonCheck::checkIfContains(saved_conditions, {"resource_win", "resource_loss", "round_counter", "max_rounds"})) {
        throw std::runtime_error("Failed loading endgame scenarios!");
    }

    std::map<std::string, int> saved_resource_win = saved_conditions["resource_win"];
    std::map<std::string, int> saved_resource_loss = saved_conditions["resource_loss"];
    int saved_round_counter = saved_conditions["round_counter"];
    int saved_max_rounds = saved_conditions["max_rounds"];

    resource_win = saved_resource_win;
    resource_loss = saved_resource_loss;
    round_counter = saved_round_counter;
    max_rounds = saved_max_rounds;
}

bool CEndgameScenarios::checkForGameEnd() {

    if(checkResourceWin())
        return true;

    if(checkResourceLoss())
        return true;

    if(checkMaxRoundEnd())
        return true;

    return false;
}

bool CEndgameScenarios::checkMaxRoundEnd() {
    if(max_rounds == round_counter) {
        std::cout << "--< Max number of rounds reached! >--" << std::endl;
        calculateWinners();
        return true;
    }
    round_counter ++;
    return false;
}

bool CEndgameScenarios::checkResourceWin() {
    if(resource_win.empty())
        return false;

    std::vector<std::string> winners;

    for(const auto & [name, stats]: players) {

        if(stats->canAfford(resource_win))
            winners.push_back(name);
    }

    if(winners.empty())
        return false;

    std::cout << "--< Resource win reached! >--" << std::endl;
    printWinners(winners);
    return true;
}

bool CEndgameScenarios::checkResourceLoss() {
    if(resource_loss.empty())
        return false;

    std::vector<std::string> losers;

    for(const auto & [name, stats]: players) {
        if(! stats->canAfford(resource_loss))
            losers.push_back(name);
    }

    if(losers.empty())
        return false;

    std::cout << "--< Resource loss reached! >--" << std::endl;

    if(losers.size() == 1){
        std::cout << "--< Player " << losers[0] << " has lost the game! >--" << std::endl;
        return true;
    }

    std::cout << "--< Draw between players! >--" << std::endl;
    for(const auto & loser: losers) {
        std::cout << loser << std::endl;
    }
    return true;
}

void CEndgameScenarios::calculateWinners() {
    std::vector<std::string> winners;
    int wining_sum = 0;

    for(const auto & [name, stats]: players) {
        int player_sum = stats->sumResources();
        if(player_sum > wining_sum) {
            wining_sum = player_sum;
            winners.clear();
            winners.push_back(name);
        }
        else if(player_sum == wining_sum) {
            winners.push_back(name);
        }
    }

    printWinners(winners);
}

void CEndgameScenarios::printWinners(const std::vector<std::string> &winners) {
    if(winners.size() == 1) {
        std::cout << "--< Player: " << winners[0] << " won! >--" << std::endl;
        return;
    }

    std::cout << "--< Draw between players! >--" << std::endl;
    for(const auto & winner: winners) {
        std::cout << winner << std::endl;
    }
}

void CEndgameScenarios::saveAsJson(nlohmann::json &save_data) const {
    save_data["round_counter"] = round_counter;
    save_data["max_rounds"] = max_rounds;
    save_data["resource_win"] = resource_win;
    save_data["resource_loss"] = resource_loss;
}

CEndgameScenarios::CEndgameScenarios() : round_counter(-1), max_rounds(-1) {

}

void CEndgameScenarios::printEndScenariosHelp() const {
    std::cout << "End game scenarios:\n   Game ends after " << max_rounds << " rounds\n   Player wins if he reaches:";

    for(const auto & [resource, amount]: resource_win) {
        std::cout << "\n      " << amount << " " << resource;
    }

    std::cout << "\n   Player looses if he goes under:";
    for(const auto & [resource, amount]: resource_loss) {
        std::cout << "\n      " << amount << " " << resource;
    }

    std::cout << std::endl;
}

int CEndgameScenarios::getRoundCount() const {
    return round_counter;
}

int CEndgameScenarios::getMaxRounds() const {
    return max_rounds;
}

const std::map<std::string, int> &CEndgameScenarios::getResourceWin() const {
    return resource_win;
}

const std::map<std::string, int> &CEndgameScenarios::getResourceLoss() const {
    return resource_loss;
}
