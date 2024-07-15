#ifndef SERVER_CLIENT_FUNC
#define SERVER_CLIENT_FUNC

#include <cstdio>
// struct FILE;


#define PORT 8080
#define BACKLOG 5
#define BUFFER_SIZE 1024

void receive_image(int clientfd, const char* save_path);
void send_image(int serverfd, const char* image);
long get_file_size(FILE* ptr);

#endif 