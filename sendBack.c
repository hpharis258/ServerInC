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

        // 6. Read the request and ensure it's NUL-terminated
        ssize_t nread = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (nread <= 0)
        {
            perror("recv");
            close(client_fd);
            continue;
        }
        buffer[nread] = '\0';
        printf("Received request:\n%s\n", buffer);

        // Extract path and method from the first request line
        char method[8] = {0};
        char path[256] = {0};

        // Find end of the request line (CRLF preferred, fallback to LF)
        char *line_end = strstr(buffer, "\r\n");
        if (!line_end)
        {
            line_end = strchr(buffer, '\n');
        }

        char request_line[BUFFER_SIZE] = {0};
        if (line_end)
        {
            size_t line_len = (size_t)(line_end - buffer);
            if (line_len >= sizeof(request_line))
                line_len = sizeof(request_line) - 1;
            memcpy(request_line, buffer, line_len);
            request_line[line_len] = '\0';
        }
        else
        {
            // If no line ending is found, use the whole buffer as the request line
            strncpy(request_line, buffer, sizeof(request_line) - 1);
            request_line[sizeof(request_line) - 1] = '\0';
        }

        // Safely parse method and path from the request line
        int parsed = sscanf(request_line, "%7s %255s %*s", method, path);
        if (parsed < 2)
        {
            // Fallbacks if parsing failed
            if (method[0] == '\0')
                strncpy(method, "UNKNOWN", sizeof(method) - 1);
            if (path[0] == '\0')
                strncpy(path, "/", sizeof(path) - 1);
        }

        // 7. Send an HTTP response
        // Build body and accurate Content-Length
        char body[512];
        snprintf(body, sizeof(body),
                 "Hello from you requested : %s with method : %s",
                 path, method);

        char response[1024];
        int header_len = snprintf(response, sizeof(response),
                                  "HTTP/1.1 200 OK\r\n"
                                  "Content-Type: text/plain\r\n"
                                  "Content-Length: %zu\r\n"
                                  "\r\n",
                                  strlen(body));
        size_t total_len = (size_t)header_len;
        size_t body_len = strlen(body);
        if (header_len > 0 && header_len + body_len < sizeof(response))
        {
            memcpy(response + header_len, body, body_len);
            total_len += body_len;
        }

        send(client_fd, response, total_len, 0);

        // 8. Close the connection
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
