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

/*
 * Get the least significant 32 bits from a 128 bit address.
 *
 * (A quadlet is 32 bits.)
 *
 * E.g., fa01::7000:1a00 will return 70001a00.
 */
uint32_t in6_addr_least_signficiant_32bits(struct in6_addr *addr)
{
    uint32_t quadlet = 0;

    for (uint8_t i = 0; i < 4; i++) {
        quadlet += (1 << i * 8) * addr->s6_addr[15 - i];
    }

    return quadlet;
}

/**
 * Main
 */
int main(int argc, char **argv)
{
    char addr_buffer[48];
    struct in6_addr addr;
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

    // Check that a CIDR is given as an argument.
    if (argc <= optind) {
        fprintf(stderr, "Error: An IPv6 CIDR must be given as the first argument.\n");
        return 1;
    }

    // Scan the CIDR argument into separate network and subnet mask.
    sscanf(argv[optind], "%[^/]/%hhu", addr_buffer, &bits);

    // Parse the IPv6 string into an integer.
    if (inet_pton(AF_INET6, addr_buffer, &addr) == 0) {
        fprintf(stderr, "Error: Invalid IPv6 address given.\n");
        return 2;
    }
    if (bits > 128) {
        fprintf(stderr, "Error: Invalid subnet mask given.\n");
        return 3;
    }

    if (bits < 96) {
        fprintf(stderr, "Error: Subnet masks less than 96 bits are not currently supported by this programme.\n");
        // This would require working with 128 bit integers instead of 32 bit as per below.
        return 3;
    }

    if (bits == 128) {
        // Easy done! There is only one address if the mask is 128 bits. Let's print it now.
        inet_ntop(AF_INET6, &addr, addr_buffer, 48);
        printf("%s\n", addr_buffer);
        return 0;
    }

    // Extract the least significant 32 bits (quadlet) of the 128-bit address.
    uint32_t lst_sig_quadlet = in6_addr_least_signficiant_32bits(&addr);

    // Convert the prefix length (bits) into 32 bit binary mask.
    uint32_t mask = ~(0xFFFFFFFF >> (bits - 96));

    // Loop over and display every address in the given CIDR block.
    uint32_t end = (lst_sig_quadlet & mask) | ~mask;
    for (uint32_t i = lst_sig_quadlet & mask; i <= end; i++) {

        // Transplant the quadlet onto the full 128 bit address.
        for (uint8_t j = 0; j < 4; j++) {
            addr.s6_addr[15 - j] = *((uint8_t*)&i + j);
        }

        // Transfer the IPv6 address into the character buffer and print.
        inet_ntop(AF_INET6, &addr, addr_buffer, 48);
        printf("%s\n", addr_buffer);
    }

    return 0;
}
