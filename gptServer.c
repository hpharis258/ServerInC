#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // 1. Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    // 2. Prepare the address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    server_addr.sin_port = htons(PORT);

    // 3. Bind socket to the port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(server_fd);
        exit(1);
    }

    // 4. Start listening
    if (listen(server_fd, 10) < 0)
    {
        perror("listen");
        close(server_fd);
        exit(1);
    }

    printf("Listening on http://localhost:%d\n", PORT);

    while (1)
    {
        // 5. Accept a client connection
        client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0)
        {
            perror("accept");
            continue;
        }

        // 6. Read the request (we don't parse it yet)
        recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        printf("Received request:\n%s\n", buffer);

        // 7. Send an HTTP response
        const char *response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "Hello from C!";

        send(client_fd, response, strlen(response), 0);

        // 8. Close the connection
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
