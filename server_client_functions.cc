#include "server_client_functions.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>


long get_file_size(std::FILE* ptr){

	long size;

	fseek(ptr, 0, SEEK_END);
	size = ftell(ptr);
	fseek(ptr, 0, SEEK_SET);

	return size;
}


void receive_image(int clientfd, const char* save_path){

char buffer[BUFFER_SIZE];
long file_size, total_bytes_received = 0, bytes_received = 0;
FILE* fd;

//receive the file size from client
bytes_received = recv(clientfd, &file_size, sizeof(file_size), 0);

if(bytes_received == -1){
    std::cerr<<"error receiving file size...";
    exit(EXIT_FAILURE);
}

// convert the file size to host byte order to be able to use it 
file_size = ntohl(file_size);

// open the file for writing
fd = fopen(save_path, "wb");

if (fd == NULL ){
    std::cerr << "Error creating the file...";
    return;
}

// receive file data in chunks
while(total_bytes_received < file_size){

    bytes_received = recv(clientfd, buffer, sizeof(buffer), 0);

    if(bytes_received <= 0) {
        if(bytes_received ==0){
            std::cerr << "Connection lost.";
        }
        else{
            std::cerr << "Error receiving file";
            return;
        }
        return;
    }
    // write the received data to the file
    fwrite(buffer, 1, bytes_received, fd);

    total_bytes_received += bytes_received;
}

// free the resource
fclose(fd);

}


void send_image(int serverfd, const char* image) {
    std::FILE* fd;
    char buffer[BUFFER_SIZE];
    long file_size;
    uint32_t network_file_size;
    ssize_t bytes_sent = 0, total_bytes = 0;
    size_t bytes_read;

    fd = fopen(image, "rb");
    if (fd == NULL) {
        std::cerr << "Could not read the file " << image;
        return;
    }

    file_size = get_file_size(fd);
    network_file_size = htonl((uint32_t)file_size);

    if (send(serverfd, &network_file_size, sizeof(network_file_size), 0) != sizeof(network_file_size)) {
        std::cerr << "Error sending file size";
        fclose(fd);
        return;
    }

    while (total_bytes < file_size) {
        bytes_read = fread(buffer, 1, BUFFER_SIZE, fd);
        if (bytes_read == 0) {
            if (ferror(fd)) {
                std::cerr << "Error reading file";
                fclose(fd);
                return;
            }
            break;  // End of file
        }

        ssize_t bytes_to_send = bytes_read;
        char* buf_ptr = buffer;
        while (bytes_to_send > 0) {
            bytes_sent = send(serverfd, buf_ptr, bytes_to_send, 0);
            if (bytes_sent < 0) {
                std::cerr << "Error sending file data";
                fclose(fd);
                return;
            }
            bytes_to_send -= bytes_sent;
            buf_ptr += bytes_sent;
            total_bytes += bytes_sent;
        }
    }

    fclose(fd);
    std::cout << "Image sent successfully! Total bytes: " << total_bytes;
}