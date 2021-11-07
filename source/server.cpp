#ifdef _NETCODE
#include "server.h"
#include <enet/enet.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

server::server() {
        // a. Initialize enet
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occured while initializing ENet.\n");
        exit;
    }
    atexit(enet_deinitialize);

    address.host = ENET_HOST_ANY;
    address.port = 1234;
}
void server::start() {
        srver = enet_host_create(&address, 32, 2, 0, 0);

    if (srver == NULL) {
        fprintf(stderr, "An error occured while trying to create an ENet server host\n");
        exit(EXIT_FAILURE);
    }

    eventStatus = 1;

}
void server::logic() {
        eventStatus = enet_host_service(srver, &event, 10);

        // If we had some event that interested us
        if (eventStatus > 0) {
            switch(event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    printf("(Server) We got a new connection from %x\n",
                            event.peer->address.host);
                    break;
                }

                case ENET_EVENT_TYPE_RECEIVE: {
                    printf("(Server) Message from client : %s\n", event.packet->data);
                    // Lets broadcast this message to all
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT: {
                    printf("%s disconnected.\n", event.peer->data);

                    // Reset client's information
                    event.peer->data = NULL;
                    break;
                }

            }
        }

}
void server::sendBlockArray(int*(blocks)) {
    packet = enet_packet_create(blocks, sizeof(blocks), ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(srver,1, packet);

}
#endif