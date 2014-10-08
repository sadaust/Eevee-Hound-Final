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
	//starts a server and sets its port
	bool init(int port);
	//recives messages and dissconects timed out clients recive and send share message space
	bool update();
	char* getMSG(int cli,int& leng);
	//sends clients their messages and dissconects timed out clients
	bool sendMSGs();
	//sets a message length and message recive and send share message space
	bool setMSG(int cli,int msgLength,char*msg);
	//shuts down server and dissconects clients
	bool shutdown();
	~networking();
};