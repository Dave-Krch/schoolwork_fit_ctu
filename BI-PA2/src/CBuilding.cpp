//
// Created by dxve on 15.5.23.
//

#include "CBuilding.h"

CBuilding::CBuilding( char texture, std::string name, const std::map<std::string, int> & build_costs)
                    : texture(texture), name(std::move(name)), owner(nullptr), build_costs(build_costs), active(false), upgraded(
        false), x_pos(-1), y_pos(-1){}

CBuilding::CBuilding(const nlohmann::json &saved_data, CPlayerStats * owner) : owner(owner), x_pos(-1), y_pos(-1) {

    if( !CJsonCheck::checkIfContains(saved_data, {"name", "texture", "costs", "x", "y", "active", "upgraded"})) {
        throw std::runtime_error("Failed loading building!");
    }

    std::string saved_name = saved_data["name"];
    std::string saved_texture = saved_data["texture"];
    std::map<std::string, int> saved_costs = saved_data["costs"];
    int x = saved_data["x"];
    int y = saved_data["y"];
    bool saved_active = saved_data["active"];
    bool saved_upgraded = saved_data["upgraded"];

    name = saved_name;
    texture = saved_texture[0];
    build_costs = saved_costs;
    x_pos = x;
    y_pos = y;
    active = saved_active;
    upgraded = saved_upgraded;
}

void CBuilding::setOwner(CPlayerStats * new_owner) {
    this->owner = new_owner;
}

void CBuilding::setPos(int x, int y) {
    x_pos = x;
    y_pos = y;
}

char CBuilding::getTexture() const {
    return texture;
}

const std::map<std::string, int> &CBuilding::getBuildCosts() const {
    return build_costs;
}

bool CBuilding::canBeBuilt(const std::pair<std::shared_ptr<CBuilding>, CDeposit> & tile) const {
    if(tile.first == nullptr) return true;
    return false;
}

const CPlayerStats *CBuilding::getOwner() const{
    return owner;
}

void CBuilding::saveAsJson(nlohmann::json &data) const {
    data["name"] = name;
    data["texture"] = std::string(1, texture);
    data["costs"] = build_costs;
    data["x"] = x_pos;
    data["y"] = y_pos;
    data["active"] = active;
    data["upgraded"] = upgraded;

    if(owner == nullptr)
        data["owner"] = nullptr;
    else
        data["owner"] = owner->getName();
}

void CBuilding::activate() {
    active = true;
}

void CBuilding::deActivate() {
    active = false;
}

int CBuilding::calculateValue(const CPlayerStats *player, const CEndgameScenarios *end_scenarios) const {
    int multiplier = 1;
    for(const auto & [resource, amount]: build_costs) {

        if((player->getIncome().at(resource) == 0) && ( (player->getResources().at(resource) - amount) == 0))
            multiplier = multiplier * 0;
    }
    return multiplier;
}








