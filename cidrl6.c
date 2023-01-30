/*
 * cidrl6
 *
 * Lists all IPv6 addresses within a CIDR block.
 *
 * @author Damien Bezborodov
 * @link https://github.com/emden-norfolk/cidrl
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "in6calc.h"

extern char *optarg;
extern int optind;

int main(int argc, char **argv)
{
    char addr_buffer[48], whitespace;
    struct in6_addr addr, mask, start, end;
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

    // Check that a CIDR is given as an argument.
    if (argc - optind != 1) {
        fprintf(stderr, "Error: An IPv6 CIDR must be given as the first argument.\n");
        exit(EXIT_FAILURE);
    }

    // Scan the CIDR argument into separate network and network prefix.
    if (sscanf(argv[optind], "%[^/]/%hhu %c", addr_buffer, &bits, &whitespace) != 2) {
        fprintf(stderr, "Error: Invalid IPv6 CIDR given.\n");
        exit(EXIT_FAILURE);
    }

    // Parse the IPv6 string into an integer.
    if (inet_pton(AF_INET6, addr_buffer, &addr) == 0) {
        fprintf(stderr, "Error: Invalid IPv6 address given.\n");
        exit(EXIT_FAILURE);
    }
    if (bits > 128) {
        fprintf(stderr, "Error: Invalid network prefix given.\n");
        exit(EXIT_FAILURE);
    }

    in6_addr_mask(&mask, bits);
    in6_addr_start(&start, &addr, &mask);
    in6_addr_end(&end, &start, &mask);

    if (subnet) {
        if (subnet <= bits) {
            fprintf(stderr, "Error: Subnet must have less hosts than the network.\n");
            exit(EXIT_FAILURE);
        }
        if (subnet > 128) {
            fprintf(stderr, "Error: Invalid subnet network prefix.\n");
            exit(EXIT_FAILURE);
        }
        if (subnet - bits > 64) {
            fprintf(stderr, "Error: Too many subnets.\n");
            exit(EXIT_FAILURE);
        }

        uint64_t n = (uint64_t)~0 >> (64 - subnet + bits);
        for (uint64_t i = 0; i <= n; i++) {
            inet_ntop(AF_INET6, &start, addr_buffer, 48);
            printf("%s/%hhu\n", addr_buffer, subnet);

            in6_addr_incr_pow2(&start, 128 - subnet);
        }

        exit(EXIT_SUCCESS);
    }

    if (analyse) {
        inet_ntop(AF_INET6, &start, addr_buffer, 48);
        printf("Start:      %s\n", addr_buffer);

        inet_ntop(AF_INET6, &end, addr_buffer, 48);
        printf("End:        %s\n", addr_buffer);

        inet_ntop(AF_INET6, &mask, addr_buffer, 48);
        printf("Netmask:    %s\n", addr_buffer);

        exit(EXIT_SUCCESS);
    }

    if (bits < 64) {
        fprintf(stderr, "Error: Too many hosts.\n");
        exit(EXIT_FAILURE);
    }

    uint64_t n = bits > 127 ? 0 : (uint64_t)~0 >> (bits - 64);
    for (uint64_t i = 0; i <= n; i++) {
        inet_ntop(AF_INET6, &start, addr_buffer, 48);
        printf("%s\n", addr_buffer);

        in6_addr_incr_pow2(&start, 0);
    }

    exit(EXIT_SUCCESS);
}
