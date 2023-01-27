/*
 * cidrl
 *
 * Lists all IPv4 addresses within a CIDR block.
 *
 * TODO `cidrl 16/28` should work.
 *
 * @author Damien Bezborodov
 * @link https://github.com/emden-norfolk/cidrl
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>

extern char *optarg;
extern int optind;

int main(int argc, char **argv) {
    char network[15];
    struct in_addr addr;
    uint32_t mask, hladdr, hladdr_start, hladdr_end, i;
    uint8_t bits;
    uint8_t subnetwork = 0;
    bool analyse = false;
    int opt;

    while ((opt = getopt(argc, argv, "as:")) != -1) {
        switch (opt) {
            case 's':
                subnetwork = atoi(optarg);
                break;
            case 'a':
                analyse = true;
                break;
            default:
                exit(1);
        }
    }

    if (argc <= optind) {
        fprintf(stderr, "Error: An IPv4 CIDR must be given as the first argument.\n");
        return 1;
    }

    sscanf(argv[optind], "%[^/]/%hhu", network, &bits);
    if (inet_aton(network, &addr) == 0) {
        fprintf(stderr, "Error: Invalid IP address given.\n");
        return 2;
    }

    if (bits > 32) {
        fprintf(stderr, "Error: Invalid subnet mask given.\n");
        return 3;
    }

    if (bits == 32) {
        printf("%s\n", inet_ntoa(addr));
        return 0;
    }

    mask = ~0 << (32 - bits);
    hladdr = ntohl(addr.s_addr);
    hladdr_start = hladdr & mask;
    hladdr_end = (hladdr & mask) | ~mask;

    if (subnetwork) {
        if (subnetwork <= bits) {
            fprintf(stderr, "Error: Subnetwork must be greater than bits.\n");
            return 4;
        }
        if (subnetwork > 32) {
            fprintf(stderr, "Error: Subnetwork must not be greater than 32.\n");
            return 3;
        }

        for (i = hladdr_start; i <= hladdr_end; i += (1 << (32 - subnetwork))) {
            addr.s_addr = htonl(i);
            printf("%s/%hhu\n", inet_ntoa(addr), subnetwork);
        }

        return 0;
    }

    if (analyse) {
        addr.s_addr = htonl(hladdr_start);
        printf("Network:    %s\n", inet_ntoa(addr));
        addr.s_addr = htonl(hladdr_end);
        printf("Broadcast:  %s\n", inet_ntoa(addr));
        addr.s_addr = htonl(mask);
        printf("Netmask:    %s\n", inet_ntoa(addr));
        printf("Hosts:      %u\n", hladdr_end - hladdr_start + 1);
        return 0;
    }

    for (i = hladdr_start; i <= hladdr_end; i++) {
        addr.s_addr = htonl(i);
        printf("%s\n", inet_ntoa(addr));
    }

    return 0;
}
