//
// Created by dxve on 4.6.23.
//

#include "CAction.h"

#include <utility>

std::map<std::string, std::shared_ptr<CAction>> CAction::loaded_actions;

CAction::CAction(const nlohmann::json &saved_data, CGame *game) : game(game) {

    if( !CJsonCheck::checkIfContains(saved_data, {"name", "costs"})) {
        throw std::runtime_error("Failed loading upgrade multiplier!");
    }

    std::string saved_name = saved_data["name"];
    std::map<std::string, int> saved_costs = saved_data["costs"];

    name = saved_name;
    execute_cost = saved_costs;
}

bool CAction::execute() const {
    CPlayerStats * executor = game->accessPlayerOnMove();
    if(!executor->canAfford(execute_cost)) {
        std::cout << "--> Cannot afford! <--" << std::endl;
        return false;
    }

    executor->removeResources(execute_cost);
    return true;
}

std::string CAction::printInfo() const {
    std::stringstream ss;
    ss << "[" << name << "]:\n  Costs:";
    for(const auto & [resource, amount]: execute_cost) {
        ss << "\n   " << resource << " " << amount;
    }
    return ss.str();
}

CAction::CAction(CGame *game, std::string name) :game(game), name(std::move(name)) {

}

const std::map<std::string, int> &CAction::getExecuteCost() {
    return execute_cost;
}

int CAction::calculateValue() {
    return 0;
}

std::pair<int, int> CAction::calculateBestPosition() {
    return {-1,-1};
}
