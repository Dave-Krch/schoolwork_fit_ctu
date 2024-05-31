//
// Created by dxve on 6.6.23.
//

#include "CInputHandlerAi.h"

CInputHandlerAi::CInputHandlerAi(CGame *game, int max_moves) : CInHandler(game), move_counter(0), max_moves(max_moves) {

}

void CInputHandlerAi::calculateBuildingValues(std::vector<std::pair<int, std::string>> & sorted_buildings) const {
    for(const auto & [building_name, building_stats]: game->getBuildingTemplates()) {
        if (game->getPlayers()[game->player_on_move]->canAfford(building_stats->getBuildCosts())) {
            int val = building_stats->calculateValue(game->getPlayers()[game->player_on_move], &game->getEndGameScenarios());

            if(val > 0)
                sorted_buildings.emplace_back(val, building_name);
        }
    }

    if(sorted_buildings.empty())
        return;

    std::sort(sorted_buildings.begin(), sorted_buildings.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });

}

void CInputHandlerAi::calculateActionValues(std::vector<std::pair<int, std::string>> &sorted_actions, std::map<std::string, std::pair<int, int>> & best_tiles_for_action) const {
    for(const auto & [name, action]: CAction::loaded_actions) {
        if (game->getPlayers()[game->player_on_move]->canAfford(action->getExecuteCost())) {
            int value = 0;
            std::pair<int, int> pos;

            value = action->calculateValue();
            if(value <= 0)
                continue;

            pos = action->calculateBestPosition();
            if(pos.first <= -1 && pos.second <= -1) {
                continue;
            }

            sorted_actions.emplace_back(value, name);
            best_tiles_for_action.insert({name, pos});
        }
    }
}

void CInputHandlerAi::getInput() {

    if(move_counter == max_moves) {
        move_counter = 0;
        game->nextRound();
        return;
    }

    std::vector<std::pair<int, std::string>> sorted_buildings;
    std::vector<std::pair<int, std::string>> sorted_actions;
    std::map<std::string, std::pair<int, int>> best_tiles_for_action;

    calculateBuildingValues(sorted_buildings);
    calculateActionValues(sorted_actions, best_tiles_for_action);

    if(sorted_buildings.empty()) {
        if(sorted_actions.empty()) {
            game->nextRound();
            return;
        }
    }

    if(sorted_actions.empty() || sorted_actions.back().first < sorted_buildings.back().first) {
        while( !tryToBuild(sorted_buildings.back().second) ) {
            sorted_buildings.pop_back();
            if(sorted_buildings.empty()) {
                game->nextRound();
                return;
            }
        }
        move_counter ++;
        return;
    }

    executeAction(sorted_actions.back().second, best_tiles_for_action.at(sorted_actions.back().second));
    sorted_actions.pop_back();
    move_counter ++;
}

bool CInputHandlerAi::tryToBuild(const std::string &building_name) const {
    std::string message;
    for(int i = 0; i < game->getBoardWidth(); i++) {
        for(int j = 0; j < game->getBoardHeight(); j++) {
            game->changeCurrentPlayerPosition(j, i);
            if(game->getBuildingTemplates().at(building_name)->canBeBuilt(game->getSelectedTile())) {
                game->build(building_name, message);
                std::cout << "Ai building: " << building_name << "<-----------------------------------------" << std::endl;
                return true;
            }
        }
    }
    std::cout << message << std::endl;
    return false;
}

void CInputHandlerAi::executeAction(const std::string &action_name, std::pair<int, int> position) const {
    game->changeCurrentPlayerPosition(position.first, position.second);
    CAction::loaded_actions.at(action_name)->execute();
    std::cout << "Ai executing: " << action_name << "<-----------------------------------------" << std::endl;
}


