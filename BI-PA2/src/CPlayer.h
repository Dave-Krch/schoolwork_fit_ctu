//
// Created by dxve on 1.6.23.
//

#ifndef SEMESTRAL_CPLAYER_H
#define SEMESTRAL_CPLAYER_H

#include "CPlayerStats.h"
#include "CPlayerInputHandler.h"
#include "CInputHandlerAi.h"

#include "../libs/json.hpp"
#include "memory"

class CPLayer{
public:
    CPlayerStats stats;
    std::shared_ptr<CInHandler> input_handler;

    CPLayer(const nlohmann::json & saved_player, CGame * game);

    CPLayer(int x, int y, std::string name, const std::map<std::string, int> & resources, const std::map<std::string, int> & income);
};

#endif //SEMESTRAL_CPLAYER_H
