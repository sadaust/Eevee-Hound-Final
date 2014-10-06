#include "networking.h"

void shutdown_close(SOCKET s) {

	// Tell the operating system the socket is
	// about to be closed	
	shutdown(s, SD_BOTH); 
	// close the socket….
	closesocket(s); 
}

void shutdown_close_and_exit(SOCKET s) {
	shutdown_close(s);
	exit(1);
}

networking::networking() {
	addr_len = sizeof(clnt_addr[0]);
	curClients = 0;
	result = 0;
	wait.tv_sec = 1;
	wait.tv_usec = 100;
	instant.tv_sec = 0;
	instant.tv_usec = 0;
	for(int i = 0;i<maxClients;++i)
		bActive[i] = false;
}

void networking::FDClient() {
	FD_ZERO(&fd);
	for(int i = 0; i < maxClients;++i) {
		if(bActive[i]) {
			FD_SET(clients[i],&fd);
		}
	}
}

//return true if fail
bool networking::init(int port) {
	//init winsock
	if(WSAStartup(MAKEWORD(2,0),&wsaData)!= 0) {
		fprintf(stderr,"Error: WSAStartup() faild");
		return true;
	}

	//create tcp listen socket
	if((srvr_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))== SOCKET_ERROR) {
		fprintf(stderr, "Error: socket() failed with error %d\n", WSAGetLastError());
		shutdown_close(srvr_socket);
		return true;
	}

	//construct local address
	memset(&srvr_addr,0,sizeof(srvr_addr));
	srvr_addr.sin_family = AF_INET;
	srvr_addr.sin_addr.s_addr = INADDR_ANY;
	srvr_addr.sin_port = htons(port);

	//bind local address
	if(bind(srvr_socket, (struct sockaddr *) &srvr_addr,sizeof(srvr_addr)) == SOCKET_ERROR) {
		fprintf(stderr,"Error: socket() faild with error %d",WSAGetLastError());
		shutdown_close(srvr_socket);
		return true;
	}

	if(listen(srvr_socket,1) == SOCKET_ERROR) {
		printf("listen failed with error: %d\n",WSAGetLastError());
		shutdown_close(srvr_socket);
		return true;
	}
	return false;
}

bool networking::update() {
	FD_ZERO(&fd);
	FD_SET(srvr_socket,&fd);
	int temp = 0;

	if((result = select(0,&fd,0,0,&instant)) == SOCKET_ERROR) {
		printf("select failed with error: %d\n",WSAGetLastError());
		shutdown_close(srvr_socket);
		return true;
	}
	if( result == 1) {
		if(curClients < maxClients) {
			while(bActive[temp]) {
				++temp;
			}
			clients[temp] = accept(srvr_socket,(sockaddr*)&clnt_addr,&addr_len);
			if(clients[temp] == INVALID_SOCKET) {
				printf("accept failed with error: %d\n",WSAGetLastError());
				shutdown_close(clients[temp]);
				return true;
			} else {
				bActive[temp] = true;
				clnt_addr[temp];
				++curClients;
			}
		}
	}
	if(curClients > 0) {
		FDClient();
		if((result = select(0,&fd,0,0,&instant)) == SOCKET_ERROR) {

		} else if(result > 0) {
			for(int i = 0; i < fd.fd_count; ++i) {
				if ((recv_msg_len = recvfrom(fd.fd_array[i], recv_buf, MYBUFSIZE, 0,
					(struct sockaddr *) &clnt_addr[i], &addr_len )) == SOCKET_ERROR) {
						//disconnect client
						printf("recvfrom() failed with error: %d\n",WSAGetLastError());
						for(int z = 0; z < maxClients;++z) {
							if(clients[z] == fd.fd_array[i]) {
								shutdown_close(clients[z]);
								bActive[z] = false;
								--curClients;
								printf("client %d disconected\n",z);
							}
						}
				} else {
					//handel tcp msg
					printf("%d\n",recv_msg_len);
				}
			}
		}
	}
	return false;
};

bool networking::sendMSGs() {
	int temp = 0;
	FDClient();
	if((result = select(0,0,&fd,0,&wait)) == SOCKET_ERROR) {

	} else if(result > 0) {
		for(int i = 0; i < fd.fd_count; ++i) {
			temp = 0;
			while(clients[temp] != fd.fd_array[i])
				++temp;
			if(sendto(fd.fd_array[i], msgs[temp].msg, msgs[temp].msgLength, 0,(sockaddr *) &clnt_addr[temp], addr_len) == SOCKET_ERROR) {
				printf("sendto() failed with error: %d\n",WSAGetLastError());
				//disconnect client
				for(int z = 0; z < maxClients;++z) {
					if(clients[z] == fd.fd_array[i]) {
						shutdown_close(clients[z]);
						bActive[z] = false;
						--curClients;
						printf("client %d disconected\n",z);
					}
				}
			}
		}
	}
	return false;
}

bool networking::setMSG(int client,int msgLen,char * msg){
	if(client<maxClients&&client>=0&&msgLen<MYBUFSIZE) {
		msgs[client].msgLength = msgLen;
		for(int i = 0; i <msgLen;++i) {
			msgs[client].msg[i] = msg[i];
		}
		return false;
	}
	return true;
}

bool networking::shutdown() {
	printf("shutting down server\n");
	for(int i = 0; i < maxClients; ++i) {
		if(bActive[i])
			shutdown_close(clients[i]);
	}
	shutdown_close(srvr_socket);
	printf("WSACleanup\n");
	WSACleanup();
	return true;
}

networking::~networking() {
	shutdown();
}