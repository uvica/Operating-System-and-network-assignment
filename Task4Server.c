/*
 * Task 4 - Server
 * Network Programming and IPC
 * Author: Uvica Shrestha
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 8080

void handleClient(int clientSocket)
{
    char username[50];
    char password[50];
    char message[1024];
    char buffer[1024];

    // Receive username
    recv(clientSocket, username, sizeof(username), 0);

    // Receive password
    recv(clientSocket, password, sizeof(password), 0);

    // Authentication
    if(strcmp(username, "admin") != 0 ||
       strcmp(password, "password123") != 0)
    {
        strcpy(buffer, "Authentication Failed");

        send(clientSocket,
             buffer,
             strlen(buffer)+1,
             0);

        printf("Client authentication failed.\n");

        close(clientSocket);

        exit(0);
    }

    strcpy(buffer,"Authentication Successful");

    send(clientSocket,
         buffer,
         strlen(buffer)+1,
         0);

    printf("Client authenticated successfully.\n");

    // Receive message
    recv(clientSocket,
         message,
         sizeof(message),
         0);

    // Validate message
    if(strlen(message)==0)
    {
        strcpy(buffer,"Invalid Message");

        send(clientSocket,
             buffer,
             strlen(buffer)+1,
             0);

        close(clientSocket);

        exit(0);
    }

    printf("Client says: %s\n", message);

    strcpy(buffer,"Message received successfully.");

    send(clientSocket,
         buffer,
         strlen(buffer)+1,
         0);

    close(clientSocket);

    exit(0);
}
int main()
{
    int serverSocket;
    int clientSocket;

    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    socklen_t addrSize = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(serverSocket < 0)
    {
        printf("Socket creation failed.\n");
        return 1;
    }

    printf("Socket created successfully.\n");

    // Configure server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if(bind(serverSocket,
            (struct sockaddr*)&serverAddr,
            sizeof(serverAddr)) < 0)
    {
        printf("Bind failed.\n");
        return 1;
    }

    printf("Bind successful.\n");

    // Listen for connections
    listen(serverSocket, 5);

    printf("Server started.\n");
    printf("Waiting for clients...\n");

    while(1)
    {
        clientSocket = accept(serverSocket,
                              (struct sockaddr*)&clientAddr,
                              &addrSize);

        if(clientSocket < 0)
        {
            printf("Connection failed.\n");
            continue;
        }

        printf("Client connected.\n");

        pid_t pid = fork();

        if(pid == 0)
        {
            // Child process handles one client
            close(serverSocket);
            handleClient(clientSocket);
        }
        else if(pid > 0)
        {
            // Parent continues accepting clients
            close(clientSocket);

            while(waitpid(-1, NULL, WNOHANG) > 0);
        }
        else
        {
            printf("Fork failed.\n");
            close(clientSocket);
        }
    }

    close(serverSocket);

    return 0;
}