#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "controller.h"
#include "Linkedlists.h"
#include "gameCommands.h"

#define PORT 8000
#define MAX_PENDING_CONNECTIONS 5
#define BUFFER_SIZE 1024

// Function to update the buffer with a new message
void updateGUIMessage(const char *new_message);

// Function to handle connection with a client
void *handle_connection(void *arg);

// Function to start the socket server
void start_socket();

#endif // SOCKET_H
