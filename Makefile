All:
	g++ -std=c++17 server.cpp -o server
	g++ -std=c++17 client.cpp -o client -pthread

debug:
	g++ -std=c++17 server.cpp -o server -DDEBUG
	g++ -std=c++17 client.cpp -o client -DDEBUG -pthread

clean:
	rm server client
