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
    uint8_t subnet = 0;
    bool analyse = false;
    int opt;

    while ((opt = getopt(argc, argv, "as:")) != -1) {
        switch (opt) {
            case 's':
                subnet = atoi(optarg);
                break;
            case 'a':
                analyse = true;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (argc <= optind) {
        fprintf(stderr, "Error: An IPv4 CIDR must be given as the first argument.\n");
        exit(EXIT_FAILURE);
    }

    sscanf(argv[optind], "%[^/]/%hhu", network, &bits);
    if (inet_aton(network, &addr) == 0) {
        fprintf(stderr, "Error: Invalid IP address given.\n");
        exit(EXIT_FAILURE);
    }

    if (bits > 32) {
        fprintf(stderr, "Error: Invalid mask given.\n");
        exit(EXIT_FAILURE);
    }

    if (bits == 32) {
        printf("%s\n", inet_ntoa(addr));
        exit(EXIT_SUCCESS);
    }

    mask = ~0 << (32 - bits);
    hladdr = ntohl(addr.s_addr);
    hladdr_start = hladdr & mask;
    hladdr_end = (hladdr & mask) | ~mask;

    if (subnet) {
        if (subnet <= bits) {
            fprintf(stderr, "Error: Subnetwork must be greater than bits.\n");
            exit(EXIT_FAILURE);
        }
        if (subnet > 32) {
            fprintf(stderr, "Error: Subnetwork must not be greater than 32.\n");
            exit(EXIT_FAILURE);
        }

        for (i = hladdr_start; i <= hladdr_end; i += (1 << (32 - subnet))) {
            addr.s_addr = htonl(i);
            printf("%s/%hhu\n", inet_ntoa(addr), subnet);
        }

        exit(EXIT_SUCCESS);
    }

    if (analyse) {
        addr.s_addr = htonl(hladdr_start);
        printf("Network:    %s\n", inet_ntoa(addr));

        addr.s_addr = htonl(hladdr_end);
        printf("Broadcast:  %s\n", inet_ntoa(addr));

        addr.s_addr = htonl(mask);
        printf("Netmask:    %s\n", inet_ntoa(addr));

        printf("Hosts:      %u\n", hladdr_end - hladdr_start + 1);

        exit(EXIT_SUCCESS);
    }

    for (i = hladdr_start; i <= hladdr_end; i++) {
        addr.s_addr = htonl(i);
        printf("%s\n", inet_ntoa(addr));
    }

    exit(EXIT_SUCCESS);
}
