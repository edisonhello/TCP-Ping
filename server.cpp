#include "server.hpp"

int main(int argc, char **argv) {
	if (argc < 2) {
		perr << "Usage: ./server <port>" << std::endl;
		exit(1);
	}
	const int server_open_port = atoi(argv[1]);

	int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket < 0) {
		perr << "server_socket open failed: err = " << server_socket << std::endl;
		exit(1);
	}
	plog << "server_socket open at fd = " << server_socket << std::endl;

	sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(server_open_port);
	bind(server_socket, reinterpret_cast<sockaddr *>(&server_address), sizeof(server_address));
	listen(server_socket, kMaxConnection);
	plog << "Server listening" << std::endl;

	while (true) {
		sockaddr_in client_address;
		socklen_t client_address_size = sizeof(client_address);
		int client_socket = accept(server_socket, reinterpret_cast<sockaddr *>(&client_address), &client_address_size);
		plog << "Accept connection from " << inet_ntoa(client_address.sin_addr) << ":" << client_address.sin_port << " in fd = " << client_socket << std::endl;
		std::cout << "recv from " << inet_ntoa(client_address.sin_addr) << ":" << client_address.sin_port << std::endl;

		close(client_socket);
	}
}
