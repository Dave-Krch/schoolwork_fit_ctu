//
// Created by dxve on 3.6.23.
//

#ifndef SEMESTRAL_CFACTORY_H
#define SEMESTRAL_CFACTORY_H

#include "CBuilding.h"

/**
 * Child class of CBuilding,
 * each round consumes and produces given amount of resources for its owner.
 *
 * @brief Class representing factories.
 */
class CFactory: public CBuilding{
private:
    /**
     * Map of resources and amounts that the factory produces each round.
     */
    std::map<std::string, int> output_resources;
    /**
     *  Map of resources and amounts that the factory consumes each round.
     */
    std::map<std::string, int> input_resources;
public:
    /**
     * Constructs new factory without set owner and position
     *
     * @brief Constructor.
     *
     * @param texture Char representing the factory on the board.
     * @param name Name of the building type, not of the specific building.
     * @param build_costs Resources needed to build.
     * @param output_resources Produced resources.
     * @param input_resources Consumed resources.
     */
    CFactory(char texture,
             std::string name,
             const std::map<std::string, int> & build_costs,
             const std::map<std::string, int> & output_resources,
             const std::map<std::string, int> & input_resources);

    /**
     * Loads factory fromm given json object.
     *
     * @brief Constructor fromm json.
     *
     * @param save_data Json object storing information about the factory.
     * @param owner Player who built the factory.
     */
    explicit CFactory(const nlohmann::json & save_data, CPlayerStats * owner);


    /**
     * Factory adds resources defined in output_resources and takes resources defined in input_resources from player each round.
     *
     * @param update_messages Factory creates error message if its owner doesnt have enough resources for its input.
     */
    void update(std::map<std::string, std::vector<std::string>> & update_messages) override;

    /**
     * Creates copy of the building with given position and owner. Used to build buildings chosen by player from building templates.
     *
     * @param owner Player who built the factory.
     * @param x X position of selected tile.
     * @param y Y position of selected tile.
     * @param deposit Deposit on selected tile.
     * @return Returns shard pointer to the new building.
     */
    std::shared_ptr<CBuilding> buildCopy(CPlayerStats * owner, int x, int y, const std::pair<std::shared_ptr<CBuilding>, CDeposit> & tile) const override;

    /**
     * @return String with build costs and input/output specification.
     */
    std::string printBuildInfo() const override;

    /**
     * Saves factory to json object.
     *
     * @param data Object for factory to be stored in.
     */
    void saveAsJson(nlohmann::json & data) const override;

    /**
     * Multiplies production rate of factory with given number.
     *
     * @param multiplier
     */
    void upgrade(double multiplier) override;

    /**
     * @brief Calculates value of the building for ai.
     *
     * @param player
     * @param end_scenarios
     * @return
     */
    int calculateValue(const CPlayerStats * player, const CEndgameScenarios * end_scenarios ) const override;

    /**
     * Adds resources to player income.
     */
    void addOwnerIncome() override;

    /**
     * Removes resources from player income.
     */
    void takeOwnerIncome() override;
};

#endif //SEMESTRAL_CFACTORY_H
