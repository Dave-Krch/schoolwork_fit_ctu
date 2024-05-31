//
// Created by dxve on 5.6.23.
//

#ifndef SEMESTRAL_CENDGAMESCENARIOS_H
#define SEMESTRAL_CENDGAMESCENARIOS_H

#include "CPlayerStats.h"
#include "CJsonCheck.h"

#include "../libs/json.hpp"

/**
 * @brief Class storing condition to win/loose the game and checking if they have been met.
 */
class CEndgameScenarios{
private:
    /**
     * Counts number of rounds played.
     */
    int round_counter;

    /**
     * Number of rounds after which will game end.
     */
    int max_rounds;

    /**
     * Resources and amounts needed by a player to win the game.
     */
    std::map<std::string, int> resource_win;

    /**
     * Player will loose when his resources drop under given amount.
     */
    std::map<std::string, int> resource_loss;

    /**
     * Map of all player in game.
     */
    std::map<std::string, CPlayerStats *> players;
public:
    /**
     * @brief Default constructor.
     *
     * Creates empty object.
     */
    CEndgameScenarios();

    /**
     * @brief Constructor from json.
     *
     * @param saved_conditions Json object storing end conditions.
     * @param players All players in game.
     */
    CEndgameScenarios(const nlohmann::json & saved_conditions, const std::map<std::string, CPlayerStats *> & players);

    /**
     * Called after every round. Checks all possible ends and prints winners.
     *
     * @return True if game ended.
     */
    bool checkForGameEnd();

    /**
     * Checks if max round number was reached. Then calculate who won.
     *
     * @return True if limit was reached.
     */
    bool checkMaxRoundEnd();

    /**
     * Checks if player met threshold for resource win.
     *
     * @return True if threshold met.
     */
    bool checkResourceWin();

    /**
     * Checks if player met threshold for resource loss.
     *
     * @return True if threshold met.
     */
    bool checkResourceLoss();

    /**
     * Calculates winners when game reached max rounds.
     */
     void calculateWinners();

     /**
      * Prints out winners.
      */
     static void printWinners(const std::vector<std::string> & winners);

     /**
      * Saves endgame scenarios into given json object.
      *
      * @param save_data object in which will be scenarios saved.
      */
     void saveAsJson(nlohmann::json & save_data) const;

     /**
      * Prints out information about end scenarios.
      */
     void printEndScenariosHelp() const;

     int getRoundCount() const;
     int getMaxRounds() const;
     const std::map<std::string, int> & getResourceWin() const;
     const std::map<std::string, int> & getResourceLoss() const;
};

#endif //SEMESTRAL_CENDGAMESCENARIOS_H
