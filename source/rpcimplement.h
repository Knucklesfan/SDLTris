#pragma once
#ifdef _DISCORD
#include "discord/discord.h"
#endif
#include <string>
#include <iostream>
#include <chrono>

class rpcimplement {
public:
#ifdef _DISCORD
    discord::Result result;
    discord::Core* core{};
#endif
    rpcimplement();
    void update(std::string state, std::string score, std::string icon, int time);
    void logic();
};
