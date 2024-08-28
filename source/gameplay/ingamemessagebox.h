#pragma once
#include <string>

class ingamemessagebox
{
    public:
        double x = 0;
        double y = 0;
        bool goup = false;
        bool godown = false;
        bool active = false;
        double uptime = 0;
        void logic(double deltatime);
        void activate(std::string title, std::string desc);
        std::string name;
        std::string content;
        void render();
        ingamemessagebox(std::string name, std::string desc, int loc);
        ingamemessagebox();

    private:

    };
