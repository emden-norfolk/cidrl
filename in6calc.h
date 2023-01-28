void in6_addr_mask(struct in6_addr *mask, uint8_t bits);

void in6_addr_start(struct in6_addr *start, struct in6_addr *addr, struct in6_addr *mask);

void in6_addr_end(struct in6_addr *end, struct in6_addr *addr, struct in6_addr *mask);

void in6_addr_add(struct in6_addr *addr, uint8_t addend, uint8_t i);

void in6_addr_increment(struct in6_addr *addr);

void in6_addr_iter(struct in6_addr *addr, uint8_t subnet);
