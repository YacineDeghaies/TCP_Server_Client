/*

TCP Client from scratch

*/

//include necessary headers

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h> // for socket, send, recv
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include "server_client_functions.h"

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;


int main(int argc, char const *argv[])
{
	int clientfd;
	struct sockaddr_in server_info;

	clientfd = socket(AF_INET, SOCK_STREAM, 0);

	if(clientfd == -1){
		cerr <<"Error creating socket.."<<endl;
		return 1;
	}

// set up server address structure
	memset(&server_info, 0, sizeof(server_info));

	server_info.sin_port = htons(PORT);
	server_info.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_info.sin_family = AF_INET;

//connect to server
	if(connect(clientfd, (struct sockaddr* ) &server_info, sizeof(server_info)) == -1){
		cerr <<"Error connecting to server\n";
		close(clientfd);
		return 1;
	}
	cout << "Connected to server !";
	
	send_image(clientfd, "period.png");

	close(clientfd);
	
	return 0;
}