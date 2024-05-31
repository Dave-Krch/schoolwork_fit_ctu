//
// Created by dxve on 5.6.23.
//

#ifndef SEMESTRAL_CDEACTIVATEBUILDINGACTION_H
#define SEMESTRAL_CDEACTIVATEBUILDINGACTION_H

#include "CAction.h"

/**
 * @brief Action that activates building selected by a player on move.
 */
class CDeActivateBuildingAction: public CAction {
public:
    /**
     * @brief Constructor.
     *
     * @param game Pointer to a game object on which will be action executed.
     */
    explicit CDeActivateBuildingAction(CGame * game);
    /**
     * Activates building selected by a player if its his own.
     *
     * @return True is activation was successful.
     */
    bool execute() const override;

    /**
     *
     * @return Simple description.
     */
    std::string printInfo() const override;
};

#endif //SEMESTRAL_CDEACTIVATEBUILDINGACTION_H
