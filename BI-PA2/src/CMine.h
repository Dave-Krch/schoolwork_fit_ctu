//
// Created by dxve on 3.6.23.
//

#ifndef SEMESTRAL_CMINE_H
#define SEMESTRAL_CMINE_H

#include "CBuilding.h"

/**
 * @brief Class representing mine building.
 *
 * Class representing mine building, needs to be built on deposit,
 * each round consumes resources from deposit and produces resources for its owner.
 */
class CMine: public CBuilding{
private:
    /**
     * Name of teh resource on which the mine must be built.
     */
    std::string deposit;
    /**
     * Amount of resource left in deposit.
     */
    int deposit_amount;
    /**
     * Amount of deposited resource consumed each round.
     */
    int input_amount;
    /**
     * Map of resources and amounts produced each round.
     */
    std::map<std::string, int> output_resources;
public:
    /**
     * @brief Constructor.
     *
     * @param texture
     * @param name
     * @param deposit
     * @param deposit_amount
     * @param input_amount
     * @param build_costs
     * @param output
     */
    CMine(char texture,
          std::string name,
          std::string deposit,
          int deposit_amount,
          int input_amount,
          const std::map<std::string, int> & build_costs,
          const std::map<std::string, int> & output);

    /**
     * @brief Constructor from json.
     *
     * @param save_data
     * @param owner
     */
    explicit CMine(const nlohmann::json & save_data,
                   CPlayerStats * owner);

    /**
     * Adds output resources to owner if active.
     *
     * @param update_messages Mine cannot fail so it doesnt create update messages.
     */
    void update(std::map<std::string, std::vector<std::string>> & update_messages) override;

    /**
     * Creates copy of the building with given position and owner. Used to build buildings chosen by player from building templates.
     *
     * @param owner Player who built the mine.
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
     * Saves mine into json object.
     *
     * @param data Object for farm to be stored in.
     */
    void saveAsJson(nlohmann::json & data) const override;

    /**
     * Different from parrent method, mines can be built only on deposits.
     *
     * @param tile
     * @return
     */
    bool canBeBuilt(const std::pair<std::shared_ptr<CBuilding>, CDeposit> & tile) const override;

    /**
     * Multiplies mine production with given multiplier.
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

#endif //SEMESTRAL_CMINE_H
