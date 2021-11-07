#pragma once
#ifdef _WIN32
#include "discord/discord.h"

class rpcimplement {
public:
    discord::Core* core{};
    rpcimplement();
    void update(std::string state, std::string score, std::string icon, discord::Timestamp time);
    void logic();
};
#endif