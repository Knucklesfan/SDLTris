#pragma once
#include "discord/discord.h"
#include <string>
#include <iostream>
#include <chrono>

class rpcimplement {
public:
    discord::Result result;
    discord::Core* core{};
    rpcimplement();
    void update(std::string state, std::string score, std::string icon, discord::Timestamp time);
    void logic();
};
