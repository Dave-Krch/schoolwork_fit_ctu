//
// Created by dxve on 1.6.23.
//

#include "CPlayer.h"

#include <utility>

CPLayer::CPLayer( int x, int y, std::string name,const std::map<std::string, int> & resources, const std::map<std::string, int> & income)
                : stats(x, y, std::move(name), resources, income) {

}

CPLayer::CPLayer(const nlohmann::json &saved_player, CGame *game) {
    if( !CJsonCheck::checkIfContains(saved_player, {"name", "type", "resources", "income", "x", "y", "ai_moves"})) {
        throw std::runtime_error("Failed loading player");
    }

    std::map<std::string, int> resources = saved_player["resources"];
    std::map<std::string, int> income = saved_player["income"];

    if(resources.empty()) {
        throw std::runtime_error("Failed loading player, resources cannot be empty!");
    }

    std::string name = saved_player["name"];
    std::string type = saved_player["type"];
    int x = saved_player["x"];
    int y = saved_player["y"];
    int ai_moves = saved_player["ai_moves"];

    stats = CPlayerStats(x, y, std::move(name), resources, income);

    if(type == "human") {
        input_handler = std::make_shared<CPlayerInputHandler>(game);
    }
    else if(type == "ai") {
        input_handler = std::make_shared<CInputHandlerAi>(game, ai_moves);
    }
}
