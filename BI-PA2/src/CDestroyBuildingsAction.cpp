//
// Created by dxve on 5.6.23.
//

#include "CDestroyBuildingsAction.h"

CDestroyBuildingsAction::CDestroyBuildingsAction(const nlohmann::json &saved_data, CGame *game)
                                                : CAction(saved_data, game){
    if( !CJsonCheck::checkIfContains(saved_data, {"radius", "can_destroy_enemy"})) {
        throw std::runtime_error("Failed loading destroy building action!");
    }

    int saved_radius = saved_data["radius"];
    bool saved_can_destroy_enemy = saved_data["can_destroy_enemy"];

    radius = saved_radius;
    can_destroy_enemy = saved_can_destroy_enemy;
}

bool CDestroyBuildingsAction::execute() const {

    if(!can_destroy_enemy) {
        if(game->enemyBuildingsInArea(radius)){
            std::cout << "--> Enemy buildings in radius cannot be destroyed! <--" << std::endl;
            return false;
        }
    }

    if(!CAction::execute()) {
        return false;
    }

    game->destroyBuildingsInArea(radius);
    std::cout << "--< Destroyed buildings in radius! >--" << std::endl;
    return true;
}

std::string CDestroyBuildingsAction::printInfo() const {
    std::stringstream ss;
    ss <<  CAction::printInfo() << "\n Destroys buildings in radius: " << radius;
    if(can_destroy_enemy) {
        ss << ", can destroy enemy buildings!";
    }
    else {
        ss << ", can NOT destroy enemy buildings!";
    }

    return ss.str();
}

int CDestroyBuildingsAction::calculateValue() {
    std::map<std::string, int> multiplied_resources = game->getPlayers()[game->player_on_move]->getResources();
    for(auto & [resource, amount]: multiplied_resources) {
        amount = amount * 3;
    }
    if(game->getPlayers()[game->player_on_move]->canAfford(multiplied_resources) ) {
        return 100 * radius;
    }
    return 0;
}

std::pair<int, int> CDestroyBuildingsAction::calculateBestPosition() {
    for(int i = 0; i < game->getBoardWidth(); i++) {
        for(int j = 0; j < game->getBoardHeight(); j++) {
            if(game->getBuildingAt(i, j) == nullptr){
                continue;
            }
            else if(game->getBuildingAt(i, j)->getOwner()->getName() != game->getPlayers()[game->player_on_move]->getName()) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}
