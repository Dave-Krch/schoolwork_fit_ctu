//
// Created by dxve on 15.5.23.
//

#ifndef SEMESTRAL_CBUILDING_H
#define SEMESTRAL_CBUILDING_H

#include "CPlayerStats.h"
#include "CDeposit.h"
#include "CEndgameScenarios.h"
#include "CJsonCheck.h"

#include <iostream>
#include <sstream>
#include <utility>

#include "../libs/json.hpp"

/**
 * @brief Abstract class for buildings, stores basic building data and handles actions with building.
 */
class CBuilding{
protected:
    /**
     * Char representing the building on the game board.
     */
    char texture;
    /**
     * Name used to identify building type.
     */
    std::string name;
    /**
     * Pointer to the data of player who built the building.
     */
    CPlayerStats * owner;
    /**
     * Resources and amounts of them required to build the building.
     */
    std::map<std::string, int> build_costs;
    /**
     * If false building exits its update method at the start.
     */
     bool active;

public:
    /**
     * Variable used by ai to know if it already upgraded the building.
     */
    bool upgraded;
    /**
     * Position of the building on the game board.
     */
    int x_pos, y_pos;

    /**
     * Constructor, creates building without set owner and position.
     *
     * @brief Constructor.
     *
     * @param texture Char representing the building on board.
     * @param name String for initializing name member.
     * @param build_costs Map<string, int> for initializing build_costs member.
     */
    CBuilding(char texture, std::string name, const std::map<std::string, int> & build_costs);
    /**
     * Constructor, creates building from saved json object.
     *
     * @brief Constructor from json.
     *
     * @param saved_data building saved as json object
     * @param owner pointer to a player that build the building
     */
    explicit CBuilding(const nlohmann::json & saved_data, CPlayerStats * owner);

    /**
     * Depends on the child object, typically adds resources to owner.
     */
    virtual void update(std::map<std::string, std::vector<std::string>> & update_messages) = 0;
    /**
     * @brief Creates copy of the building from building templates stored in game.
     *
     * @param owner Pointer to a player that built the building.
     * @param x
     * @param y
     * @return Returns shared_ptr to the created building.
     */
    virtual std::shared_ptr<CBuilding> buildCopy(CPlayerStats * owner, int x, int y, const std::pair<std::shared_ptr<CBuilding>, CDeposit> & tile) const = 0;

    /**
     * @return String with info about the building, including building costs and production info.
     */
    virtual std::string printBuildInfo() const = 0;

    /**
     * Stores the building as json object.
     *
     * @param data Reference to a json object in which the building will be stored.
     */
    virtual void saveAsJson(nlohmann::json & data) const;

    /**
     * Method used to find out if we can build the building on given tile.
     *
     * @param tile Reference to the selected tile.
     * @return True if the building can be built there.
     */
    virtual bool canBeBuilt(const std::pair<std::shared_ptr<CBuilding>, CDeposit> & tile) const;

    /**
     * Multiplies building production by given multiplier.
     *
     * @param multiplier
     */
    virtual void upgrade(double multiplier) = 0;

    void activate();
    void deActivate();

    void setOwner(CPlayerStats * new_owner);
    void setPos(int x, int y);
    char getTexture() const;
    const CPlayerStats * getOwner() const;
    /**
     * @return Pointer to the map of building costs.
     */
    const std::map<std::string, int> & getBuildCosts() const;

    /**
     * Adds resources to player income.
     */
    virtual void addOwnerIncome() = 0;

    /**
     * Removes resources from player income.
     */
    virtual void takeOwnerIncome() = 0;

    /**
     * @brief Method used by ait to decide what to build.
     *
     * Calculates value of the building for given player with given end scenarios.
     * Parent class method calculates multiplier from building cost and player income, this is same for all buildings.
     *
     * @param player
     * @param end_scenarios
     * @return
     */
    virtual int calculateValue(const CPlayerStats * player, const CEndgameScenarios * end_scenarios ) const;


};

#endif //SEMESTRAL_CBUILDING_H
