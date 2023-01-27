/*
 * cidrl6
 *
 * Lists all IPv6 addresses within a CIDR block.
 *
 * Classless Inter-Domain Routing
 *
 * TODO Implement option to break A CIDRL into smaller subclasses.
 * Probably more useful when dealing with IPv6.
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

void in6_addr_mask(struct in6_addr *addr, uint8_t bits)
{
    for (uint8_t i = 0; i < 16; i++) {
        addr->s6_addr[i] = ~0 << (8 - (bits > 8 ? 8 : bits));

        if (bits < 8) bits = 0;
        else bits -= 8;
    }
}

void in6_addr_start(struct in6_addr *start, struct in6_addr *addr, struct in6_addr *mask)
{
    for (uint8_t i = 0; i < 16; i++) {
        start->s6_addr[i] = addr->s6_addr[i] & mask->s6_addr[i];
    }
}

void in6_addr_end(struct in6_addr *end, struct in6_addr *addr, struct in6_addr *mask)
{
    for (uint8_t i = 0; i < 16; i++) {
        end->s6_addr[i] = (addr->s6_addr[i] & mask->s6_addr[i]) | ~mask->s6_addr[i];
    }
}

void in6_addr_iter(struct in6_addr *addr, uint8_t subnet)
{
    uint8_t i, addend;
    uint16_t sum;

    subnet--;
    for (i = 0; i < 16; i++) {
        if (subnet < 8) break;
        subnet -= 8;
    }

    addend = 1 << (7 - subnet);

    while (true) {
        sum = addr->s6_addr[i] + addend;
        //printf("%hhu + %hhu = %hu (%hhu)\n", addr->s6_addr[i], addend, sum, sum >> 8);

        addr->s6_addr[i] = sum;

        addend = sum >> 8; // Carry overflow to next byte.

        if (!--i || !addend) break;
    }
}

/**
 * Main
 */
int main(int argc, char **argv)
{
    char addr_buffer[48];
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
                exit(1);
        }
    }

    // Check that a CIDR is given as an argument.
    if (argc <= optind) {
        fprintf(stderr, "Error: An IPv6 CIDR must be given as the first argument.\n");
        return 1;
    }

    // Scan the CIDR argument into separate network and bits.
    sscanf(argv[optind], "%[^/]/%hhu", addr_buffer, &bits);

    // Parse the IPv6 string into an integer.
    if (inet_pton(AF_INET6, addr_buffer, &addr) == 0) {
        fprintf(stderr, "Error: Invalid IPv6 address given.\n");
        return 2;
    }
    if (bits > 128) {
        fprintf(stderr, "Error: Invalid prefix bits given.\n");
        return 3;
    }
    
    in6_addr_mask(&mask, bits);
    inet_ntop(AF_INET6, &mask, addr_buffer, 48);
    printf("Netmask: %s\n", addr_buffer);

    in6_addr_start(&start, &addr, &mask);
    inet_ntop(AF_INET6, &start, addr_buffer, 48);
    printf("Start: %s\n", addr_buffer);

    in6_addr_end(&end, &addr, &mask);
    inet_ntop(AF_INET6, &end, addr_buffer, 48);
    printf("End: %s\n", addr_buffer);

    uint8_t n = (1 << (subnet - bits));
    for (uint32_t i = 0; i < n; i++) {
        inet_ntop(AF_INET6, &start, addr_buffer, 48);
        printf("%s/%hhu\n", addr_buffer, bits);

        in6_addr_iter(&start, subnet);
    }

    return 0;
}
