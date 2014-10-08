#include "networking.h"

void main(int argc, char *argv[]) {
	/*SOCKET srvr_socket, acceptSock;
	struct sockaddr_in srvr_addr;
	struct sockaddr_in clnt_addr; // Client address
	int addr_len = 0;
	short portnum;
	WSADATA wsaData;
	FD_SET fd;
	int result = 0;
	timeval wait = {1,100};
	timeval instant = {0,0};
	char recv_buf[MYBUFSIZE];
	int recv_msg_len;*/

	short portnum;
	networking net;
	bool go = true;;

	if(argc == 2) {
		portnum = atoi(argv[1]);
		printf("port = %d\n",portnum);
	} else {
		fprintf(stderr, "Usage: %s port\n",argv[0]);
		exit(1);
	}
	
	if(net.init(portnum)) {
		exit(1);
	}
	while(go == true) {
		if(net.update())
			go = false;
		net.sendMSGs();
	}
	net.shutdown();
}