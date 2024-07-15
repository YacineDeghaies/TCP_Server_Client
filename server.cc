/*

TCP Server from scratch

*/
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

#include "server_client_functions.h"

using namespace std;


// the driver function
int main() {
    struct sockaddr_in server_info, client_info;
    int socketfd, clientfd;

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_info, 0, sizeof(server_info));

    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = htonl(INADDR_ANY);
    server_info.sin_port = htons(PORT);

    if (::bind(socketfd, (struct sockaddr *)&server_info, sizeof(server_info)) == -1) {
        cerr << "Unable to bind to port." << endl;
        exit(EXIT_FAILURE);
    }

    if (listen(socketfd, BACKLOG) == -1) {
        cerr << "Unable to listen for connections." << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Server is listening on port " << PORT << endl;

    socklen_t client_len = sizeof(client_info);
    clientfd = accept(socketfd, (struct sockaddr *)&client_info, &client_len);
    if (clientfd == -1) {
        cerr << "Error accepting connection." << endl;
        exit(EXIT_FAILURE);
    }

    receive_image(clientfd, "./image.png");

    close(socketfd);
    close(clientfd);

    return 0;
}