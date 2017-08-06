#include "utility.c"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_PENDING 5
#define BUFFER_SIZE 256

internal void handle_tcp_client(int client_socket)
{
	char buffer[BUFFER_SIZE];
	ssize_t num_bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

	if (num_bytes_received < 0)
	{
		die_with_sys_msg("recv() Failed !!!");
	}

	while (num_bytes_received > 0)
	{
		ssize_t num_bytes_sent = send(client_socket, buffer, num_bytes_received, 0);

		if (num_bytes_sent < 0)
		{
			die_with_sys_msg("send() Failed !!!");
		}
		else if (num_bytes_sent != num_bytes_received)
		{
			die_with_user_msg("send()", "Sent Unexpected Number Of Bytes");
		}

		num_bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

		if (num_bytes_received < 0)
		{
			die_with_sys_msg("recv() Failed !!!");
		}
	}

	close(client_socket);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		die_with_user_msg("Parameter(s)", "[Server Port]");
	}

	in_port_t server_port = atoi(argv[1]);
	int server_sock = 0;

	if ((server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		die_with_sys_msg("socket() Failed !!!");
	}

	struct sockaddr_in server_address;

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(server_port);

	if (bind(server_sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		die_with_sys_msg("bind() Failed !!!");
	}

	if (listen(server_sock, MAX_PENDING) < 0)
	{
		die_with_sys_msg("listen() Failed !!!");
	}

	for (;;)
	{
		struct sockaddr_in client_address;
		socklen_t client_address_length = sizeof(client_address);
		int client_sock = accept(server_sock, (struct sockaddr *)&client_address, &client_address_length);

		if (client_sock < 0)
		{
			die_with_sys_msg("accept() Failed !!!");
		}

		char client_name[INET_ADDRSTRLEN];

		if (inet_ntop(AF_INET, &client_address.sin_addr.s_addr, client_name, sizeof(client_name)) != NULL)
		{
			printf("Handling Client %s:%d\n", client_name, ntohs(client_address.sin_port));
		}
		else
		{
			printf("Unable To Retrieve The Client's Address !!!\n");
		}

		handle_tcp_client(client_sock);
	}
}
