//
// Created by dxve on 3.6.23.
//

#include "CFactory.h"

#include <utility>

CFactory::CFactory(char texture,
                   std::string name,
                   const std::map<std::string, int> &build_costs,
                   const std::map<std::string, int> &output_resources,
                   const std::map<std::string, int> &input_resources) :
        CBuilding(texture, std::move(name), build_costs),
        output_resources(output_resources),
        input_resources(input_resources) {}

CFactory::CFactory(const nlohmann::json &save_data,
                   CPlayerStats *owner) :
        CBuilding(save_data, owner) {
        std::map<std::string, int> saved_output = save_data["output_resources"];
        std::map<std::string, int> saved_input = save_data["input_resources"];

        output_resources = saved_output;
        input_resources = saved_input;
}

void CFactory::update(std::map<std::string, std::vector<std::string>> & update_messages) {

    if(!active) return;

    if(!owner->canAfford(input_resources)) {
        std::stringstream ss;
        ss << name << " at: " << x_pos << " " << y_pos << " cannot produce, insufficient resources";
        update_messages[owner->getName()].push_back(ss.str());
        return;
    }

    owner->removeResources(input_resources);

    for(const auto & [resource, amount]: output_resources) {
        owner->addResource(resource, amount);
    }
}

std::shared_ptr<CBuilding> CFactory::buildCopy(CPlayerStats *owner, int x, int y, const std::pair<std::shared_ptr<CBuilding>, CDeposit> & tile) const {
    std::shared_ptr<CBuilding> out = std::make_shared<CFactory>(CFactory(*this));
    out->setOwner(owner);
    out->setPos(x, y);

    out->addOwnerIncome();

    return out;
}

std::string CFactory::printBuildInfo() const {
    std::stringstream ss;
    ss << name << " " << texture << "\nGenerates:";
    for(const auto & [resource, amount]: output_resources) {
        ss << "\n   " << amount << " " << resource;
    }

    ss << "\nFor:";
    for(const auto & [resource, amount]: input_resources) {
        ss << "\n   " << amount << " " << resource;
    }

    ss << "\nCosts:";
    for(const auto & [resource, amount]: build_costs) {
        ss << "\n   " << amount << " " << resource;
    }

    return ss.str();
}

void CFactory::saveAsJson(nlohmann::json &data) const {
    CBuilding::saveAsJson(data);

    data["output_resources"] = output_resources;
    data["input_resources"] = input_resources;
    data["type"] = "factory";
}

void CFactory::upgrade(double multiplier) {
    takeOwnerIncome();

    upgraded = true;
    for(auto & [resource, amount]: output_resources) {
        amount = int(amount * multiplier);
    }

    addOwnerIncome();
}

int CFactory::calculateValue(const CPlayerStats *player, const CEndgameScenarios *end_scenarios) const {
    int value_sum = 0;
    for(const auto & [resource, amount]: output_resources) {
        if(end_scenarios->getResourceWin().count(resource)) {
            value_sum += amount * 2;
            continue;
        }
        value_sum += amount;
    }

    value_sum = value_sum * CBuilding::calculateValue(player, end_scenarios);

    for(const auto & [resource, amount]: input_resources) {
        if(player->getIncome().at(resource) <= amount ) {
            return value_sum * 0;
        }
    }

    return value_sum;
}

void CFactory::addOwnerIncome() {
    owner->addToIncome(output_resources);
    owner->removeFromIncome(input_resources);
}

void CFactory::takeOwnerIncome() {
    owner->addToIncome(input_resources);
    owner->removeFromIncome(output_resources);
}
