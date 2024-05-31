//
// Created by dxve on 15.5.23.
//

#include "CPlayerStats.h"

#include <utility>

CPlayerStats::CPlayerStats() : selected_x(0), selected_y(0) {}

CPlayerStats::CPlayerStats(int x, int y, std::string name, const std::map<std::string, int> & resources, const std::map<std::string, int> & income)
                          : name(std::move(name)), resources(resources), income(income), selected_x(x), selected_y(y) {}

void CPlayerStats::setSelectedPosition(int x, int y) {
    selected_x = x;
    selected_y = y;
}

std::ostream &operator<<(std::ostream &oss, const CPlayerStats &player)  {
    oss << "Player: " << player.name << "\nResources:" ;
    for(auto & [resource, amount]: player.resources) {
        oss << "\n  " << resource << ": " << amount;
    }
    oss << "\nIncome:";
    for(auto & [resource, amount]: player.income) {
        oss << "\n  " << resource << ": " << amount;
    }
    return oss;
}

void CPlayerStats::addResource(const std::string& resource, int amount) {
    resources[resource] += amount;
}

bool CPlayerStats::removeResources(const std::map<std::string, int> &resources_to_remove) {
    for(auto & [resource, amount]: resources_to_remove) {
        if(resources[resource] < amount) return false;
    }

    for(auto & [resource, amount]: resources_to_remove) {
        resources[resource] -= amount;
    }
    return true;
}

const std::string &CPlayerStats::getName() const {
    return name;
}

bool CPlayerStats::canAfford(const std::map<std::string, int> &costs) const {
    for(const auto & [resource, amount]: costs) {
        if(resources.at(resource) < amount)
            return false;
    }
    return true;
}

int CPlayerStats::sumResources() const {
    int sum = 0;

    for(const auto & [resource, amount]: resources) {
        sum += amount;
    }

    return sum;
}

const std::map<std::string, int> &CPlayerStats::getResources() const {
    return resources;
}

void CPlayerStats::addToIncome(const std::map<std::string, int> &resources_to_add) {
    for(const auto & [resource, amount]: resources_to_add) {
        income[resource] += amount;
    }
}

void CPlayerStats::removeFromIncome(const std::map<std::string, int> &resources_to_remove) {
    for(const auto & [resource, amount]: resources_to_remove) {
        income[resource] -= amount;
    }
}

const std::map<std::string, int> &CPlayerStats::getIncome() const {
    return income;
}

