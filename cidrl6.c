/*
 * cidrl
 *
 * Lists all IPv6 addresses within a CIDR block.
 *
 * Classless Inter-Domain Routing
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
        // TODO remove dependency on libm. Test first though.
        // quadlet += (1 << i * 8) * addr->s6_addr[15 - i];
        quadlet += pow(2, i * 8) * addr->s6_addr[15 - i];
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
    uint32_t bitmask;

    // Check that a CIDR is given as an argument.
    if (argc != 2) {
        fprintf(stderr, "Error: A CIDR must be given as the first argument.\n");
        return 1;
    }

    // Scan the CIDR argument into separate network and subnet mask.
    sscanf(argv[1], "%[^/]/%u", addr_buffer, &bitmask);

    // Parse the IPv6 string into an integer.
    if (inet_pton(AF_INET6, addr_buffer, &addr) == 0) {
        fprintf(stderr, "Error: Invalid IPv6 address given.\n");
        return 2;
    }
    if (bitmask > 128) {
        fprintf(stderr, "Error: Invalid subnet mask given.\n");
        return 3;
    }

    if (bitmask < 96) {
        fprintf(stderr, "Error: Subnet masks less than 96 bits are not currently supported by this programme.\n");
        // This would require working with 128 bit integers instead of 32 bit as per below.
        return 3;
    }

    if (bitmask == 128) {
        // Easy done! There is only one address if the mask is 128 bits. Let's print it now.
        inet_ntop(AF_INET6, &addr, addr_buffer, 48);
        printf("%s\n", addr_buffer);
        return 0;
    }

    // Extract the least significant 32 bits (quadlet) of the 128-bit address.
    // TODO Strange behaviour here. Re-examine and test this. Does the bitmask
    // cancel out the need for lst_sig_quadlet? Setting to zero has no maleffect.
    uint32_t lst_sig_quadlet = in6_addr_least_signficiant_32bits(&addr);

    // Convert 128 bit decimal bitmask number into 32 bit binary mask.
    uint32_t mask = ~(0xFFFFFFFF >> (bitmask - 96));

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
