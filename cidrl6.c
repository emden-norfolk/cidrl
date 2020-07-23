/*
 * cidrl
 *
 * Lists all IPv6 addresses within a CIDR block.
 *
 * Usage:
 * ./cidrl6 2001:db8:0:8a2e::/124
 *
 * @author Damien Bezborodov
 * @link https://github.com/emden-norfolk/cidrl
 */

#include <stdio.h>
#include <arpa/inet.h>
#include <math.h>

int main( int argc, char **argv ) {
    char addr_buffer[48];
    struct in6_addr addr;
    unsigned int mask, hladdr, hladdr_start, hladdr_end;
    unsigned int trailing = 0;
    unsigned int bits;

    // Check that a CIDR is given as an argument.
    if (argc != 2) {
        fprintf(stderr, "Error: A CIDR must be given as the first argument.\n");
        return 1;
    }

    // Scan the CIDR argument into separate network and subnet mask.
    sscanf(argv[1], "%[^/]/%u", addr_buffer, &bits);


    // Parse the IPv6 string into an integer.
    if (inet_pton(AF_INET6, addr_buffer, &addr) == 0) {
        fprintf(stderr, "Error: Invalid IPv6 address given.\n");
        return 2;
    }
    if (bits > 128) {
        fprintf(stderr, "Error: Invalid subnet mask given.\n");
        return 3;
    }

    if (bits == 128) {
        inet_ntop(AF_INET6, &addr, addr_buffer, 48);
        printf("%s\n", addr_buffer);
        return 0;
    }

    for (int i = 0; i < 4; i++) {
        trailing += pow(2, i * 8) * addr.s6_addr[15 - i];
    }

    printf("Test: %u\n", trailing);

    /*

    mask = ~(0xFFFFFFFF >> bits);

    hladdr = ntohl(addr.s_addr);
    hladdr_start = hladdr & mask;
    hladdr_end = (hladdr & mask) | ~mask;

    for (i = hladdr_start; i <= hladdr_end; i++) {
        addr.s_addr = htonl(i);
        printf("%s\n", inet_ntop(addr));
    }
    */

    return 0;
}
