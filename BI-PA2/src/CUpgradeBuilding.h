//
// Created by dxve on 4.6.23.
//

#ifndef SEMESTRAL_CUPGRADEBUILDING_H
#define SEMESTRAL_CUPGRADEBUILDING_H

#include "CAction.h"

/**
 * Child class action, upgrades (or downgrades) building selected by player.
 */
class CUpgradeBuildingAction: public CAction{
private:
    double multiplier;
public:
    /**
     * @brief Constructor from json.
     *
     * @throws std::runtime_error if any given field is not present in save file
     *
     * @param saved_data Json object storing data about action.
     * @param game Pointer to a game object in which actions will be executed.
     */
    CUpgradeBuildingAction(const nlohmann::json & saved_data, CGame * game);

    /**
     * Multiplies production rates of building selected by player on turn.
     *
     * @return True is upgrade was successful.
     */
    bool execute() const override;

    /**
     *
     * @return String with name and multiplier value.
     */
    std::string printInfo() const override;

    /**
     * Calculates value of upgrade.
     *
     * @return Value of action.
     */
    int calculateValue() override;

    /**
     * Finds best position on board to use action for ai.
     *
     * @return Pair (x, y).
     */
    std::pair<int, int> calculateBestPosition() override;
};

#endif //SEMESTRAL_CUPGRADEBUILDING_H
