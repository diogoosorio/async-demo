#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include "./lib/server.h"
#include "./lib/signals.h"

static void handle_connection(int connection_fd) {
    pid_t pid = getpid();
    printf("pid %d | processing a new client\n", pid);

    char buffer[50];
    int bytes_read;

    while(1) {
        memset(&buffer, ' ', sizeof(buffer));
        bytes_read = read_line(connection_fd, buffer, 50);

        if (bytes_read < 0) {
            printf("pid %d | error receiving data | %s\n", pid, buffer);
            break;
        }

        if (strncmp(buffer, "goodbye", 7) == 0) {
            printf("pid %d | received goodbye | %s\n", pid, buffer);
            break;
        }
        
        printf("pid %d | finished reading line | %s\n", pid, buffer);
        printf("pid %d | simulating some busy work for %d seconds \n", pid, 2);
        sleep(2);
        write(connection_fd, "ack", 3);
        printf("pid %d | sent ack \n", pid);
    }
}


int main() {
    int exit = 0;
    trap_exit(&exit);

    int server_fd = create_server();
    if (server_fd == -1) {
        return 1;
    }

    while(exit == 0) {
        int connection_fd = accept_connection(server_fd);
        handle_connection(connection_fd);
        close(connection_fd);
    }
    close(server_fd);

    return 0;
}
