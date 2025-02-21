#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h>

#define FILE_BUFFER_SIZE 8192
#define CLIENT_REQUEST_BUFFER_SIZE 4096

int main()
{
    // Open html file

    FILE *html_data = fopen("index.html", "r");

    // Define buffer for client request

    char client_request_buffer[CLIENT_REQUEST_BUFFER_SIZE];

    // Space to store client http request method, path and version

    char *client_method, *client_requested_path, *client_http_version;

    // Check if html_data is NULL

    if (html_data == NULL)
    {
        perror("Error opening index.html");
        exit(EXIT_FAILURE);
    }

    // Construct HTTP response

    char response_data[FILE_BUFFER_SIZE];
    memset(response_data, 0, sizeof(response_data));

    // Read contents of index.html with fread

    size_t bytes_read = fread(response_data, 1, sizeof(response_data) - 1, html_data);
    fclose(html_data);

    // Construct HTTP header

    char http_header[FILE_BUFFER_SIZE + 128];
    snprintf(http_header, sizeof(http_header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n\r\n%s",
             bytes_read, response_data);

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

    if ((bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address))) == -1)
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

        // Receive and print client http request

        int client_request = recv(client_socket, client_request_buffer, CLIENT_REQUEST_BUFFER_SIZE - 1, 0);

        // Check whether client http request was received successfully

        if (client_request < 0)
        {
            perror("recv failed");
            continue;
        }
        else if (client_request == 0)
        {
            printf("Client disconnected\n");
        }
        else
        {
            client_request_buffer[client_request] = '\0';
            printf("\nReceived HTTP request [FULL]:\n%s\n", client_request_buffer);
        }

        // Extract client request method, path and headers

        char *request_line = strtok(client_request_buffer, "\r\n");

        if (request_line)
        {

            // Get the client method

            client_method = strtok(request_line, " ");

            // Get the client requested path

            client_requested_path = strtok(NULL, " ");

            // Get the client request http version

            client_http_version = strtok(NULL, " ");
        }

        if (client_method && client_requested_path && client_http_version)
        {
            printf("----------Extracted Data----------\n");
            printf("Method: %s\n", client_method);
            printf("Path: %s\n", client_requested_path);
            printf("HTTP Version: %s\n", client_http_version);
            printf("----------------------------------\n");
        }

        // Send http header

        if ((send(client_socket, http_header, strlen(http_header), 0)) == -1)
        {
            perror("Error sending http header");
            continue;
        }

        close(client_socket);
        printf("Client Disconneted\n");
    }

    return 0;
}