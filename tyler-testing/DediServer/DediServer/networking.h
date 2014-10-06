#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define MYBUFSIZE	20000
#define maxClients 10

struct message {
	char msg[MYBUFSIZE];
	int msgLength;
};

class networking {
private:
	SOCKET srvr_socket;
	SOCKET clients[maxClients];
	bool bActive[maxClients];
	message msgs[maxClients];
	struct sockaddr_in srvr_addr;
	struct sockaddr_in clnt_addr[maxClients];
	//struct sockaddr_in clnt_addr; // Client address
	int addr_len;
	int curClients;
	WSADATA wsaData;
	FD_SET fd;
	int result;
	timeval wait;
	timeval instant;
	char recv_buf[MYBUFSIZE];
	int recv_msg_len;
	
	void FDClient();
public:
	networking();
	bool init(int port);
	bool update();
	bool sendMSGs();
	bool setMSG(int cli,int msgLength,char*msg);
	bool shutdown();
	~networking();
};