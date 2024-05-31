//
// Created by dxve on 15.5.23.
//

#ifndef SEMESTRAL_CPLAYERSTATS_H
#define SEMESTRAL_CPLAYERSTATS_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <ostream>

#include "memory"

/**
 * @brief Class storing and handling information about player.
 */
class CPlayerStats{
private:
    /**
     * Name of the player.
     */
    std::string name;
    /**
     * Current amount of resources player has.
     */
    std::map<std::string, int> resources;
    /**
     * Amount of resources player gets after every turn.
     */
    std::map<std::string, int> income;
public:
    int selected_x, selected_y;

    CPlayerStats();
    CPlayerStats(int x, int y, std::string name, const std::map<std::string, int> & resources, const std::map<std::string, int> & income);

    /**
     * Sets players selected position to given x and y.
     *
     * @param x
     * @param y
     */
    void setSelectedPosition(int x, int y);

    void addResource(const std::string& resource, int amount);
    /**
     * Removes amount of given material from players resources.
     *
     * @param resources_to_remove
     * @return True if player can afford removal (resource amount can not get under 0).
     */
    bool removeResources(const std::map<std::string, int> & resources_to_remove);
    /**
     *
     * @param costs Map of resources and amounts.
     * @return True is player has at least given amount of resources.
     */
    bool canAfford(const std::map<std::string, int> & costs) const;

    /**
     * @brief Operator << overload.
     *
     * Returns player name and his resources.
     *
     * @param oss
     * @param player
     * @return
     */
    friend std::ostream & operator << (std::ostream & oss, const CPlayerStats & player);

    /**
     *
     * @return Const reference to palyers name.
     */
    const std::string & getName() const;

    /**
     *
     * @return Const reference to players resources.
     */
    const std::map<std::string, int> & getResources() const;

    /**
     *
     * @return Sum af all resources.
     */
    int sumResources() const;

    /**
     * Adds given amounts o resource sto income.
     *
     * @param resources_to_add
     */
    void addToIncome(const std::map<std::string, int> & resources_to_add);

    /**
     * Removes given amounts o resource sto income.
     *
     * @param resources_to_remove
     */
    void removeFromIncome(const std::map<std::string, int> & resources_to_remove);

    const std::map<std::string, int> & getIncome() const;
};

#endif //SEMESTRAL_CPLAYERSTATS_H
