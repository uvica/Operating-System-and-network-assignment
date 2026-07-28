/*
 * Task 4 - Client
 * Network Programming and IPC
 * Author: Uvica Shrestha
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int clientSocket;

    struct sockaddr_in serverAddr;

    char username[50];
    char password[50];
    char message[1024];
    char buffer[1024];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket < 0)
    {
        printf("Socket creation failed.\n");
        return 1;
    }

    printf("Socket created successfully.\n");

    // Configure server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect
    if(connect(clientSocket,
               (struct sockaddr*)&serverAddr,
               sizeof(serverAddr)) < 0)
    {
        printf("Connection failed.\n");
        return 1;
    }

    printf("Connected to server.\n");

    // Login
    printf("Username: ");
    scanf("%49s", username);

    printf("Password: ");
    scanf("%49s", password);

    // Send credentials
    send(clientSocket, username, sizeof(username), 0);
    send(clientSocket, password, sizeof(password), 0);

    // Receive authentication result
    recv(clientSocket, buffer, sizeof(buffer), 0);

    printf("%s\n", buffer);

    if(strcmp(buffer, "Authentication Successful") != 0)
    {
        close(clientSocket);
        return 0;
    }

    getchar();

    // Send message
    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);

    // Validation
    if(strlen(message) <= 1)
    {
        printf("Empty message not allowed.\n");
        close(clientSocket);
        return 0;
    }

    send(clientSocket, message, sizeof(message), 0);

    // Receive reply
    recv(clientSocket, buffer, sizeof(buffer), 0);

    printf("Server: %s\n", buffer);

    close(clientSocket);

    printf("Connection closed.\n");

    return 0;
}