#include <enet/enet.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
int main(int argc, char **argv)
{
    ENetAddress address;
    ENetHost *client;
    ENetPeer *peer;
    char message[1024];
    ENetEvent event;
    int eventStatus;

    // a. Initialize enet
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occured while initializing ENet.\n");
        return EXIT_FAILURE;
        
    }

    atexit(enet_deinitialize);

    // b. Create a host using enet_host_create
    client = enet_host_create(NULL, 1, 2, 57600/8, 14400/8);

    if (client == NULL) {
        fprintf(stderr, "An error occured while trying to create an ENet server host\n");
        exit(EXIT_FAILURE);
    }

    enet_address_set_host(&address, "localhost");
    address.port = 1234;

    // c. Connect and user service
    peer = enet_host_connect(client, &address, 2, 0);

    if (peer == NULL) {
        fprintf(stderr, "No available peers for initializing an ENet connection");
        exit(EXIT_FAILURE);
    }

    eventStatus = 1;

    while (1) {
        eventStatus = enet_host_service(client, &event, 1000);

        // If we had some event that interested us
        if (eventStatus > 0) {
            switch(event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    break;

                case ENET_EVENT_TYPE_RECEIVE: {
                    int* blocks = (int*) event.packet->data;
                    std::cout << "============\n";
                    for(int i = 0; i < 200; i++) {
                        std::cout << blocks[i];
                        if(i % 10 == 0 && i > 0) {
                            std::cout << "\n";
                        }
                    }
                    enet_packet_destroy(event.packet);
                    break;
                }
            }
        }
    }
}
