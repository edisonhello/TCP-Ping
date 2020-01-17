--- 
title: "Computer Networks - Project 1 Report"
author: "B07902131" 
toc: false 
listings: true 
geometry: margin=0.5in 
documentclass: extarticle 
fontsize: 12pt 
header-includes: |
    \usepackage{amssymb}
    \usepackage{dsfont}
    \newcommand{\defas}{\coloneqq}
---

## Compile

Simply run `make` in current directory. Use `make debug` to show more details.

Note that this program require `c++17` to compile correctly (available on csie workstation).

## Usage

Use `./server <port>` to open a ping server.

Use `./client [options] <host>:<port> [<host>:<port> [...]]` to ping a ping server. Available options are

```
-h, --help:   Show help (then quit).
-n <number>:  Set number of times to ping. 0 means ping forever. The specific number 
			  support up to `INT_MAX` and should be non-negative. Default: 0.
-t <number>:  Set max timeout waiting a server connected. Unit: millisecond. Support up 
			  to `INT_MAX` and should be non-negative. Default: 1000.
```

.

## Others

1. The `client` will wait 1 second after previous ping to the same server gets response or timeout. 
2. Due to previous point, when pinging multiple server simultaneously, each ping thread of each server may not start their each ping simultaneously.
3. The RTT calculating  will round down.
3. The timing is based on the time before client `connect` and after. The server will close a connection as soon as the server accept it.
4. The max number of waiting accepted connection the server can handle is 1024. Since the server keeps closing connection immediately after accept, I hope the number of waiting accepted connection will never exceed this number.

\newpage

## Short Code Explain
### Server

1. Line 10: initial socket.
2. Line 17: setting to listen.
3. Line 26: accept a connection then close it.

### Client

1. Line 3: show help and exit.
2. Line 8: get host ip by domain name, or check it is an ip.
3. Line 30: parse given argument.
4. Line 52: the ping part.
5. Line 73: setting timeout.
6. Line 81: calculating time elapsed.
7. Line 85: check if the connection timeout or not.
8. Line 102: main part.
9. Line 108: create threads for each `host:port` being ping.
