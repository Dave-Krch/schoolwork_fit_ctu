//
// Created by dxve on 7.6.23.
//

#include "CCataclysmicEvent.h"

CCataclysmicEvent::CCataclysmicEvent(const nlohmann::json &saved_data, CGame *game)
                                    : CAction(saved_data, game) {
    if( !CJsonCheck::checkIfContains(saved_data, {"resource", "amount"})) {
        throw std::runtime_error("Failed loading cataclysmic event building action!");
    }


    std::string resource = saved_data["resource"];
    int amount = saved_data["amount"];
    std::string saved_texture = saved_data["texture"];

    deposit_resource = resource;
    deposit_amount = amount;
    texture = saved_texture[0];
}

bool CCataclysmicEvent::execute() const {
    if(!CAction::execute()) {
        return false;
    }

    game->destroyBuildingsInArea(0);
    game->createDeposit(deposit_resource, deposit_amount, texture);

    return true;
}

std::string CCataclysmicEvent::printInfo() const {
    std::stringstream ss;
    ss  << CAction::printInfo() << "\n Destroys selected building and creates deposit with "
        << deposit_amount << " " << deposit_resource;
    return ss.str();
}

int CCataclysmicEvent::calculateValue() {
    if(game->depositExists(deposit_resource)) return 0;
    return deposit_amount;
}

std::pair<int, int> CCataclysmicEvent::calculateBestPosition() {
    for(int i = 0; i < game->getBoardWidth(); i++) {
        for(int j = 0; j < game->getBoardHeight(); j++) {
            if(game->getBuildingAt(i, j) == nullptr){
                return {i, j};
            }
            else if(game->getBuildingAt(i, j)->getOwner()->getName() != game->getPlayers()[game->player_on_move]->getName()) {
                return {i, j};
            }
        }
    }

    return {-1, -1};
}
