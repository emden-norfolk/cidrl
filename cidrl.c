/*
 * cidrl
 *
 * Lists all IPv4 addresses within a CIDR block.
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

int main(int argc, char **argv)
{
    char addr_buffer[15], whitespace;
    struct in_addr addr;
    uint32_t hladdr, hlmask, hlend;
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

    if (argc - optind != 1) {
        fprintf(stderr, "Error: An IPv4 CIDR must be given as the first argument.\n");
        exit(EXIT_FAILURE);
    }

    if (sscanf(argv[optind], "%[^/]/%hhu %c", addr_buffer, &bits, &whitespace) != 2) {
        fprintf(stderr, "Error: Invalid IPv4 CIDR given.\n");
        exit(EXIT_FAILURE);
    }

    if (inet_aton(addr_buffer, &addr) == 0) {
        fprintf(stderr, "Error: Invalid IPv4 address given.\n");
        exit(EXIT_FAILURE);
    }

    if (bits > 32) {
        fprintf(stderr, "Error: Invalid network prefix given.\n");
        exit(EXIT_FAILURE);
    }

    hlmask = bits ? ~0 << (32 - bits) : 0;
    hladdr = ntohl(addr.s_addr) & hlmask;
    hlend = hladdr | ~hlmask;

    if (subnet) {
        if (subnet <= bits) {
            fprintf(stderr, "Error: Subnet must have less hosts than the network.\n");
            exit(EXIT_FAILURE);
        }
        if (subnet > 32) {
            fprintf(stderr, "Error: Invalid subnet network prefix.\n");
            exit(EXIT_FAILURE);
        }

        for (uint32_t i = hladdr; i <= hlend; i += (1 << (32 - subnet))) {
            addr.s_addr = htonl(i);
            printf("%s/%hhu\n", inet_ntoa(addr), subnet);
        }

        exit(EXIT_SUCCESS);
    }

    if (analyse) {
        addr.s_addr = htonl(hladdr);
        printf("Network:    %s\n", inet_ntoa(addr));

        addr.s_addr = htonl(hlend);
        printf("Broadcast:  %s\n", inet_ntoa(addr));

        addr.s_addr = htonl(hlmask);
        printf("Netmask:    %s\n", inet_ntoa(addr));

        if (bits)
            printf("Hosts:      %u\n", hlend - hladdr + 1);
        else
            printf("Hosts:      4294967296\n");

        exit(EXIT_SUCCESS);
    }

    for (uint32_t i = hladdr; i <= hlend; i++) {
        addr.s_addr = htonl(i);
        printf("%s\n", inet_ntoa(addr));
    }

    exit(EXIT_SUCCESS);
}
