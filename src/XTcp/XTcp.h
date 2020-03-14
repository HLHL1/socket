#pragma once
#include <string>
class XTcp
{
public:
	int CreateSocket();
	bool Bind(unsigned short port);
	XTcp Accept();
	void Close();
	int Recv(char *buf, int bufsize);
	int Send(const char *buf, int sendsize);
	XTcp();
	virtual ~XTcp();
	int sock;
	unsigned short port;
	std::string ip;
};

