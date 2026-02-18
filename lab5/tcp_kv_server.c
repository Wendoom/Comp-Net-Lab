#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8898
#define MAX_CONN 10
#define BUFFER_SIZE 1024

typedef struct {
    int client_sock;
    struct sockaddr_in client_addr;
} client_data_t;

void *handle_client_threads(void *arg) {
    client_data_t *data = (client_data_t *)arg;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Handle client interaction (Echo server)
    while ((bytes_read = read(data->client_sock, buffer, sizeof(buffer))) > 0) {
        write(data->client_sock, buffer, bytes_read);
    }

    if (bytes_read == 0) {
        printf("Client disconnected from %s:%d\n",
               inet_ntoa(data->client_addr.sin_addr),
               ntohs(data->client_addr.sin_port));
    } 
    else if (bytes_read == -1) {
        perror("Error reading from socket");
    }

    close(data->client_sock);
    free(data);               // Free allocated memory
    pthread_exit(NULL);
}


int main(){
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t thread_id;

    // Create server socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_sock, (struct sockaddr *)&server_addr, 
             sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_sock, MAX_CONN) == -1) {
        perror("Listen failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    
}