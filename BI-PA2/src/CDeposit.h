//
// Created by dxve on 3.6.23.
//

#ifndef SEMESTRAL_CDEPOSIT_H
#define SEMESTRAL_CDEPOSIT_H

#include <string>

#include "../libs/json.hpp"

#include "CJsonCheck.h"

/**
 * @brief Represents deposit of single resource.
 *
 * Simple class representing deposit of single resource on the map. Used to check if mine can be built and for initialization of the mine.
 */
struct CDeposit{
    /**
     * Name of deposited resource.
     */
    std::string resource;
    int amount;

    /**
     * Representation on the board.
     */
    char texture;

    /**
     * Creates empty deposit.
     */
    CDeposit();

    /**
     * Loads deposit information from given json object.
     *
     * @param save_data Json object storing information about the deposit.
     */
    explicit CDeposit(const nlohmann::json & save_data);

    /**
     * @brief Constructor.
     *
     * @param resource
     * @param amount
     * @param texture
     */
    CDeposit(const std::string & resource, int amount, char texture);

    /**
     * @return True if deposit is empty (string resource is empty)
     */
    bool isEmpty() const;
};

#endif //SEMESTRAL_CDEPOSIT_H
