//
// Created by dxve on 4.6.23.
//

#include "CUpgradeBuilding.h"

CUpgradeBuildingAction::CUpgradeBuildingAction(const nlohmann::json &saved_data, CGame *game)
                                                : CAction(saved_data, game) {
    if( !CJsonCheck::checkIfContains(saved_data, {"mult"})) {
        throw std::runtime_error("Failed loading upgrade multiplier!");
    }

    double saved_mult = saved_data["mult"];
    multiplier = saved_mult;
}

bool CUpgradeBuildingAction::execute() const {

    if(!CAction::execute())
        return false;

    if(game->upgradeSelectedBuilding(multiplier)) {

        std::cout << "--< Building upgraded! >--" << std::endl;
        return true;
    }
    std::cout << "--> Cannot upgrade! <--" << std::endl;
    return false;
}

std::string CUpgradeBuildingAction::printInfo() const {
    std::stringstream ss;
    ss <<  CAction::printInfo() << "\n  Multiplies building production by:" << multiplier;

    return ss.str();
}

int CUpgradeBuildingAction::calculateValue() {
    if(multiplier < 1)
        return 0;
    return int(multiplier * 100);
}

std::pair<int, int> CUpgradeBuildingAction::calculateBestPosition() {
    for(int i = 0; i < game->getBoardWidth(); i++) {
        for(int j = 0; j < game->getBoardHeight(); j++) {
            if(game->getBuildingAt(i, j) == nullptr){
                continue;
            }
            else if(game->getBuildingAt(i, j)->getOwner()->getName() == game->getPlayers()[game->player_on_move]->getName()) {
                if(game->getBuildingAt(i, j)->upgraded)
                    continue;
                return {i, j};
            }
        }
    }
    return {-1, -1};
}
