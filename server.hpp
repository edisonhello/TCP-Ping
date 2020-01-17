#ifndef _SERVER_H_INCLUDED
#define _SERVER_H_INCLUDED

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <thread>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "debugger.hpp"

const char *kServerHost = "127.0.0.1";
constexpr int kMaxConnection = 1024;
constexpr int kMaxMessageLength = 1024;

#endif // _SERVER_H_INCLUDED
