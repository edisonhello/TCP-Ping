#include "client.hpp"

void ShowHelp() {
	perr << "Usage: ./client [-n number] [-t timeout] <host>:<port> [host:port [...]]" << std::endl;
	exit(1);
}

std::string GetIP(std::string host) {
    uint32_t a, b, c, d;
    int r = sscanf(host.c_str(), "%u.%u.%u.%u", &a, &b, &c, &d);
    if (r == 4 && !(a >> 8) && !(b >> 8) && !(c >> 8) && !(d >> 8)) return host;
    addrinfo hints;
    addrinfo *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (int status = getaddrinfo(host.c_str(), "", &hints, &result); status != 0) {
        perr << "getaddrinfo failed when getting IP for " << host << ", err msg = \"" << gai_strerror(status) << "\"" << std::endl;
        return host;
    }

    char ip[128];
    inet_ntop(AF_INET, reinterpret_cast<void *>(&reinterpret_cast<sockaddr_in *>(result->ai_addr)->sin_addr), ip, sizeof(ip));
    return std::string(ip);
}

std::tuple<int, int, std::vector<std::pair<std::string, int>>> ParseArgument(int argc, char **argv) {
	int number = 0, timeout = 1000;
	std::vector<std::pair<std::string, int>> hosts;
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) ShowHelp();
		else if (strcmp(argv[i], "-n") == 0) {
			number = atoi(argv[++i]);
			if (number < 0) ShowHelp();
		} else if (strcmp(argv[i], "-t") == 0) {
			timeout = atoi(argv[++i]);
			if (number < 0) ShowHelp();
		} else {
			char host[64]; int port;
			int read = sscanf(argv[i], "%[^:]:%d", host, &port);
            if (read != 2) ShowHelp();
            plog << "host = " << host << ", port = " << port << std::endl;
            hosts.emplace_back(GetIP(std::string(host)), port);
		}
	}
	return make_tuple(number, timeout, hosts);
}

void Ping(uint64_t packet_count, int timeout, std::pair<std::string, int> _host) {
	auto [host, port] = _host;
	if (packet_count == 0) packet_count = ULLONG_MAX;

	while (packet_count--) {
		if (packet_count == 1'000'000'000'000) packet_count *= 10;

		int client_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (client_socket < 0) {
			perr << "client_socket open failed: err = " << client_socket << std::endl;
			return;
		}
		plog << "client_socket open at fd = " << client_socket << std::endl;

		sockaddr_in server_address;
		memset(&server_address, 0, sizeof(server_address));
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = inet_addr(host.c_str());
		server_address.sin_port = htons(port);
		socklen_t server_address_size = sizeof(server_address);

		timeval _timeout;
		_timeout.tv_sec = timeout / 1000;
		_timeout.tv_usec = timeout % 1000 * 1000;

		if (setsockopt (client_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char *>(&_timeout), sizeof(_timeout)) < 0 ||
			setsockopt (client_socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char *>(&_timeout), sizeof(_timeout)) < 0)
			perr << "setsockopt failed" << std::endl;

		std::chrono::steady_clock::time_point t_start = std::chrono::steady_clock::now();
		int connect_err = connect(client_socket, reinterpret_cast<sockaddr *>(&server_address), server_address_size);
		std::chrono::steady_clock::time_point t_stop = std::chrono::steady_clock::now();

		if (connect_err < 0) {
			plog << "Connect to server failed, err = " << connect_err << std::endl;
			std::cout << "timeout when connect to " << host << std::endl;
		} else {
			auto rtt = std::chrono::duration_cast<std::chrono::milliseconds>(t_stop - t_start).count();

			plog << "RTT to " << host << ":" << port << " is " << rtt << " msec" << std::endl;
			std::cout << "recv from " << host << ":" << port << ", RTT = " << rtt << " msec" << std::endl;
		} 

		close(client_socket);

		sleep(1);
	}
	
}

int main(int argc, char **argv) {
	if (argc < 2) { ShowHelp(); }
	auto [packet_count, timeout, hosts] = ParseArgument(argc, argv);
	
	std::vector<std::thread> thrs;
	for (auto host : hosts) {
		thrs.push_back(std::thread(Ping, packet_count, timeout, host));
	}

	for (auto &t : thrs) t.join();
}
