//
// Created by dxve on 4.6.23.
//

#ifndef SEMESTRAL_CJSONCHECK_H
#define SEMESTRAL_CJSONCHECK_H

#include "../libs/json.hpp"

#include "list"
#include <iostream>

/**
 * @brief Methods for checking json objects.
 */
class CJsonCheck{
public:
    /**
     * Checks if variables with given keys exists in given object.
     *
     * @param data
     * @param keys
     * @return False if any key is missing.
     */
    static bool checkIfContains(const nlohmann::json & data, const std::list<std::string> & keys);
};

#endif //SEMESTRAL_CJSONCHECK_H
