//
// Created by dxve on 5.6.23.
//

#include "CActivateBuildingAction.h"

CActivateBuildingAction::CActivateBuildingAction(CGame * game) : CAction(game, "activate") {

}

bool CActivateBuildingAction::execute() const {
    if(game->accessSelectedBuilding() == nullptr) {
        std::cout << "--> Not a building! <--" << std::endl;
        return false;
    }

    if(game->accessSelectedBuilding()->getOwner() == game->accessPlayerOnMove()) {
        game->accessSelectedBuilding()->activate();
        std::cout << "--< Selected building activated! >--" << std::endl;
        return true;
    }

    std::cout << "--> Cannot activate enemy building! <--" << std::endl;
    return false;
}

std::string CActivateBuildingAction::printInfo() const {
    return "[activate]: Activates selected building";
}
