#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Goal -> To Learn about how a web server works under the hood, don't use AI slop generator.
// Base knowledge on https://github.com/bloominstituteoftechnology/C-Web-Server

// 1. Started on 17/06/2024 -> Read https://github.com/bloominstituteoftechnology/C-Web-Server/blob/master/guides/net.md
// 2. Started on 18/06/2024 -> Already knew what a doubly linked list is, but read about it anyway https://github.com/bloominstituteoftechnology/C-Web-Server/blob/master/guides/dllist.md
// Wtf is an LRU cache? -> https://github.com/bloominstituteoftechnology/C-Web-Server/blob/master/guides/lrucache.md
// In the context of a webserver, when a file is requested by a client, the following steps occur:
// Check if the file is in the cache.
// If it is, serve it. We're done.
// If it's not in the cache, load it from disk.
// Save it in the cache.
// Serve it.

int main()
{
    printf("Initiallising Server...\n");
    int ServerFileDescriptor; // Unique id that os gives to the server socket
    int ClientFileDescriptor; // Unique id that os gives to the client socket
    char Buffer[BUFFER_SIZE]; // Buffer to store the data received from the client
    //
    // Create a socket
    ServerFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerFileDescriptor < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    return 0;
}