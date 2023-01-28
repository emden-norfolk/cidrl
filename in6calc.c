#include <arpa/inet.h>

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
