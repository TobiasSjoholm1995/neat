#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <limits.h>
#include <pwd.h>
#include <sys/stat.h>

#define PM_SOCK_BACKLOG 5
#define BUFSIZE 256

#define PM_SOCK_DIR ".neat"
#define PM_SOCK_NAME "pm_socket"

char *
pm_socket_path() {
    char pm_socket_path[PATH_MAX];
    char *homedir;
    struct stat st;

    homedir = getenv("HOME");

    if (homedir == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    snprintf(pm_socket_path, PATH_MAX, "%s/%s/", homedir, PM_SOCK_DIR);

    if (stat(pm_socket_path, &st) == -1) {
        mkdir(pm_socket_path, 0700);
        printf("created directory %s\n", pm_socket_path);
    }

    return strncat(pm_socket_path, PM_SOCK_NAME, PATH_MAX - strlen(pm_socket_path));
}

void
handle_request(const char *request, int client_sock) {
    char candidates[BUFSIZE];

    printf("handling request: %s\n", request);

    // candidates = candidates(request);
    strcpy(candidates, "{}");

    printf("candidates: %s\n", candidates);

    if (send(client_sock, candidates, sizeof(candidates), 0) == -1) {
        perror("send");
        // exit(1);
    }
    printf("candidates sent\n");
}

int
main(int argc, char **argv) {
    int pm_sock;
    int client_sock;
    int bytes_read;
    struct sockaddr_un addr;
    char buffer[BUFSIZE];
    char *pm_sock_path;

    pm_sock_path = pm_socket_path();

    memset(&addr, 0, sizeof(addr));
    memset(buffer, 0, BUFSIZE);
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, pm_sock_path, sizeof(addr.sun_path)-1);

    if ((pm_sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        close(pm_sock);
        return 1;
    }

    unlink(pm_sock_path);
    if(bind(pm_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind error");
        return 1;
    }

    if (listen(pm_sock, PM_SOCK_BACKLOG) == -1) {
        perror("listen error");
        return 1;
    }

    for (;;) {
        if ((client_sock = accept(pm_sock, 0, 0)) == -1) {
            perror("accept error");
            return 1;
        }
        while ((bytes_read = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
            printf("received %s (%u bytes)\n", buffer, bytes_read);
        }
        if (bytes_read == -1) {
            perror("read error");
            close(pm_sock);
            close(client_sock);
            return 1;
        }
        if (bytes_read == 0) {
            handle_request(buffer, client_sock);
            close(client_sock);
        }
    }

    return 0;
}
