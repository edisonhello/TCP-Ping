# A simple TCP ping tool 

A simple TCP ping tool.

Project of Computer Network in NTU.CSIE.

## Compiling

Make sure your `g++` version supports c++17.

Use `make` to compile production version, and `make debug` to open debug outputs.

## Usage

### Server

```
./server <port number>
```

This will start a TCP ping server at port <port number>.

### Client

```
./client [options] host:port [host:port...]
```

Available options are

1. `-h` / `--help`: print help message.
2. `-n integer`: specify the number of packet the client will send. 0 means infinity, default is 0.
3. `-t integer`: specify the max timeout (in unit of millisecond) the client will wait the server to respond. Default is 1000.

You can ping multiple server at one time.

