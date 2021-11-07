#pragma once
#include "discord/discord.h"

class highscore {
public:
    int previousscore;
    int maxscore;
    highscore();
    void update(int score);
};