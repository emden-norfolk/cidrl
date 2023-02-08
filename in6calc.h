void in6_addr_mask(struct in6_addr *mask, uint8_t bits);

void in6_addr_network(struct in6_addr *network, struct in6_addr *addr, struct in6_addr *mask);

void in6_addr_end(struct in6_addr *end, struct in6_addr *addr, struct in6_addr *mask);

void in6_addr_incr_pow2(struct in6_addr *addr, uint8_t n);
