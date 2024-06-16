//
// Created by dxve on 15.5.23.
//

#include "CApp.h"

bool CApp::run() {

    std::string save_path = CPlayerInputHandler::startMenu();
    if (save_path.empty())
        return false;


    loadPLayers(save_path.c_str());
    //connect players to game, game to input handler
    for (auto &player: players) {
        game.addPlayer(&player.stats);

    }
    game.loadGame(save_path);

    loadActions(save_path.c_str());

    while (true) {

        while (!game.roundOver() && !game.exit_called) {
            game.draw();

            players[game.player_on_move].input_handler->getInput();
        }

        game.update();
        game.printUpdateMessages();

        if (game.exit_called || game.checkGameEnd()) break;
    }

    return true;
}

void CApp::loadPLayers(const char * save_path) {
    std::ifstream config;
    config.open(save_path);

    if(!config.is_open()) {
        throw std::runtime_error("Failed to open save_path file!");
    }

    nlohmann::json json_config;
    config >> json_config;

    if( !CJsonCheck::checkIfContains(json_config, {"players"})) {
        throw std::runtime_error("Failed loading players");
    }

    nlohmann::json saved_players = json_config["players"];

    for(const auto & saved_player: saved_players) {
        players.emplace_back(saved_player, &game);
    }
}

void CApp::loadActions(const char * save_path) {
    std::map<std::string, std::shared_ptr<CAction>> actions;

    std::ifstream config;
    config.open(save_path);

    if(!config.is_open()) {
        throw std::runtime_error("Failed to open save_path file!");
    }

    nlohmann::json json_data;
    config >> json_data;

    if( !CJsonCheck::checkIfContains(json_data, {"actions"})) {
        throw std::runtime_error("Failed loading actions");
    }

    nlohmann::json action_data = json_data["actions"];

    for(const auto & saved_action: action_data ) {

        if( !CJsonCheck::checkIfContains(saved_action, {"name", "type"})) {
            throw std::runtime_error("Failed loading action");
        }

        std::string name = saved_action["name"];
        std::string type = saved_action["type"];

        if(type == "upgrade") {
            CAction::loaded_actions[name] = std::make_shared<CUpgradeBuildingAction>(saved_action, &game);
        }
        else if(type == "destroy_building") {
            CAction::loaded_actions[name] = std::make_shared<CDestroyBuildingsAction>(saved_action, &game);
        }
        else if(type == "cataclysmic_event") {
            CAction::loaded_actions[name] = std::make_shared<CCataclysmicEvent>(saved_action, &game);
        }
    }

    //these can be hardcoded because they are same in every game
    CAction::loaded_actions["activate"] = std::make_shared<CActivateBuildingAction>(&game);
    CAction::loaded_actions["deactivate"] = std::make_shared<CDeActivateBuildingAction>(&game);
}

CApp::CApp(int argCount, char **argValues) {
    if(argCount == 1) {
        std::cout << "Arguments not passed, using default save path." << std::endl;
        return;
    }
    if(argCount != 3) {
        throw std::runtime_error("Invalid number of arguments!");
    }
    std::string new_games_path = argValues[1];
    std::string saved_games_path = argValues[2];

    if(new_games_path.empty() || saved_games_path.empty()) {
        throw std::runtime_error("Invalid arguments!");
    }

    CPlayerInputHandler::setSavePaths(new_games_path, saved_games_path);
}


