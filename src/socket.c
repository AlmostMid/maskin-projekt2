//
// Created by eren on 01/05/2024.
//
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

int client_socket;

// Function to update the buffer with a new message
void updateGUIMessage(const char *new_message) {
    // Ensure new_message fits within the buffer size
    char buffer[BUFFER_SIZE] = {0};
    strncpy(buffer, new_message, BUFFER_SIZE - 2);
    buffer[BUFFER_SIZE - 2] = '\0'; // Ensure null-termination
    // Send the message over the socket to the Java GUI
    send(client_socket, buffer, strlen(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
}

void *handle_connection(void *arg) {
    int client_socket = *((int *)arg); // Cast arg to int

    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received;

    // Initialize game state
    Linked_list *loadedDeck = NULL;
    bool deckLoaded = false;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        printf("\nReceived from client: %s\n", buffer);

        // Parse and handle client commands
        if (strncmp("LD", buffer, 2) == 0) {
            loadedDeck = LD(buffer + 3, 2); // Pass the argument to LD function
            if (loadedDeck != NULL) {
                deckLoaded = true;
                // Send confirmation message to client
                send(client_socket, "Deck loaded successfully.", strlen("Deck loaded successfully."), 0);
            } else {
                // Send error message to client
                send(client_socket, "Error loading deck.", strlen("Error loading deck."), 0);
            }
        } else if (deckLoaded && strncmp("P", buffer, 1) == 0) {
            bool gameResult = playPhase(&loadedDeck); // Pass the loaded deck to playPhase function
            if (!gameResult) {
                // Send game over message to client
                send(client_socket, "Game over. You win!", strlen("Game over. You win!"), 0);
            }
        } else {
            // Send invalid command message to client
            send(client_socket, "Invalid command.", strlen("Invalid command."), 0);
        }

        memset(buffer, 0, sizeof(buffer)); // Clear buffer for next iteration
    }

    if (bytes_received == 0) {
        printf("Client disconnected\n");
    } else if (bytes_received == -1) {
        perror("recv");
    }

    close(client_socket);
    return NULL;
}


void start_socket() {
    pthread_t thread_id;
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

#ifdef _WIN32
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        exit(EXIT_FAILURE);
    }
#endif

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
#ifdef _WIN32
        fprintf(stderr, "Socket creation failed with error code: %d\n", WSAGetLastError());
#else
        perror("Socket creation failed");
#endif
        exit(EXIT_FAILURE);
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen on all interfaces
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_PENDING_CONNECTIONS) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept incoming connection
        if ((client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_len)) == -1) {
            perror("Accept failed");
            continue;
        }

        // Create thread to handle connection
        if (pthread_create(&thread_id, NULL, handle_connection, (void *) &client_socket) != 0) {
            perror("Thread creation failed");
            close(client_socket);
            continue;
        }

        // Detach thread to allow it to run independently
        pthread_detach(thread_id);
    }
}
