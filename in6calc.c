/*
 * IPv6 Address Calculations
 *
 * @author Damien Bezborodov
 * @link https://github.com/emden-norfolk/cidrl
 */

#include <arpa/inet.h>

/**
 * Calculate a 128-bit mask given a network prefix.
 */
void in6_addr_mask(struct in6_addr *mask, uint8_t bits)
{
    for (uint8_t i = 0; i < 16; i++) {
        mask->s6_addr[i] = bits ? ~0 << (8 - (bits > 8 ? 8 : bits)) : 0;

        if (bits < 8) bits = 0;
        else bits -= 8;
    }
}

/**
 * Calculate the first address in a network given a mask.
 */
void in6_addr_start(struct in6_addr *start, struct in6_addr *addr, struct in6_addr *mask)
{
    for (uint8_t i = 0; i < 16; i++) {
        start->s6_addr[i] = addr->s6_addr[i] & mask->s6_addr[i];
    }
}

/**
 * Calculate the last address in a network given a mask.
 */
void in6_addr_end(struct in6_addr *end, struct in6_addr *addr, struct in6_addr *mask)
{
    for (uint8_t i = 0; i < 16; i++) {
        end->s6_addr[i] = (addr->s6_addr[i] & mask->s6_addr[i]) | ~mask->s6_addr[i];
    }
}

/**
 * Increment an address by two to the power of n, where n is the exponent.
 *
 * To increment by 1, set n = 0. Remember that two to the power of zero is one.
 */
void in6_addr_incr_pow2(struct in6_addr *addr, uint8_t n)
{
    uint8_t i, addend;
    uint16_t sum; // 16-bit sum of two 8-bit integers.

    i = (127 - n) / 8; // The byte where begin addition.
    addend = 1 << (7 - ((127 - n) % 8)); // Scale the addend to i with the remainder.

    while (addend) {
        sum = addr->s6_addr[i] + addend; // Perform addition with the addend.
        addr->s6_addr[i] = sum; // Save the sum to the current byte.

        addend = sum >> 8; // Carry overflow to next significant byte.

        if (!--i) break; // Go to next significant byte (and check if out of range.)
    }
}
