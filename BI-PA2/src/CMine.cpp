//
// Created by dxve on 3.6.23.
//

#include "CMine.h"

#include <utility>

CMine::CMine(char texture, std::string name,
             std::string deposit,
             int deposit_amount,
             int input_amount,
             const std::map<std::string, int> &build_costs,
             const std::map<std::string, int> &output) :
        CBuilding(texture, std::move(name), build_costs),
        deposit(std::move(deposit)),
        deposit_amount(deposit_amount),
        input_amount(input_amount) {}

CMine::CMine(const nlohmann::json &save_data,
             CPlayerStats *owner) :
        CBuilding(save_data, owner) {

    if( !CJsonCheck::checkIfContains(save_data, {"deposit", "deposit_amount", "input_amount", "output_resources"})) {
        throw std::runtime_error("Failed loading CMine!");
    }


    std::string saved_deposit = save_data["deposit"];
    int saved_deposit_amount = save_data["deposit_amount"];
    int saved_input_amount = save_data["input_amount"];
    std::map<std::string, int> saved_output = save_data["output_resources"];

    deposit = saved_deposit;
    deposit_amount = saved_deposit_amount;
    input_amount = saved_input_amount;
    output_resources = saved_output;
}

void CMine::update(std::map<std::string, std::vector<std::string>> &update_messages) {

    if(!active) return;

    if(deposit_amount < input_amount) {
        std::stringstream ss;
        ss << name << " at: " << x_pos << " " << y_pos << " cannot produce, deposit empty";
        update_messages[owner->getName()].push_back(ss.str());
        return;
    }

    deposit_amount -= input_amount;

    for(auto & [resource, amount]: output_resources) {
        owner->addResource(resource, amount);
    }
}

std::shared_ptr<CBuilding> CMine::buildCopy(CPlayerStats *owner, int x, int y, const std::pair<std::shared_ptr<CBuilding>, CDeposit> & tile) const {
    const auto & [tile_building, tile_deposit] = tile;
    CMine tmp = CMine(*this);
    tmp.deposit = tile_deposit.resource;
    tmp.deposit_amount = tile_deposit.amount;

    std::shared_ptr<CBuilding> out = std::make_shared<CMine>(tmp);
    out->setOwner(owner);
    out->setPos(x, y);

    out->addOwnerIncome();

    return out;
}

std::string CMine::printBuildInfo() const {
    std::stringstream ss;
    ss << name << " " << texture << "\nGenerates:";
    for(auto & [resource, amount]: output_resources) {
        ss << "\n   " << amount << " " << resource;
    }
    ss << "\nNeeds to be built on " << deposit << " deposit\nCosts:";

    for(const auto & [resource, amount]: build_costs) {
        ss << "\n   " << amount << " " << resource;
    }

    return ss.str();
}

void CMine::saveAsJson(nlohmann::json &data) const {
    CBuilding::saveAsJson(data);

    data["type"] = "mine";
    data["deposit"] = deposit;
    data["deposit_amount"] = deposit_amount;
    data["input_amount"] = input_amount;
    data["output_resources"] = output_resources;
}

bool CMine::canBeBuilt(const std::pair<std::shared_ptr<CBuilding>, CDeposit> &tile) const {
    const auto & [tile_building, tile_deposit] = tile;
    if(tile_building != nullptr || tile_deposit.resource != deposit) return false;
    return true;
}

void CMine::upgrade(double multiplier) {
    takeOwnerIncome();

    upgraded = true;
    for(auto & [resource, amount]: output_resources) {
        amount = int(amount * multiplier);
    }

    addOwnerIncome();
}

int CMine::calculateValue(const CPlayerStats *player, const CEndgameScenarios *end_scenarios) const {
    int value_sum = 0;
    for(const auto & [resource, amount]: output_resources) {
        if(end_scenarios->getResourceWin().count(resource)) {
            value_sum += amount * 2;
            continue;
        }
        value_sum += amount;
    }

    value_sum = value_sum * CBuilding::calculateValue(player, end_scenarios);

    return value_sum;
}

void CMine::addOwnerIncome() {
    owner->addToIncome(output_resources);
}

void CMine::takeOwnerIncome() {
    owner->removeFromIncome(output_resources);
}