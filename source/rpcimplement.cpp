#ifdef _WIN32
#include "rpcimplement.h"
#include "discord/discord.h"

#include <iostream>
#include <chrono>


rpcimplement::rpcimplement()
{
    result = discord::Core::Create(906928395521253396, 1, &core);
    if (result == discord::Result::Ok) {
        discord::Activity activity{};
        activity.SetState("In the menu.");
        activity.SetDetails("");

        activity.GetAssets().SetLargeImage("mainicon");
        activity.SetType(discord::ActivityType::Playing);
        activity.GetTimestamps().SetStart(time(nullptr));

        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        });
    }
}
void rpcimplement::logic() {
    if (result == discord::Result::Ok) {
        core->RunCallbacks();
    }
}

void rpcimplement::update(std::string state, std::string score, std::string icon, discord::Timestamp time) {
    if (result == discord::Result::Ok) {
        discord::Activity activity{};
        activity.SetState(state.c_str());
        activity.SetDetails(score.c_str());
        activity.GetAssets().SetLargeImage(icon.c_str());
        activity.SetType(discord::ActivityType::Playing);
        activity.GetTimestamps().SetStart(time);

        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        });
    }

}
#endif