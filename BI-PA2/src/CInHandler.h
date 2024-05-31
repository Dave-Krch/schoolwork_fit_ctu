//
// Created by dxve on 6.6.23.
//

#ifndef SEMESTRAL_CINHANDLER_H
#define SEMESTRAL_CINHANDLER_H

#include "CPlayerStats.h"
#include "CGame.h"
#include "CAction.h"

#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>
#include <set>

/**
 * @brief Abstract class for different game input controllers.
 */
class CInHandler {
public:
    /**
     * Pointer to game controlled by the input handler.
     */
    CGame * game;

    /**
     * @brief Constructor.
     */
    explicit CInHandler(CGame * game);

    /**
     * Main method, handles user input/ai decisions and calls other methods.
     */
    virtual void getInput() = 0;
};

#endif //SEMESTRAL_CINHANDLER_H
