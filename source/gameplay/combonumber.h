
// WACKY AND CRAAAAAAZY COMBO NUMBER!
// might have an epic pinball pixely thingy
// might not though, definitely will use that effect somewhere in the game tho
#include <string>
#define NUMBERALPHA 0.5
class combonumber {
    public:
        combonumber();
        void logic();
        void render();
        void splash(float value);
    private:
        float currentValue = 0.0;
        float animationTimer = 0.0;
        int length = 15;
        int animation = 0;
        bool active = false;
        std::string renderString = "";
};