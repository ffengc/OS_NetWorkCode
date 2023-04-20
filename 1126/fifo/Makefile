.PHONY:all
all:client server

client:client.cc
	g++ -o $@ $^ -std=c++11
server:server.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f client server