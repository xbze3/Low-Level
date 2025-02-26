#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define FILE_BUFFER_SIZE 8192
#define CLIENT_REQUEST_BUFFER_SIZE 4096

int server_socket;

void handle_sigint(int sig)
{
    printf("\nCaught signal %d (Ctrl+C). Closing server socket...\n", sig);
    close(server_socket);
    exit(0);
}

char *get_content_type(char *content_ext)
{
    if (strcmp(content_ext, ".aac") == 0)
        return "audio/aac";

    else if (strcmp(content_ext, ".abw") == 0)
        return "application/x-abiword";

    else if (strcmp(content_ext, ".apng") == 0)
        return "image/apng";

    else if (strcmp(content_ext, ".arc") == 0)
        return "application/x-freearc";

    else if (strcmp(content_ext, ".avif") == 0)
        return "image/avif";

    else if (strcmp(content_ext, ".avi") == 0)
        return "video/x-msvideo";

    else if (strcmp(content_ext, ".azw") == 0)
        return "application/vnd.amazon.ebook";

    else if (strcmp(content_ext, ".bin") == 0)
        return "application/octet-stream";

    else if (strcmp(content_ext, ".bmp") == 0)
        return "image/bmp";

    else if (strcmp(content_ext, ".bz") == 0)
        return "application/x-bzip";

    else if (strcmp(content_ext, ".bz2") == 0)
        return "application/x-bzip2";

    else if (strcmp(content_ext, ".cda") == 0)
        return "application/x-cdf";

    else if (strcmp(content_ext, ".csh") == 0)
        return "application/x-csh";

    else if (strcmp(content_ext, ".css") == 0)
        return "text/css";

    else if (strcmp(content_ext, ".csv") == 0)
        return "text/csv";

    else if (strcmp(content_ext, ".doc") == 0)
        return "application/msword";

    else if (strcmp(content_ext, ".docx") == 0)
        return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";

    else if (strcmp(content_ext, ".eot") == 0)
        return "application/vnd.ms-fontobject";

    else if (strcmp(content_ext, ".epub") == 0)
        return "application/epub+zip";

    else if (strcmp(content_ext, ".gz") == 0)
        return "application/gzip";

    else if (strcmp(content_ext, ".gif") == 0)
        return "image/gif";

    else if (strcmp(content_ext, ".htm") == 0 || strcmp(content_ext, ".html") == 0)
        return "text/html";

    else if (strcmp(content_ext, ".ico") == 0)
        return "image/vnd.microsoft.icon";

    else if (strcmp(content_ext, ".ics") == 0)
        return "text/calendar";

    else if (strcmp(content_ext, ".jar") == 0)
        return "application/java-archive";

    else if (strcmp(content_ext, ".jpeg") == 0 || strcmp(content_ext, ".jpg") == 0)
        return "image/jpeg";

    else if (strcmp(content_ext, ".js") == 0)
        return "text/javascript";

    else if (strcmp(content_ext, ".json") == 0)
        return "application/json";

    else if (strcmp(content_ext, ".jsonld") == 0)
        return "application/ld+json";

    else if (strcmp(content_ext, ".mid") == 0 || strcmp(content_ext, ".midi") == 0)
        return "audio/midi";

    else if (strcmp(content_ext, ".mp3") == 0)
        return "audio/mpeg";

    else if (strcmp(content_ext, ".mp4") == 0)
        return "video/mp4";

    else if (strcmp(content_ext, ".mpeg") == 0)
        return "video/mpeg";

    else if (strcmp(content_ext, ".png") == 0)
        return "image/png";

    else if (strcmp(content_ext, ".pdf") == 0)
        return "application/pdf";

    else if (strcmp(content_ext, ".zip") == 0)
        return "application/zip";

    else if (strcmp(content_ext, ".7z") == 0)
        return "application/x-7z-compressed";

    else
        return "application/octet-stream";
}

void handle_get_request(int client_socket, char *requested_path)
{
    char file_buffer[4096];
    char http_header[512];
    char file_path[256] = ".";
    size_t bytes_read;

    if (strcmp(requested_path, "/") == 0)
    {
        strcpy(requested_path, "/www/index.html");
    }

    strncat(file_path, requested_path, sizeof(file_path) - strlen(file_path) - 1);

    FILE *file = fopen(file_path, "r");

    struct stat file_stat;

    if (file)
    {
        stat(file_path, &file_stat);

        if (S_ISDIR(file_stat.st_mode))
        {
            snprintf(http_header, sizeof(http_header),
                     "HTTP/1.1 403 Forbidden\r\n"
                     "Content-Type: text/plain\r\n"
                     "Content-Length: 15\r\n"
                     "Connection: close\r\n\r\n"
                     "403 Forbidden");

            if ((send(client_socket, http_header, strlen(http_header), 0)) == -1)
            {
                perror("Error sending http header");
            }

            printf(" [ 403 ]\n");

            fclose(file);

            close(client_socket);

            return;
        }

        size_t file_size = file_stat.st_size;
        snprintf(http_header, sizeof(http_header),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: %s\r\n"
                 "Content-Length: %zu\r\n"
                 "Connection: close\r\n\r\n",
                 get_content_type(strrchr(requested_path, '.')), file_size);

        if ((send(client_socket, http_header, strlen(http_header), 0)) == -1)
        {
            perror("Error sending http header");
        }

        printf(" [ 200 ]\n");

        while ((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), file)) > 0)
        {
            send(client_socket, file_buffer, bytes_read, 0);
        }

        fclose(file);
    }

    else
    {
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
        }
    }

    close(client_socket);
    return;
}

int main()
{
    // Define buffer for client request

    char client_request_buffer[CLIENT_REQUEST_BUFFER_SIZE];
    char client_ip[INET_ADDRSTRLEN];

    // Space to store client http request method, path and version

    char *client_method, *client_requested_path, *client_http_version, *read_type;

    // Register sigint handler

    signal(SIGINT, handle_sigint);

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

    printf("Server is listening on port 8001...\n\n");

    while (1)
    {
        // Accept client requests

        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);

        inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip)) == NULL;

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

        if (client_method && client_requested_path && client_http_version && client_ip)
        {
            printf("%s : ", client_method);
            printf("[ %s ] :", client_ip);
            printf(" [ %s ]", client_requested_path);
            printf(" %s", client_http_version);
        }

        if ((strcmp(client_method, "GET")) == 0)
        {
            handle_get_request(client_socket, client_requested_path);
        }
    }

    return 0;
}