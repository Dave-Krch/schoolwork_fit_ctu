//
// Created by dxve on 15.5.23.
//
#ifndef SEMESTRAL_CAPP_H
#define SEMESTRAL_CAPP_H

#include "CGame.h"
#include "CPlayer.h"
#include "CPlayerInputHandler.h"
#include "CAction.h"
#include "CUpgradeBuilding.h"
#include "CActivateBuildingAction.h"
#include "CDeActivateBuildingAction.h"
#include "CDestroyBuildingsAction.h"
#include "CCataclysmicEvent.h"

#include "fstream"
#include "iostream"

#include "../libs/json.hpp"

/**
 * @brief Stores info about game and players, handles game initialization and game loop.
 */
class CApp{
    /**
     * Stores and manages all data
     */
    CGame game;
    /**
     * Stores players
     */
    std::vector<CPLayer> players;
public:

    /**
     * Takes arguments from main and stores them into path variables stored in CPlayerInputHandler
     *
     * @param argCount
     * @param argValues
     */
    CApp(int argCount, char* argValues[]);
    /**
     * Method loads players, game state, actions and winning/loosing conditions from file given by start menu. Then manages game loop.
     *
     * @return false when game exit has been called
     */
    bool run();
    /**
     * Loads players from given new_games file.
     *
     * @param save_path path to configuration or save file
     */
    void loadPLayers(const char * save_path);

    /**
     * Loads actions from save file.
     */
    void loadActions(const char * save_path);
};

#endif //SEMESTRAL_CAPP_H
