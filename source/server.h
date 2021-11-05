#include <enet/enet.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#pragma once

class server {
    public:
        ENetAddress address;
        ENetHost *srver;
        ENetEvent event;
        int eventStatus;
        ENetPacket *packet;
    
        void start();
        void logic();
        void sendBlockArray(int*(blocks));
         server();
    
};