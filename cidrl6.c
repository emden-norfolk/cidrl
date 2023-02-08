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
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "in6calc.h"

#include "version.h"

#define EXIT_NOT_EXISTS 64

extern char *optarg;
extern int optind;

int main(int argc, char **argv)
{
    char addr_buffer[INET6_ADDRSTRLEN];
    struct in6_addr addr, mask, network, end, host, host_net;
    uint8_t bits;

    // Options.
    uint8_t subnet = 0;
    bool analyse = false;
    bool exists = false;
    int opt;
    while ((opt = getopt(argc, argv, "vas:e:")) != -1) {
        switch (opt) {
            case 'v':
                printf("%s\n", version());
                exit(EXIT_SUCCESS);
                break;
            case 's':
                subnet = atoi(optarg);
                break;
            case 'a':
                analyse = true;
                break;
            case 'e':
                if (inet_pton(AF_INET6, optarg, &host) == 0) {
                    fprintf(stderr, "Error: Invalid IPv6 address given.\n");
                    exit(EXIT_FAILURE);
                }
                exists = true;
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
    char whitespace; // Detect trailing characters in sscanf.
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

    // Calculate IPv6 network range and mask.
    in6_addr_mask(&mask, bits);
    in6_addr_network(&network, &addr, &mask);
    in6_addr_end(&end, &network, &mask);

    // Split a IPv6 CIDR block into smaller subnetworks.
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
            inet_ntop(AF_INET6, &network, addr_buffer, INET6_ADDRSTRLEN);
            printf("%s/%hhu\n", addr_buffer, subnet);

            in6_addr_incr_pow2(&network, 128 - subnet);
        }

        exit(EXIT_SUCCESS);
    }

    // Analyse an IPv6 CIDR block.
    if (analyse) {
        inet_ntop(AF_INET6, &network, addr_buffer, INET6_ADDRSTRLEN);
        printf("Network:    %s\n", addr_buffer);

        inet_ntop(AF_INET6, &end, addr_buffer, INET6_ADDRSTRLEN);
        printf("Range-end:  %s\n", addr_buffer);

        inet_ntop(AF_INET6, &mask, addr_buffer, INET6_ADDRSTRLEN);
        printf("Netmask:    %s\n", addr_buffer);

        if (exists) {
            in6_addr_network(&host_net, &host, &mask);
            inet_ntop(AF_INET6, &host, addr_buffer, INET6_ADDRSTRLEN);
            printf("\nThe host %s %s within this network.\n",
                    addr_buffer,
                    memcmp(&network.s6_addr, &host_net.s6_addr, sizeof(network.s6_addr)) == 0
                            ? "exists" : "does not exist");
        }

        exit(EXIT_SUCCESS);
    }

    // Check if host exists within network.
    if (exists) {
        in6_addr_network(&host_net, &host, &mask);
        if (memcmp(&network.s6_addr, &host_net.s6_addr, sizeof(network.s6_addr)) == 0)
            exit(EXIT_SUCCESS);
        else
            exit(EXIT_NOT_EXISTS);
    }

    // List all IPv6 addresses in a CIDR block.
    if (bits < 64) {
        fprintf(stderr, "Error: Too many hosts.\n");
        exit(EXIT_FAILURE);
    }
    uint64_t n = bits > 127 ? 0 : (uint64_t)~0 >> (bits - 64);
    for (uint64_t i = 0; i <= n; i++) {
        inet_ntop(AF_INET6, &network, addr_buffer, INET6_ADDRSTRLEN);
        printf("%s\n", addr_buffer);

        in6_addr_incr_pow2(&network, 0);
    }

    exit(EXIT_SUCCESS);
}
