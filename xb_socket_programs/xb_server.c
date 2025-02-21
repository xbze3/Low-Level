#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main()
{
    char server_response[] = "You have reached the server";

    // Create server socket

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Check if server socket was created successfully

    if (server_socket == -1)
    {
        perror("Error while creating server socket");
        exit(EXIT_FAILURE);
    }

    // Define server address

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9999);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to specified port

    int socket_bind = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    // Check if bind was successful

    if (socket_bind == -1)
    {
        perror("Error binding server to specified socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for requests

    if (listen(server_socket, 5) == -1)
    {
        perror("Error while listening");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port 9999...\n");

    // Accept client connection

    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1)
    {
        perror("Error accepting client connection");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Client connected!\n");

    // Send message to client

    if (send(client_socket, server_response, strlen(server_response), 0) == -1)
    {
        perror("Error sending message to client");
    }

    // Close client and server sockets

    close(client_socket);
    close(server_socket);

    return 0;
}
