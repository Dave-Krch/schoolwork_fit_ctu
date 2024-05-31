//
// Created by dxve on 4.6.23.
//

#ifndef SEMESTRAL_CACTION_H
#define SEMESTRAL_CACTION_H

#include "CPlayerStats.h"
#include "CBuilding.h"
#include "CGame.h"
#include "CJsonCheck.h"

/**
 * @brief Parent class for all action types.
 */
class CAction{
protected:
    /**
     * Pointer to a game object in which actions will be executed.
     */
    CGame * game;
    /**
     * Name to identify a specific action.
     */
    std::string name;
    /**
     * Stores cost of action to execute.
     */
    std::map <std::string, int> execute_cost;
public:

    static std::map<std::string, std::shared_ptr<CAction>> loaded_actions;

    /**
     * @brief Constructor from json.
     *
     * @throws std::runtime_error if any given field is not present in save file
     *
     * @param saved_data Json object storing data about action.
     * @param game Pointer to a game object in which actions will be executed.
     */
    CAction(const nlohmann::json & saved_data, CGame * game);

    /**
     * @brief Constructor.
     *
     * @param game
     */
    explicit  CAction(CGame * game, std::string name);

    /**
     * Checks if executor has enough resources. Used in child overrides of execute.
     *
     * @return True if action has been executed.
     */
    virtual bool execute() const;

    /**
     * @return String with information about the action.
     */
    virtual std::string printInfo() const;

    /**
     * Calculates value of action for ai.
     *
     * @return Value of action.
     */
    virtual int calculateValue();

    /**
     * Finds best position on board to use action for ai.
     *
     * @return Pair (x, y).
     */
    virtual std::pair<int, int> calculateBestPosition();

    const std::map <std::string, int> & getExecuteCost();
};

#endif //SEMESTRAL_CACTION_H
