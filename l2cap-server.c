

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

int total = 0;

int bt_send(int client, int size) {
    char buf[600];

    memset(buf, 1, sizeof(buf));

    while (size > 0) {
        int n = size > sizeof(buf) ? sizeof(buf) : size;
        int written = write(client, buf, n);
        if (written == -1) {
            perror("write");
            abort();
        }

        total += written;
        size -= written;

        printf("sent %d bytes, total %d bytes\n", written, total);
    }
}

int main(int argc, char **argv)
{
    struct sockaddr_l2 loc_addr = { 0 }, rem_addr = { 0 };
    char buf[600] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP);
    if (s == -1) {
        perror("socket");
        abort();
    }

    // bind socket to port 130 of the first available 
    // bluetooth adapter
    loc_addr.l2_family = AF_BLUETOOTH;
    loc_addr.l2_bdaddr = *BDADDR_ANY;
    loc_addr.l2_bdaddr_type = BDADDR_LE_PUBLIC;
    loc_addr.l2_psm = htobs(130);

    if (bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) == -1) {
        perror("bind");
        abort();
    }

    // put socket into listening mode
    if (listen(s, 1) == -1) {
        perror("listen");
        abort();
    }

    fprintf(stderr, "Ready\n");

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    if (client == -1) {
        perror("accept");
        abort();
    }

    ba2str( &rem_addr.l2_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);

    // write data to the client
    int n = 1;
    for (int p=0; p <= 5; p++, n *= 10) {
        bt_send(client, n);
    }

    printf("sleeping\n");
    sleep(10);

    bt_send(client, 1);
    sleep(1);

    printf("Sent total of %d bytes\n", total);

    // close connection
    close(client);
    close(s);
}
