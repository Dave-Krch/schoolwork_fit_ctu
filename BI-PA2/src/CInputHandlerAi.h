//
// Created by dxve on 6.6.23.
//

#ifndef SEMESTRAL_CINPUTHANDLERAI_H
#define SEMESTRAL_CINPUTHANDLERAI_H

#include "CInHandler.h"

/**
 * @brief Class managing ai logic.
 */
class CInputHandlerAi: public CInHandler {
private:

    /**
     * Counts how many move has the ai done.
     */
    int move_counter;

    /**
     * Max number of moves before ending game.
     */
    int max_moves;
public:
    /**
     *  @brief Constructor.
     *
     * @param game
     */
    explicit CInputHandlerAi(CGame * game, int max_moves);

    /**
     * Calculates value of every building player can afford and sort them by values.
     *
     */
    void calculateBuildingValues(std::vector<std::pair<int, std::string>> & sorted_buildings) const;

    /**
     * Calculates value of every action player can afford and sort them by values.
     * Finds best tile for action to be used on.
     */
    void calculateActionValues(std::vector<std::pair<int, std::string>> & sorted_actions, std::map<std::string, std::pair<int, int>> & best_tiles_for_action) const;

    /**
     * @brief Main decision making method.
     *
     * Ai calculates value of every building/action and then executes the best.
     *
     */
    void getInput() override;

    /**
     * @brief Finds empty tile and builds selected building there.
     *
     * @param building_name Selected building template.
     * @return True if building was built successfully.
     */
    bool tryToBuild(const std::string & building_name) const;

    /**
     * Executes selected action.
     *
     * @param action_name
     */
    void executeAction(const std::string & action_name, std::pair<int, int> position) const;

};

#endif //SEMESTRAL_CINPUTHANDLERAI_H
