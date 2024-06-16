//
// Created by dxve on 4.6.23.
//

#include "CJsonCheck.h"

bool CJsonCheck::checkIfContains(const nlohmann::json & data, const std::list<std::string> & keys) {
    for(const auto & key:  keys ) {
        if (!data.contains(key)) {
            std::cout << "Field " << key << " does not exist!" << std::endl;
            return false;
        }
    }
    return true;
}