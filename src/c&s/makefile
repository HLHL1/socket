all:tcpserver tcpclient
tcpserver:tcpserver.cpp XTcp.cpp XTcp.h
	g++ tcpserver.cpp XTcp.cpp -o tcpserver -lpthread

tcpclient:tcpclient.cpp XTcp.cpp XTcp.h
	g++ tcpclient.cpp XTcp.cpp -o tcpclient -lpthread
