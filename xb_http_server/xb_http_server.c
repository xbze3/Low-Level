#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h>

int main()
{
    // Open html file

    FILE *html_data = fopen("index.html", "r");

    // Construct HTTP response

    char response_data[1024];
    fgets(response_data, 1024, html_data);

    // Construct HTTP header

    char http_header[2048] = "HTTP/1.1 200 OK\r\n\n";
    strcat(http_header, response_data);

    // Create Socket

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Check if socket was created successfully

    if (server_socket == -1)
    {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    // Define Socket Address

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8001);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind socket and check

    if ((bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)))-1)
    {
        perror("Error encountered while binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for client requests

    if (listen(server_socket, 5) == -1)
    {
        perror("Error encountered while listening");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port 8001...\n");

    while (1)
    {

        // Accept client requests

        int client_socket = accept(server_socket, NULL, NULL);

        // Check if client request was accpeted successfully

        if (client_socket == -1)
        {
            perror("Error accepting client connection");
            continue;
        }

        // Send http header

        if ((send(client_socket, http_header, sizeof(http_header), 0)) == -1)
        {
            perror("Error sending http header");
            continue;
        }

        close(client_socket);
        printf("Client Disconneted\n");
    }

    return 0;
}