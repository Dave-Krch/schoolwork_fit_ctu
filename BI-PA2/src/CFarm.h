//
// Created by dxve on 28.5.23.
//

#ifndef SEMESTRAL_CFARM_H
#define SEMESTRAL_CFARM_H

#include "CBuilding.h"

/**
 * @brief Class representing farm building.
 *
 * FFarms produce given amount of one resource for their owner each round.
 */
class CFarm : public CBuilding{
private:
    /**
     * Name of the output resource.
     */
    std::string output_resource;

    /**
     * Amount produced every turn.
     */
    int         output_amount;
public:
    /**
     * @brief Constructor.
     *
     * Constructs object without set owner and position.
     *
     * @param texture Char representing farm on the board.
     * @param name Name of the building type, not the specific object.
     * @param build_costs Resources needed to build.
     * @param resource Produced resource.
     * @param amount Produced amount.
     */
    CFarm(char texture,
          std::string name,
          const std::map<std::string, int> & build_costs,
          std::string resource,
          int amount);

    /**
     * @brief Constructor from json.
     *
     * Loads and constructs farm from json object.
     *
     * @param saved_data Json object in which the farm is stored.
     * @param owner Player who built the farm.
     */
    explicit CFarm(const nlohmann::json & saved_data,
                   CPlayerStats * owner);

    /**
     * Adds resource to owner if active.
     *
     * @param update_messages Farm cannot fail so it doesnt create update messages.
     */
    void update(std::map<std::string, std::vector<std::string>> & update_messages) override;

    /**
     * Creates copy of the building with given position and owner. Used to build buildings chosen by player from building templates.
     *
     * @param owner Player who built the farm.
     * @param x X position of selected tile.
     * @param y Y position of selected tile.
     * @param tile Deposit on selected tile (not used in building).
     * @return Returns shard pointer to the new building.
     */
    std::shared_ptr<CBuilding> buildCopy(CPlayerStats * owner, int x, int y, const std::pair<std::shared_ptr<CBuilding>, CDeposit> & tile) const override;

    /**
     * @return String with cost to build and info about production.
     */
    std::string printBuildInfo() const override;

    /**
     * Saves farm into json object.
     *
     * @param data Object for farm to be stored in.
     */
    void saveAsJson(nlohmann::json & data) const override;

    /**
     * Multiplies production rate of farm with given number.
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

#endif //SEMESTRAL_CFARM_H
