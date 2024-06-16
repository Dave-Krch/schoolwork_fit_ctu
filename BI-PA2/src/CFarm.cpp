//
// Created by dxve on 28.5.23.
//

#include "CFarm.h"

CFarm::CFarm(char texture, std::string name, const std::map<std::string, int> &build_costs, std::string resource, int amount)
            : CBuilding(texture, std::move(name), build_costs), output_resource(std::move(resource)), output_amount(amount){}

CFarm::CFarm(const nlohmann::json &saved_data, CPlayerStats * owner) : CBuilding(saved_data, owner) {
    std::string resource = saved_data["resource"];
    int amount = saved_data["amount"];

    output_resource = resource;
    output_amount = amount;
}

void CFarm::update(std::map<std::string, std::vector<std::string>> & update_messages) {
    if(!active) return;
    owner->addResource(output_resource, output_amount);
}

std::shared_ptr<CBuilding> CFarm::buildCopy(CPlayerStats *owner, int x, int y, const std::pair<std::shared_ptr<CBuilding>, CDeposit> & tile) const {
    std::shared_ptr<CBuilding> out = std::make_shared<CFarm>(CFarm(*this));
    out->setOwner(owner);
    out->setPos(x, y);

    out->addOwnerIncome();

    return out;
}

std::string CFarm::printBuildInfo() const {
    std::stringstream ss;
    ss << name << " " << texture << "\nGenerates:\n " << output_resource << " " << output_amount << "\nCost:";
    for(auto & [resource, amount]: build_costs) {
        ss << "\n " << resource << " " << amount;
    }
    return ss.str();
}

void CFarm::saveAsJson(nlohmann::json & data) const {
    CBuilding::saveAsJson(data);

    data["resource"] = output_resource;
    data["amount"] = output_amount;
    data["type"] = "farm";
}

void CFarm::upgrade(double multiplier) {
    takeOwnerIncome();

    upgraded = true;
    double res = output_amount * multiplier;
    output_amount = int(res);

    addOwnerIncome();
}

int CFarm::calculateValue(const CPlayerStats *player, const CEndgameScenarios *end_scenarios) const {
    int val = output_amount;
    if(end_scenarios->getResourceWin().count(output_resource) > 0)
        val = val * 2;

    val = val * CBuilding::calculateValue(player, end_scenarios);

    return val;
}

void CFarm::addOwnerIncome() {
    owner->addToIncome({{output_resource, output_amount}});
}

void CFarm::takeOwnerIncome() {
    owner->removeFromIncome({{output_resource, output_amount}});
}

