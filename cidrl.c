/*
 * cidrl
 *
 * Lists all IP addresses within a CIDR block.
 *
 * To compile:
 * gcc -o cidrl cidrl.c -Wall
 *
 * Usage:
 * ./cidrl 192.168.0.1/28
 *
 * @author Damien Bezborodov
 * @link https://github.com/emden-norfolk/cidrl
 */

#include <stdio.h>
#include <arpa/inet.h>

int main( int argc, char **argv ) {
    char network[15];
    struct in_addr addr;
    int mask, hladdr, hladdr_start, hladdr_end, i;
    unsigned int bits;

    if (argc != 2) {
        return 1; // Not an argument.
    }

    sscanf(argv[1], "%[^/]/%u", network, &bits);
    if (inet_aton(network, &addr) == 0) {
        return 2; // Invalid IP address.
    }

    if (bits > 32) {
        return 3;
    }

    if (bits == 32) {
        printf("%s\n", inet_ntoa(addr));
        return 0;
    }

    mask = ~(0xFFFFFFFF >> bits);

    hladdr = ntohl(addr.s_addr);
    hladdr_start = hladdr & mask;
    hladdr_end = (hladdr & mask) | ~mask;

    for (i = hladdr_start; i <= hladdr_end; i++) {
        addr.s_addr = htonl(i);
        printf("%s\n", inet_ntoa(addr));
    }

    return 0;
}
