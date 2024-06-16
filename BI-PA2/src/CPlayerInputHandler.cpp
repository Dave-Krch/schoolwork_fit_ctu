//
// Created by dxve on 17.5.23.
//

#include "CPlayerInputHandler.h"

#include <utility>

std::string CPlayerInputHandler::new_game_directory = "examples/new_games/";
std::string CPlayerInputHandler::save_file_directory = "examples/saves/";

CPlayerInputHandler::CPlayerInputHandler(CGame * game) : CInHandler(game) {}

void CPlayerInputHandler::getInput() {
    std::cout <<
              "--< Options >--\n"
              "[x y] Select tile\n"
              "[1] Build\n"
              "[2] Actions\n"
              "[3] Help\n"
              "[4] End round\n"
              "[5] Save game\n"
              "[6] Exit game" << std::endl;

    std::map<std::string, std::function<bool()>> options = {
            {"1", [this]() {buildMenu(); return false;}},
            {"2", [this]() {actionMenu(); return false;}},
            {"3", [this]() {game->printHelp(); return true;}},
            {"4", [this]() {game->nextRound();return false;}},
            {"5", [this]() {saveMenu(); return false;}},
            {"6", [this]() {game->exit_called = true; return false;}},
    };

    while (true) {
        std::cout << "--[ Select option ]--" << std::endl;
        std::string input;
        std::getline(std::cin, input);

        if(std::cin.eof()) {
            throw std::runtime_error("End of file reached!");
        }

        auto res = options.find(input);

        if(res != options.end()) {
            if(options.at(input)()) continue;
            return;

        }

        std::istringstream iss(input);
        int x, y;

        if( iss >> x >> y) {
            if(game->changeCurrentPlayerPosition(x, y)) return;
            std::cout << "--> Invalid position! <--" << std::endl;
            continue;
        }

        std::cout << "--> Wrong input! <--" << std::endl;
    }
}

std::string CPlayerInputHandler::saveFilesMenu(const std::string & save_dir) {
    std::cout << "--[ Select file ]-- (press enter to go back)" << std::endl;

    for(const auto & file: std::filesystem::directory_iterator(save_dir)) {
        std::cout << file.path().filename() << std::endl;
    }

    while(true) {
        std::string input;
        std::getline(std::cin, input);

        if(std::cin.eof()) {
            throw std::runtime_error("End of file reached!");
        }

        if (input.empty())
            return "";

        std::filesystem::path file_path = save_dir + input;
        if (std::filesystem::exists(file_path)) {

            return file_path.string();
        }

        std::cout << "--> Wrong input! <--" << std::endl;
    }
}

std::string CPlayerInputHandler::startMenu() {
    std::cout << "--[ Main menu ]--\n"
                 "[1] New game\n"
                 "[2] Load game\n"
                 "[3] Exit" << std::endl;

    std::map<std::string, std::function<std::string ()>> options = {
            {"1", []() { return CPlayerInputHandler::saveFilesMenu(new_game_directory);} },
            {"2", []() { return CPlayerInputHandler::saveFilesMenu(save_file_directory); } },
            {"3", []() { return "exit"; } },
    };

    while(true) {
        std::string input;
        std::getline(std::cin, input);

        if(std::cin.eof()) {
            throw std::runtime_error("End of file reached!");
        }

        auto res = options.find(input);

        if(res != options.end()) {
            std::string out = options.at(input)();
            if(out.empty()) {
                std::cout << "--[ Select option ]--\n";
                continue;
            }
            else if(out == "exit"){
                return "";
            }
            return out;
        }

        std::cout << "--> Wrong input! <--" << std::endl;
    }

}

void CPlayerInputHandler::buildMenu() const {
    std::cout << "--< Possible buildings >--" << std::endl;

    for(auto & building: game->getBuildingTemplates()){
        std::cout << building.second->printBuildInfo() << std::endl << std::endl;
    }

    std::cout << "--[ Select building ]-- (press enter to go back):" << std::endl;

    while(true) {
        std::string input;
        std::getline(std::cin, input);

        if(std::cin.eof()) {
            throw std::runtime_error("End of file reached!");
        }

        if(input.empty()) return;

        if(game->getBuildingTemplates().count(input) > 0) {

            std::string message;

            if(game->build(input, message)) {
                return;
            }
            std::cout << message << std::endl;
            continue;
        }

        std::cout << "--> Wrong input! <--" << std::endl;
    }
}

void CPlayerInputHandler::saveMenu() const {
    std::cout << "--< Existing save files >--" << std::endl;

    std::set<std::string> existing_saves;

    for(const auto & file: std::filesystem::directory_iterator(save_file_directory)) {
        std::cout << file.path().filename().string() << std::endl;
        existing_saves.insert(file.path().filename().string());
    }

    std::cout << "--[ Enter save name ]-- (Only characters/numbers and _ , has to be 2+ characters long, press enter to go back)" << std::endl;

    std::string input;

    while (true) {
        std::getline(std::cin, input);

        if(std::cin.eof()) {
            throw std::runtime_error("End of file reached!");
        }

        if(input.empty())
            return;

        if(!validFileName(input)) {
            std::cout << "--> Invalid file name! <--" << std::endl;
            continue;
        }

        break;
    }

    if (existing_saves.count(input)) {
        std::cout << "--[ Do you want to overwrite existing file ]-- (y/n)" << std::endl;

        while (true) {
            std::string decision;
            std::getline(std::cin, decision);

            if(std::cin.eof()) {
                throw std::runtime_error("End of file reached!");
            }

            if (decision == "n")
                return;
            else if(decision == "y")
                break;
            else
                continue;
        }
    }

    std::string path = save_file_directory + input;
    game->saveCurrentState(path);
    std::cout << "--< Game \"" << input << "\" saved! >--" << std::endl;
}

void CPlayerInputHandler::actionMenu() const {

    std::cout << "--< Actions >--" << std::endl;

    for(const auto & [name, action]: CAction::loaded_actions) {
        std::cout << action->printInfo() << std::endl;
    }

    std::string input;

    while (true) {
        std::cout << "--[ Select action ]-- (press enter to go back)" << std::endl;
        std::getline(std::cin, input);

        if(std::cin.eof()) {
            throw std::runtime_error("End of file reached!");
        }

        if (input.empty())
            return;

        if(CAction::loaded_actions.count(input) > 0 ) {
            if(CAction::loaded_actions[input]->execute()) return;
            continue;
        }

        std::cout << "--> Wrong input! <--" << std::endl;
    }
}

bool CPlayerInputHandler::validFileName(const std::string &file_name) {

    if(file_name.size() < 2) return false;
    for(const auto & ch: file_name) {
        if(!(isalnum(ch) || ch == '_')) return false;
    }
    return true;
}

void CPlayerInputHandler::setSavePaths(std::string new_games, std::string saved_games) {
    new_game_directory = std::move(new_games);
    save_file_directory = std::move(saved_games);
}
