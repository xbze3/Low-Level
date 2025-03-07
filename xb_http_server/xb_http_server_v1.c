#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h>

#define FILE_BUFFER_SIZE 8192
#define CLIENT_REQUEST_BUFFER_SIZE 4096

int server_socket;

// Handle exit gracefully

void handle_sigint(int sig)
{
    printf("\nCaught signal %d (Ctrl+C). Closing server socket...\n", sig);
    close(server_socket);
    exit(0);
}

char *get_content_type(char *content_ext)
{
    if (strcmp(content_ext, ".html") == 0)
    {
        return "text/html";
    }
    else if (strcmp(content_ext, ".css") == 0)
    {
        return "text/css";
    }
    else if (strcmp(content_ext, ".js") == 0)
    {
        return "application/javascript";
    }
    else if (strcmp(content_ext, ".json") == 0)
    {
        return "application/json";
    }
    else if (strcmp(content_ext, ".png") == 0)
    {
        return "image/png";
    }
    else if (strcmp(content_ext, ".jpg") == 0 || strcmp(content_ext, ".jpeg") == 0)
    {
        return "image/jpeg";
    }
    else if (strcmp(content_ext, ".gif") == 0)
    {
        return "image/gif";
    }
    else if (strcmp(content_ext, ".pdf") == 0)
    {
        return "application/pdf";
    }
    else
    {
        return "application/octet-stream";
    }
}

int main()
{
    // Create buffer to store file path

    char file_path[256] = ".";

    // Open html file

    FILE *default_path = fopen("./www/index.html", "r");

    // Define buffer for client request

    char client_request_buffer[CLIENT_REQUEST_BUFFER_SIZE];

    // Space to store client http request method, path and version

    char *client_method, *client_requested_path, *client_http_version, *read_type;

    // Register sigint handler

    signal(SIGINT, handle_sigint);

    // Check if default_path is NULL

    if (default_path == NULL)
    {
        perror("Error opening index.html");
        exit(EXIT_FAILURE);
    }

    // Construct HTTP response

    char response_data[FILE_BUFFER_SIZE];
    memset(response_data, 0, sizeof(response_data));

    // Read contents of index.html with fread

    size_t bytes_read = fread(response_data, 1, sizeof(response_data) - 1, default_path);
    fclose(default_path);

    // Construct HTTP header

    char http_header[FILE_BUFFER_SIZE + 128];
    snprintf(http_header, sizeof(http_header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n\r\n%s",
             bytes_read, response_data);

    // Create Socket

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Check if socket was created successfully

    if (server_socket == -1)
    {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    // Set SO_REUSEADDR to allow immediate reuse of the port

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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

            // Uncomment line below to see full client http request
            // printf("\nReceived HTTP request [FULL]:\n%s\n", client_request_buffer);
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

            if (strcmp(client_requested_path, "/") == 0)
            {
                char modified_path[256];
                strcpy(modified_path, "/www/index.html");

                client_requested_path = modified_path;
            }

            printf("%s : ", client_method);
            printf("[ %s ]", client_requested_path);
            printf(" %s", client_http_version);

            strcpy(file_path, ".");
            strncat(file_path, client_requested_path, sizeof(file_path) - strlen(file_path) - 1);

            // Get the content type given its entension

            if (strcmp(strrchr(client_requested_path, '.'), ".png") == 0 ||
                strcmp(strrchr(client_requested_path, '.'), ".jpeg") == 0 ||
                strcmp(strrchr(client_requested_path, '.'), ".gif") == 0 ||
                strcmp(strrchr(client_requested_path, '.'), ".pdf") == 0)
            {
                read_type = "rb";
            }
            else
            {
                read_type = "r";
            }

            FILE *file = fopen(file_path, read_type);

            if (file)
            {

                // Read the requested file and construct and http header with its contents

                snprintf(http_header, sizeof(http_header),
                         "HTTP/1.1 200 OK\r\n"
                         "Content-Type: %s\r\n"
                         "Content-Length: %zu\r\n"
                         "Connection: close\r\n\r\n",
                         get_content_type(strrchr(client_requested_path, '.')), bytes_read);

                printf(" [ 200 ]\n");

                bytes_read = fread(response_data, 1, sizeof(response_data) - 1, file);
                fclose(file);

                if ((send(client_socket, http_header, strlen(http_header), 0)) == -1)
                {
                    perror("Error sending http header");
                    continue;
                }

                if (send(client_socket, response_data, bytes_read, 0) == -1)
                {
                    perror("Error sending file data");
                    continue;
                }
            }
            else
            {
                // Return 404 response if the file path could not be found

                snprintf(http_header, sizeof(http_header),
                         "HTTP/1.1 404 Not Found\r\n"
                         "Content-Type: text/plain\r\n"
                         "Content-Length: 13\r\n"
                         "Connection: close\r\n\r\n"
                         "404 Not Found");

                printf(" [ 404 ]\n");

                if ((send(client_socket, http_header, strlen(http_header), 0)) == -1)
                {
                    perror("Error sending http header");
                    continue;
                }
            }

            close(client_socket);

            // Uncomment line below to see when client disconnects
            // printf("Client Disconneted\n");
        }
    }
    return 0;
}