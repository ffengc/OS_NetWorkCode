.PHONY:all
all:tcp_client tcp_server

tcp_client:tcp_client.cc
	g++ -o $@ $^ -std=c++11 -g -lpthread
tcp_server:tcp_server.cc
	g++ -o $@ $^ -std=c++11 -g -lpthread

.PHONY:clean
clean:
	rm -f tcp_client tcp_server