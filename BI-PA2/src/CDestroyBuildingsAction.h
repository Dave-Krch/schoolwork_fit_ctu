//
// Created by dxve on 5.6.23.
//

#ifndef SEMESTRAL_CDESTROYBUILDINGSACTION_H
#define SEMESTRAL_CDESTROYBUILDINGSACTION_H

#include "CAction.h"

/**
 * @brief Destroys buildings in radius if executed.
 */
class CDestroyBuildingsAction: public CAction{
private:
    /**
     * For check if action can affect buildings from different player than player who executes it.
     */
    bool can_destroy_enemy;
    /**
     * Radius of destruction.
     */
    int radius;
public:
    /**
    * @brief Constructor from json.
    *
    * @throws std::runtime_error if any given field is not present in save file
    *
    * @param saved_data Json object storing data about action.
    * @param game Pointer to a game object in which actions will be executed.
    */
    CDestroyBuildingsAction(const nlohmann::json & saved_data, CGame * game);

    /**
     * Destroys buildings in given radius.
     *
     * @return True if destruction was successful.
     */
    bool execute() const override;

    /**
     *
     * @return String with information about action.
     */
    std::string printInfo() const override;

    /**
     * Calculates value of cat. event.
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

#endif //SEMESTRAL_CDESTROYBUILDINGSACTION_H
