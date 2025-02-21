#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main()
{
	// Create socket and store its descriptor inside the variable 'network_socket'
	// AF_INET - IPv4 Internet Protocols
	// SOCK_STREAM - TCP

	int network_socket = socket(AF_INET, SOCK_STREAM, 0);

	// Check if there was an error creating the socket

	if (network_socket == -1)
	{
		perror("Error creating the socket");
		exit(EXIT_FAILURE);
	}

	// Specifying an address for the created socket

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9999);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// Try to connect to 'server_address' and store status in 'connection_status' variable

	int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));

	// Check if connection was successful

	if (connection_status == -1)
	{
		perror("An error occured while trying to connect to the remote socket");
		close(network_socket);
		exit(EXIT_FAILURE);
	}

	// Allow client to receive data from the server

	char server_response[256];
	memset(server_response, 0, sizeof(server_response));
	int bytes_received = recv(network_socket, &server_response, sizeof(server_response), 0);

	// Check if there was a problem receiving the data from the server

	if (bytes_received == -1)
	{
		perror("Receiving failed");
	}
	else if (bytes_received == 0)
	{
		printf("Server closed the connection.\n");
	}
	else
	{
		// Print server response if response was received successfully

		server_response[bytes_received] = '\0';
		printf("Data from server: %s\n", server_response);
	}

	// Close the socket

	close(network_socket);

	return 0;
}
