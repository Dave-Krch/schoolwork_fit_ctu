//
// Created by dxve on 17.5.23.
//

#ifndef SEMESTRAL_CINPUTHANDLER_H
#define SEMESTRAL_CINPUTHANDLER_H

#include "CPlayerStats.h"
#include "CGame.h"
#include "CAction.h"

#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>
#include <set>

#include "CInHandler.h"

/**
 *  @brief Provides menus and handles game control for human players.
 */
class CPlayerInputHandler: public CInHandler{

    /**
     * Path to directory with files to load new games.
     */
    static std::string new_game_directory;
    /**
     * Path to directory with saved games.
     */
    static std::string save_file_directory;

public:

    /**
     * @brief Constructor.
     */
    explicit CPlayerInputHandler(CGame * game);

    /**
     * Main method, handles user input and calls other methods.
     */
    void getInput() override;

    /**
     * Menu used to select file from which the game will be loaded.
     * Method is static because it is not connected to CGame instance.
     *
     * @param save_dir Path to directory with save files.
     * @return Path to selected save file, empty if none selected.
     */
    static std::string saveFilesMenu(const std::string & save_dir);

    /**
     * Start menu where player decides to start new/load game or close application.
     *
     * @return Path to save file from which tha game will be loaded. Empty if player chose to exit aplication.
     */
    static std::string startMenu();

    /**
     * Displays all building templates and handles players choice.
     */
    void buildMenu() const;

    /**
     * Displays all actions and handles players choice.
     */
    void actionMenu() const;

    /**
     * Displays all existing save files and saves game to file with given name.
     */
    void saveMenu() const;

    /**
     * FIle name can contain only alfanumeric symbols and '_' and has to be 2+ characters long
     */
    static bool validFileName(const std::string & file_name);

    /**
     * Changes corresponding static variables.
     *
     * @param new_games
     * @param saved_games
     */
    static void setSavePaths(std::string new_games, std::string saved_games);
};

#endif //SEMESTRAL_CINPUTHANDLER_H
