/*
 * cidrl6
 *
 * Lists all IPv6 addresses within a CIDR block.
 *
 * Classless Inter-Domain Routing
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

/**
 * Calculate a 128-bit mask given a network prefix.
 */
void in6_addr_mask(struct in6_addr *mask, uint8_t bits)
{
    for (uint8_t i = 0; i < 16; i++) {
        mask->s6_addr[i] = ~0 << (8 - (bits > 8 ? 8 : bits));

        if (bits < 8) bits = 0;
        else bits -= 8;
    }
}

/**
 * Find the first address in a network given a mask.
 */
void in6_addr_start(struct in6_addr *start, struct in6_addr *addr, struct in6_addr *mask)
{
    for (uint8_t i = 0; i < 16; i++) {
        start->s6_addr[i] = addr->s6_addr[i] & mask->s6_addr[i];
    }
}

/**
 * Find the last address in a network given a mask.
 */
void in6_addr_end(struct in6_addr *end, struct in6_addr *addr, struct in6_addr *mask)
{
    for (uint8_t i = 0; i < 16; i++) {
        end->s6_addr[i] = (addr->s6_addr[i] & mask->s6_addr[i]) | ~mask->s6_addr[i];
    }
}

/**
 * Add an addend to an address, from position of the ith most significant bit.
 */
void in6_addr_add(struct in6_addr *addr, uint8_t addend, uint8_t i)
{
    uint16_t sum;

    while (addend) {
        sum = addr->s6_addr[i] + addend; // Perform addition with the addend.

        addr->s6_addr[i] = sum; // Save sum result to the byte.

        addend = sum >> 8; // Carry overflow to next significant byte.

        if (!--i) break; // Out of range.
    }
}

/**
 * Increment an address by one.
 */
void in6_addr_increment(struct in6_addr *addr)
{
    in6_addr_add(addr, 1, 15);
}

/**
 * Iterate over each network in the subnet.
 *
 * Increments by 2 to the power of subnet.
 */
void in6_addr_iter(struct in6_addr *addr, uint8_t subnet)
{
    uint8_t i, addend;

    // Find the byte where begin addition.
    subnet--;
    for (i = 0; i < 16; i++) {
        if (subnet < 8) break;
        subnet -= 8;
    }

    addend = 1 << (7 - subnet);
    in6_addr_add(addr, addend, i);
}

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
                exit(EXIT_FAILURE);
        }
    }

    // Check that a CIDR is given as an argument.
    if (argc <= optind) {
        fprintf(stderr, "Error: An IPv6 CIDR must be given as the first argument.\n");
        exit(EXIT_FAILURE);
    }

    // Scan the CIDR argument into separate network and network prefix.
    sscanf(argv[optind], "%[^/]/%hhu", addr_buffer, &bits);

    // Parse the IPv6 string into an integer.
    if (inet_pton(AF_INET6, addr_buffer, &addr) == 0) {
        fprintf(stderr, "Error: Invalid IPv6 address given.\n");
        exit(EXIT_FAILURE);
    }
    if (bits > 128) {
        fprintf(stderr, "Error: Invalid prefix bits given.\n");
        exit(EXIT_FAILURE);
    }

    if (bits == 128) {
        // There is only one address if the mask is 128 bits.
        inet_ntop(AF_INET6, &addr, addr_buffer, 48);
        printf("%s\n", addr_buffer);
        exit(EXIT_SUCCESS);
    }

    in6_addr_mask(&mask, bits);
    in6_addr_start(&start, &addr, &mask);
    in6_addr_end(&end, &addr, &mask);

    if (subnet) {
        if (subnet <= bits) {
            fprintf(stderr, "Error: Subnetwork must be greater than bits.\n");
            exit(EXIT_FAILURE);
        }
        if (subnet > 128) {
            fprintf(stderr, "Error: Subnetwork must not be greater than 128.\n");
            exit(EXIT_FAILURE);
        }
        if (subnet - bits >= 32) {
            fprintf(stderr, "Error: Difference between bits and subnet too big.\n");
            exit(EXIT_FAILURE);
        }

        uint32_t n = (1 << (subnet - bits));
        for (uint32_t i = 0; i < n; i++) {
            inet_ntop(AF_INET6, &start, addr_buffer, 48);
            printf("%s/%hhu\n", addr_buffer, subnet);

            in6_addr_iter(&start, subnet);
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

    if (bits <= 96) {
        fprintf(stderr, "Error: Too many.\n");
        exit(EXIT_FAILURE);
    }

    uint32_t n = (1 << (128 - bits));
    for (uint32_t i = 0; i < n; i++) {
        inet_ntop(AF_INET6, &start, addr_buffer, 48);
        printf("%s\n", addr_buffer);

        in6_addr_increment(&start);
    }

    exit(EXIT_SUCCESS);
}
