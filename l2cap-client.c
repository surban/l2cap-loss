

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    struct sockaddr_l2 addr = { 0 };
    struct sockaddr_l2 loc_addr = { 0 };
    int s, status;
    char dest[18] = "01:23:45:67:89:AB";
    char buf[600];

    if(argc < 2)
    {
        fprintf(stderr, "usage: %s <bt_addr>\n", argv[0]);
        exit(2);
    }

    strncpy(dest, argv[1], 18);

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP);
    if (s == -1) {
        perror("socket");
        abort();
    }

    // bind socket to LE
    loc_addr.l2_family = AF_BLUETOOTH;
    loc_addr.l2_bdaddr = *BDADDR_ANY;
    loc_addr.l2_bdaddr_type = BDADDR_LE_PUBLIC;
    loc_addr.l2_psm = 0;

    if (bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) == -1) {
        perror("bind");
        abort();
    }

    // set the connection parameters (who to connect to)
    addr.l2_family = AF_BLUETOOTH;
    addr.l2_bdaddr_type = BDADDR_LE_PUBLIC;
    addr.l2_psm = htobs(130);
    str2ba( dest, &addr.l2_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if( status == -1 ) {
        perror("connect");
        abort();
    }

    int total = 0;
    int i = 0;
    while (1) {
        int n = read(s, buf, sizeof(buf));
        if (n == -1) {
            perror("read");
            break;
        }
        total += n;
        printf("Received %d bytes, total %d bytes\n", n, total);

        i++;
        if (i == 3) {
            printf("Sleeping purposfully\n");
            // Comment this out to make it work correctly.
            sleep(5);
        }
    }

    close(s);
}
