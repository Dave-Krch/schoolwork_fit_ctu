//
// Created by dxve on 15.5.23.
//
#include "CGame.h"

// TODO: header knihovna z courses
#define RED_START "\033[31m"
#define BLUE_START "\033[34m"
#define YELLOW_START "\033[32m"
#define COLOUR_ESCAPE "\033[0m"

//TODO: fix printing for 2+ digit sized maps

void CGame::draw() const{

    for(int i = 0; i < (board_width * 4) + 10; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;

    std::cout << "  ";

    for(int i = 0; i < board_width; i++) {
        std::cout << i;

        int tmp = i / 10;

        for(int j = 3; j > tmp; j-- ) {
            std::cout << " ";
        }
    }
    std::cout << " <- X\n";

    for(int i = 0; i < board_height; i++) {

        for(int j = 0; j < board_width; j++) {
            std::cout << RED_START << "+ - " << COLOUR_ESCAPE;
        }
        std::cout << RED_START << "+" << COLOUR_ESCAPE;
        std::cout << "\n";

        for(int j = 0; j < board_width; j++) {
            std::string tile = " ";

            const auto & [building, deposit] = board[j][i];

            if(building != nullptr) {
                tile = std::string(1, building->getTexture());
                if(building->getOwner() == players[player_on_move]) {
                    tile = BLUE_START + std::string(1, building->getTexture()) + COLOUR_ESCAPE;
                }
            }
            else if(!deposit.isEmpty()) {
                tile = YELLOW_START + std::string(1, deposit.texture) + COLOUR_ESCAPE;
            }
            if(players[player_on_move]->selected_x == j && players[player_on_move]->selected_y == i ) {
                std::cout << RED_START << "|" << COLOUR_ESCAPE << BLUE_START << "[" << COLOUR_ESCAPE << tile << BLUE_START << "]" << COLOUR_ESCAPE;
            }
            else {
                std::cout << RED_START << "| " << COLOUR_ESCAPE << tile << " ";
            }
        }
        std::cout << RED_START << "|" << COLOUR_ESCAPE << "  " << i << "\n";
    }

    for(int i = 0; i < board_width; i++) {
        std::cout << RED_START << "+ - " << COLOUR_ESCAPE;
    }
    std::cout << RED_START << "+" << COLOUR_ESCAPE << std::endl;

    std::cout << (*players[player_on_move]) << std::endl;

    for(int i = 0; i < (board_width * 4) + 10; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;
}

void CGame::loadGame(const std::string & save_file_path) {
    save_path = save_file_path;
    std::ifstream config;
    config.open(save_path);

    if(!config.is_open()) {
        throw std::runtime_error("Failed to open new_games file!");
    }

    nlohmann::json save_file_json;
    config >> save_file_json;

    if( !CJsonCheck::checkIfContains(save_file_json, {"width", "height", "building_templates", "board", "deposits", "player_on_move", "endgame_scenarios"})) {
        throw std::runtime_error("Failed loading game!");
    }

    board_width = save_file_json["width"];
    board_height = save_file_json["height"];
    board = std::vector<std::vector<std::pair<std::shared_ptr<CBuilding>, CDeposit>>>(board_width,std::vector<std::pair<std::shared_ptr<CBuilding>, CDeposit>>(board_height, {nullptr,CDeposit()}));

    loadBuildingTemplates(save_file_json["building_templates"]);

    loadBoardState(save_file_json["board"]);

    loadDeposits(save_file_json["deposits"]);

    exit_called = false;
    player_on_move = save_file_json["player_on_move"];

    //loading endgame scenarios
    end_scenarios = CEndgameScenarios(save_file_json["endgame_scenarios"], players_map);
}

void CGame::loadBuildingTemplates(const nlohmann::json &building_templates_json) {
    for (const auto & json_building : building_templates_json ) {

        if( !CJsonCheck::checkIfContains(json_building, {"type", "name"})) {
            throw std::runtime_error("Failed loading building templates!");
        }

        std::string type = json_building["type"];
        std::string name = json_building["name"];

        if(type == "farm") {
            building_templates.insert({name, std::make_shared<CFarm>(json_building, nullptr)});
        }
        else if(type == "factory") {
            building_templates.insert({name, std::make_shared<CFactory>(json_building, nullptr)});
        }
        else if(type == "mine") {
            building_templates.insert({name, std::make_shared<CMine>(json_building, nullptr)});
        }
    }
}

void CGame::loadBoardState(const nlohmann::json & board_state_json) {
    for (auto it = board_state_json.begin(); it != board_state_json.end(); it++) {

        if( !CJsonCheck::checkIfContains(it.value(), {"type", "name", "owner", "x", "y"})) {
            throw std::runtime_error("Failed loading board state!");
        }

        std::string type = it.value()["type"];
        std::string name = it.value()["name"];

        int x = it.value()["x"];
        int y = it.value()["y"];

        std::string owner_name;
        CPlayerStats * owner;

        if(it.value()["owner"].is_null())
            owner = nullptr;
        else {
            owner_name = it.value()["owner"];
            owner = players_map[owner_name];
        }

        auto & [building, deposit] = board[x][y];

        if(type == "farm") {
            building = std::make_shared<CFarm>(it.value(), owner);
            buildings.push_back(building);
        }
        else if(type == "factory") {
            building = std::make_shared<CFactory>(it.value(), owner);
            buildings.push_back(building);
        }
        else if(type == "mine") {
            building = std::make_shared<CMine>(it.value(), owner);
            buildings.push_back(building);
        }
    }
}

void CGame::loadDeposits(const nlohmann::json & deposit_json) {

    for(const auto & saved_deposit: deposit_json) {
        deposit_types.push_back({CDeposit(saved_deposit), {}});

        for(const auto & position: saved_deposit["positions"]) {

            if( !CJsonCheck::checkIfContains(position, {"x", "y"})) {
                throw std::runtime_error("Failed loading game - deposits - positions!");
            }

            int saved_x = position["x"];
            int saved_y = position["y"];

            board[saved_x][saved_y].second = CDeposit(saved_deposit);
            deposit_types.back().second.emplace_back(saved_x, saved_y);
        }
    }

}

void CGame::update()  {
    for(auto & building: buildings) {
        building->update(update_messages);
    }
}

void CGame::printUpdateMessages() {
    std::cout << "--> Update messages <--" << std::endl;
    for(auto & [name, messages]: update_messages) {
        std::cout << "Player " << name << ":";
        for(auto & message: messages) {
            std::cout << "\n   " << message;
        }
    }
    std::cout << std::endl;
    update_messages.clear();
}

void CGame::addPlayer(CPlayerStats *player) {
    players.push_back(player);
    players_map.insert({player->getName(), player});
}

const std::map<std::string, std::shared_ptr<CBuilding>> & CGame::getBuildingTemplates() const {
    return building_templates;
}

bool CGame::build(const std::string &building_name, std::string & message) {
    if(building_templates[building_name]->canBeBuilt(getSelectedTile())){
        if (players[player_on_move]->removeResources(building_templates[building_name]->getBuildCosts())) {
            //builds the building
            //TODO: position struct
            board[players[player_on_move]->selected_x][players[player_on_move]->selected_y].first
                = building_templates[building_name]->buildCopy(players[player_on_move], players[player_on_move]->selected_x, players[player_on_move]->selected_y, board[players[player_on_move]->selected_x][players[player_on_move]->selected_y]);

            buildings.push_back(getSelectedTile().first);

            //destroys deposit on tile
            board[players[player_on_move]->selected_x][players[player_on_move]->selected_y].second = CDeposit();
            return true;
        }
        message = "--> Insufficient resources <--";
        return false;
    }
    message = "--> Cannot build there! <--";
    return false;
}

void CGame::nextRound() {
    player_on_move ++;
}

bool CGame::roundOver() {
    if(size_t(player_on_move) == players.size()) {
        player_on_move = 0;
        return true;
    }
    return false;
}

bool CGame::changeCurrentPlayerPosition(int x, int y) const {
    if (x >= board_width || x < 0 || y >= board_height || y < 0) return false;
    players[player_on_move]->setSelectedPosition(x, y);
    return true;
}

const std::pair<std::shared_ptr<CBuilding>, CDeposit> & CGame::getSelectedTile() const{
    return board[players[player_on_move]->selected_x][players[player_on_move]->selected_y];
}

void CGame::saveCurrentState(const std::string & new_save_path) const {
    nlohmann::json save_file_json;
    save_file_json["width"] = board_width;
    save_file_json["height"] = board_height;
    save_file_json["player_on_move"] = player_on_move;

    //copying fields that does not change in game from initial save file
    std::ifstream initial_save_file;
    initial_save_file.open(save_path);
    if(!initial_save_file.is_open()) {
        throw std::runtime_error("Failed to open new_games file!");
    }
    nlohmann::json initial_save_json;
    initial_save_file >> initial_save_json;

    save_file_json["building_templates"] = initial_save_json["building_templates"];
    save_file_json["actions"] = initial_save_json["actions"];
    save_file_json["deposits"] = initial_save_json["deposits"];

    //players change only in amount of resources and income
    save_file_json["players"] = initial_save_json["players"];
    for(const auto & player: players) {
        save_file_json["players"][player->getName()]["resources"] = player->getResources();
        save_file_json["players"][player->getName()]["income"] = player->getIncome();
    }

    //saves current state of board
    nlohmann::json buildings_on_board;
    size_t i = 0;
    for(const auto & building: buildings) {
        nlohmann::json save_building;
        building->saveAsJson(save_building);
        buildings_on_board[std::to_string(i)] = save_building;
        i++;
    }
    save_file_json["board"] = buildings_on_board;

    //saves end scenarios
    nlohmann::json endgame_scenarios_json;
    end_scenarios.saveAsJson(endgame_scenarios_json);
    save_file_json["endgame_scenarios"] = endgame_scenarios_json;

    //save json to file
    std::ofstream save_file(new_save_path);
    if(!save_file.is_open())
        throw std::runtime_error("Failed to open save file!");
    save_file << save_file_json.dump(4);
}

void CGame::printHelp() const {
    std::cout << "--< Help >--\n";

    std::cout << "Current round: " << end_scenarios.getRoundCount() << " from " << end_scenarios.getMaxRounds() << "\nPlayers:";

    for(const auto & [name, data]: players_map) {
        std::cout << "\n    " << name;
    }

    std::cout << "\nDeposit types:";
    for(const auto & [deposit, positions]: deposit_types) {
        std::cout << "\n    " << deposit.amount << " " << deposit.resource << " " << deposit.texture;
    }

    std::cout << "\nBuilding templates:";
    for(const auto & [name, data]: building_templates) {
        std::cout << "\n    " << name << " " << data->getTexture();
    }

    std::cout << std::endl;

    end_scenarios.printEndScenariosHelp();
}

bool CGame::upgradeSelectedBuilding(double multiplier) const {
    if(!playerSelectedBuilding())
        return false;
    board[players[player_on_move]->selected_x][players[player_on_move]->selected_y].first->upgrade(multiplier);
    return true;
}

bool CGame::playerSelectedBuilding() const {
    if(board[players[player_on_move]->selected_x][players[player_on_move]->selected_y].first == nullptr)
        return false;
    return true;
}

CPlayerStats *CGame::accessPlayerOnMove() const {
    return players[player_on_move];
}

CBuilding *CGame::accessSelectedBuilding() const {
    return board[players[player_on_move]->selected_x][players[player_on_move]->selected_y].first.get();
}

void CGame::destroyBuildingsInArea(int radius) {
    int x_from, x_to, y_from, y_to;
    int selected_x = players[player_on_move]->selected_x;
    int selected_y = players[player_on_move]->selected_y;

    x_from = selected_x - radius;
    x_to = selected_x + radius;
    y_from = selected_y - radius;
    y_to = selected_y + radius;

    if(x_from < 0)
        x_from = 0;
    if(x_to > (board_width - 1))
        x_to = (board_width - 1);
    if(y_from < 0)
        y_from = 0;
    if(y_to > (board_height - 1))
        y_to = (board_height - 1);

    for(int x = x_from; x <= x_to; x++) {
        for(int y = y_from; y <= y_to; y++) {
            if(board[x][y].first == nullptr) continue;
            destroyBuildingOnPos(x,y);
        }
    }
}

void CGame::destroyBuildingOnPos(int x, int y) {
    for (auto it = buildings.begin(); it != buildings.end();) {
        if ((*it)->x_pos == x && (*it)->y_pos == y) {
            it = buildings.erase(it);
        } else {
            ++it;
        }
    }
    board[x][y].first->takeOwnerIncome();
    board[x][y].first = nullptr;
}

bool CGame::enemyBuildingsInArea(int radius) const {
    int x_from, x_to, y_from, y_to;
    int selected_x = players[player_on_move]->selected_x;
    int selected_y = players[player_on_move]->selected_y;

    x_from = selected_x - radius;
    x_to = selected_x + radius;
    y_from = selected_y - radius;
    y_to = selected_y + radius;

    if(x_from < 0)
        x_from = 0;
    if(x_to > (board_width - 1))
        x_to = (board_width - 1);
    if(y_from < 0)
        y_from = 0;
    if(y_from > (board_height - 1))
        y_to = (board_height - 1);

    for(int x = x_from; x <= x_to; x++) {
        for(int y = y_from; y <= y_to; y++) {
            if(board[x][y].first == nullptr) continue;
            if(board[x][y].first->getOwner()->getName() != players[player_on_move]->getName()) return true;
        }
    }
    return false;
}

bool CGame::checkGameEnd() {
    return end_scenarios.checkForGameEnd();
}

const std::vector<CPlayerStats *> &CGame::getPlayers() const{
    return players;
}

int CGame::getBoardWidth() const {
    return board_width;
}

int CGame::getBoardHeight() const {
    return board_height;
}

const CEndgameScenarios &CGame::getEndGameScenarios() const {
    return end_scenarios;
}

void CGame::createDeposit(const std::string &resource, int amount, char texture) {
    board[players[player_on_move]->selected_x][players[player_on_move]->selected_y].second = CDeposit(resource, amount, texture);
}

bool CGame::depositExists(const std::string &resource) {
    for(const auto & row: board) {
        for (const auto & [building, deposit]: row) {
            if(deposit.resource == resource) return true;
        }
    }
    return false;
}

const CBuilding *CGame::getBuildingAt(int x, int y) const {
    return board[x][y].first.get();
}

