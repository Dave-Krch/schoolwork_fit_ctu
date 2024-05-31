//
// Created by dxve on 7.6.23.
//

#ifndef SEMESTRAL_CCATACLYSMICEVENT_H
#define SEMESTRAL_CCATACLYSMICEVENT_H

#include "CAction.h"
#include "CDeposit.h"

/**
 * Destroys building and creates deposit on one tile.
 */
class CCataclysmicEvent: public CAction {
private:
    /**
     * Represents resource of which deposit will be created.
     */
    std::string deposit_resource;

    /**
     * How much will be deposited.
     */
    int deposit_amount;

    /**
     * Texture of the deposit.
     */
     char texture;
public:
    /**
    * @brief Constructor from json.
    *
    * @throws std::runtime_error if any given field is not present in save file
    *
    * @param saved_data Json object storing data about action.
    * @param game Pointer to a game object in which actions will be executed.
    */
    CCataclysmicEvent(const nlohmann::json & saved_data, CGame * game);

    /**
     * Destroys building on selected tile and creates deposit.
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

#endif //SEMESTRAL_CCATACLYSMICEVENT_H
