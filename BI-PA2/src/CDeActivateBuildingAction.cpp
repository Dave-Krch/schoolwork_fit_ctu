//
// Created by dxve on 5.6.23.
//

#include "CDeActivateBuildingAction.h"

CDeActivateBuildingAction::CDeActivateBuildingAction(CGame * game) : CAction(game, "activate") {

}

bool CDeActivateBuildingAction::execute() const {
    if(game->accessSelectedBuilding() == nullptr) {
        std::cout << "--> Not a building! <--" << std::endl;
        return false;
    }

    if(game->accessSelectedBuilding()->getOwner() == game->accessPlayerOnMove()) {
        game->accessSelectedBuilding()->deActivate();
        std::cout << "--< Selected building deactivated! >--" << std::endl;
        return true;
    }

    std::cout << "--> Cannot deactivate enemy building! <--" << std::endl;
    return false;
}

std::string CDeActivateBuildingAction::printInfo() const {
    return "[deactivate]: Deactivates selected building";
}
