#include "utility.c"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[])
{
	if (argc < 3 || argc > 4)
	{
		die_with_user_msg("Parameter(s)", "<Server Address> <Echo Word> [Server Port]");
	}

	char *server_ip = argv[1];
	char *message = argv[2];
	in_port_t server_port = (argc == 4) ? atoi(argv[3]) : 7;
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock < 0)
	{
		die_with_sys_msg("socket() Failed !!!");
	}

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;

	int ret_val = inet_pton(AF_INET, server_ip, &server_address.sin_addr.s_addr);

	if (ret_val == 0)
	{
		die_with_user_msg("inet_pton() Failed !!!", "Invalid Address String");
	}
	else if (ret_val < 0)
	{
		die_with_sys_msg("inet_pton() Failed !!!");
	}

	server_address.sin_port = htons(server_port);

	if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		die_with_sys_msg("connect() Failed !!!");
	}

	size_t echo_string_length = strlen(message);
	ssize_t num_bytes = send(sock, message, echo_string_length, 0);

	if (num_bytes < 0)
	{
		die_with_sys_msg("send() Failed !!!");
	}
	else if (num_bytes != echo_string_length)
	{
		die_with_user_msg("send()", "Sent An Unexpected # Of Bytes");
	}

	u32 total_bytes_received = 0;
	fputs("Received: ", stdout);

	while (total_bytes_received < echo_string_length)
	{
		char buffer[BUFFER_SIZE];
		num_bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);

		if (num_bytes < 0)
		{
			die_with_sys_msg("recv() Failed !!!");
		}
		else if (num_bytes == 0)
		{
			die_with_user_msg("recv()", "Client Connection Was Closed Prematurely");
		}

		total_bytes_received += num_bytes;
		buffer[num_bytes] = '\0';
		fputs(buffer, stdout);
	}

	fputc('\n', stdout);
	close(sock);
	exit(0);
}
