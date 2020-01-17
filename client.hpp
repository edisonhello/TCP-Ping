#ifndef _CLIENT_H_INCLUDED
#define _CLIENT_H_INCLUDED

#include <utility>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>
#include <cstring>
#include <climits>
#include <unistd.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "debugger.hpp"

constexpr int kMaxMessageLength = 1024;

void ShowHelp();
std::string GetIP(std::string host);
std::tuple<int, int, std::vector<std::pair<std::string, int>>> ParseArgument(int argc, char **argv);
void Ping(uint64_t packet_count, int timeout, std::pair<std::string, int> _host);

#endif // _CLIENT_H_INCLUDED
