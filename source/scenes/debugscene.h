#pragma once
#include "../gamemode.h"
#include "../utils/defs.h"
#define MENUSIZE 7
class debugscene : public Gamemode { //literally just renders a white screen to test transitions
    public:
        void render();
        void input(SDL_Keycode keysym);
        Transition endLogic();
        void logic(double deltatime);
        void reset();
        debugscene();
    private:
        int r = 128 + rand() % (256 - 128);
        int g = 128 + rand() % (256 - 128);
        int b = 128 + rand() % (256 - 128);
        bool advance = false;
        int transition = 0;
        int background = 0;
        int selection = 0;
        int gamemode = 0;
        int currentmessage = 0;
        int messagelife = 0;
        float starX[255];
        float starY[255];
        float starZ[255];
        float starBlock[255];

        int* endings[MENUSIZE] = {
            &background,
            &transition,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        };
        std::string menu[MENUSIZE] = {
            "Current Background #",
            "Transition to Transition #",
            "Goto Gamemode #",
            "Hide this menu",
            "Load game normally",
            "Toggle Framerate",
            "Close game"
        };
        std::string kekcrocMessages[42] = {
            "What are you, a filthy cheater?",
            "Cmon man... Just pick something...",
            "We all don't have all day dude...",
            "Hey Kekcroc, what else do you have to do?",
            "Uhhh, I have no idea, but definitely don't have time to\nwait around for some idiot to make their decision",
            "Just hurry up and make your decision dude",
            "Look, all the backgrounds are the same,\ntheres nothing to see here.",
            "What, are you waiting for the blocks to stop flying??\nIt's infinite, there's no ending.",
            "Are you trying to find a memory leak or something? Cuz lemme tell you,\nthis is the best code in the world,\ntheres no WAY it could leak!",
            "Hey, isnt that what they said about the titanic?",
            "Yeah, but they said a lot of things about that boat.\nReally, you decide to take a boat not rated for\nicy water into icy water. Genius.",
            "I'm pretty sure it was rated for icy water...",
            "I DON'T CARE. WHO CARES IF ITS RATED FOR ICY WATER?!?",
            "Well, the people that died in it, for one...",
            "I swear to god, those people that died in the\ntitanic are stupid too, why not just swim away?",
            "Kekcroc, are you an idiot?\nthat was 12 degree water. I'd like to see you jump \n6 stories and then swim to shore.",
            "I would survive, and i would THRIVE.\nNo water's ever bothered me.",
            "Kekcroc, just to ask, do you have a Facebook account?",
            "No, why?",
            "You sound like a facebook account.",
            "HOW OLD DO YOU THINK I AM?",
            "Uhhhhhh, geez i dunno, probably at least 50?\nJudging by how you speak, at least...",
            "Mario, I'm 22.",
            "WHAT?!",
            "Yeah, I always have been too.\nIt's kinda that thing with game characters.",
            "Oh shut up about that, if we get too meta\nwe'll have someone online call us out for being cringe",
            "I'd rather die cringe than live normal",
            "I think thats what they said on the titanic actually",
            "SEE? So you agree with me that the titanic is cringe!",
            "No, I disagree with you that the people that died in the titanic\nwere not cringe, but the idea itself was cringe.",
            "Look, to change the subject, how many people you actually\nthink are gonna read these messages?",
            "I give it a lowball 10,\nmaybe even less but that depends if someone\nactually plays this game",
            "Yknow, you would think we'd have better marketing",
            "Yeah, someone should really complain about that\nto the higherups, huh",
            "Look, even if one person reads this message,\nthen that's a success in my book.",
            "Why would just one person read any of this?",
            "Well, maybe it goes unused, maybe the debug menu gets removed from\nthe game before launch",
            "Doubtful, remember the first launch?\nThey kept the ASAN code in the game for no reason!",
            "Yeah, we should be glad we aren't a nintendo game, people would datamine that to bits",
            "Yknow, it'd be really funny if someone does a decomp of the game,\nonly for us to release the source code a day after they finish.",
            "Yeah, but that would require us to close the\nsource now anyways... Which we won't do yet.",
            "Oh well. Anyways, pick something I'm done talking."
        };
        int messageSenders[42] = {
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            1,
        };
        std::string transitions[4] = {
            "FADE",
            "BARS",
            "BLOCKS",
            "CIRCLE"
        };

};