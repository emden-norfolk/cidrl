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

#include "version.h"

#define EXIT_NOT_EXISTS 64

extern char *optarg;
extern int optind;

int main(int argc, char **argv)
{
    char addr_buffer[INET_ADDRSTRLEN];
    struct in_addr addr;
    uint32_t hladdr, hlmask, hlend, hlhost;
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
                if (inet_aton(optarg, &addr) == 0) {
                    fprintf(stderr, "Error: Invalid IPv4 address given.\n");
                    exit(EXIT_FAILURE);
                }
                hlhost = ntohl(addr.s_addr);
                exists = true;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    // Input, conversion, and error detection.
    if (argc - optind != 1) {
        fprintf(stderr, "Error: An IPv4 CIDR must be given as the first argument.\n");
        exit(EXIT_FAILURE);
    }
    char whitespace; // Detect trailing characters in sscanf.
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

    // Calculate IPv4 network range and mask.
    hlmask = bits ? ~0 << (32 - bits) : 0;
    hladdr = ntohl(addr.s_addr) & hlmask;
    hlend = hladdr | ~hlmask;

    // Split a CIDR block into smaller subnetworks.
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

    // Analyse a CIDR block.
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

        if (exists) {
            addr.s_addr = htonl(hlhost);
            printf("\nThe host %s %s within this network.\n",
                    inet_ntoa(addr),
                    (hlhost & hlmask) == hladdr ? "exists" : "does not exist");
        }

        exit(EXIT_SUCCESS);
    }

    // Check if host exists within network.
    if (exists) {
        if ((hlhost & hlmask) == hladdr)
            exit(EXIT_SUCCESS);
        else
            exit(EXIT_NOT_EXISTS);
    }

    // List all IP addresses in a CIDR block.
    for (uint32_t i = hladdr; i <= hlend; i++) {
        addr.s_addr = htonl(i);
        printf("%s\n", inet_ntoa(addr));
    }

    exit(EXIT_SUCCESS);
}
