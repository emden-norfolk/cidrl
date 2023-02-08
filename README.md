# `cidrl` — CIDR List

**CIDRL** lists all IP addresses within a CIDR block (IPv4 and IPv6.) Supports subnetting.

Classless Inter-Domain Routing (CIDR; *[rfc1519](https://tools.ietf.org/html/rfc1519)*) is a strategy for assigning and routing Internet Protocol (IP) addresses.

## Installation

```console
$ git clone git@github.com:emden-norfolk/cidrl.git
$ cd cidrl/
$ make
$ sudo make install
```

## Usage

### `cidrl` for IPv4

Analyse and list all hosts in a /29:

```console
$ man cidrl
$ cidrl -a 192.168.50.16/29
Gateway:    192.168.50.16
Broadcast:  192.168.50.23
Netmask:    255.255.255.248
Hosts:      8
$ cidrl 192.168.50.16/29
192.168.50.16
192.168.50.17
192.168.50.18
192.168.50.19
192.168.50.20
192.168.50.21
192.168.50.22
192.168.50.23
```

Split a /24 into /28:

```console
$ cidrl -s28 10.0.0.0/24
10.0.0.0/28
10.0.0.16/28
10.0.0.32/28
10.0.0.48/28
10.0.0.64/28
10.0.0.80/28
10.0.0.96/28
10.0.0.112/28
10.0.0.128/28
10.0.0.144/28
10.0.0.160/28
10.0.0.176/28
10.0.0.192/28
10.0.0.208/28
10.0.0.224/28
10.0.0.240/28
```

See [`examples/host_exists_in_network.sh`](examples/host_exists_in_network.sh) for an example on how to use the `-e` option to programmatically check if a host exists within a given network. Alternatively, use `cidrl -ae <host> <network>`.

### `cidrl6` for IPv6

Split an IPv6 /60 into /64:

```console
$ man cidrl6
$ cidrl6 -s64 fa01:0:0:ffa0::/60
fa01:0:0:ffa0::/64
fa01:0:0:ffa1::/64
fa01:0:0:ffa2::/64
fa01:0:0:ffa3::/64
fa01:0:0:ffa4::/64
fa01:0:0:ffa5::/64
fa01:0:0:ffa6::/64
fa01:0:0:ffa7::/64
fa01:0:0:ffa8::/64
fa01:0:0:ffa9::/64
fa01:0:0:ffaa::/64
fa01:0:0:ffab::/64
fa01:0:0:ffac::/64
fa01:0:0:ffad::/64
fa01:0:0:ffae::/64
fa01:0:0:ffaf::/64
```

Analyse an IPv6 network and check if a given host exists within it:

```console
$ cidrl6 -ae fa01:0:0:ffb0:3::ae90 fa01:0:0:ffa0::/64
Network:    fa01:0:0:ffa0::
Range-end:  fa01::ffa0:ffff:ffff:ffff:ffff
Netmask:    ffff:ffff:ffff:ffff::

The host fa01::ffb0:3:0:0:ae90 does not exist within this network.
```

## References

The following manual pages are helpful for understanding the implementation of CIDRL:

 * [`getopt.3`](https://man7.org/linux/man-pages/man3/getopt.3.html)
 * [`scanf.3`](https://man7.org/linux/man-pages/man3/scanf.3.html)
 * [`byteorder.3`](https://man7.org/linux/man-pages/man3/byteorder.3.html)
 * [`atoi.3`](https://man7.org/linux/man-pages/man3/atoi.3.html)
 * [`inet.3`](https://man7.org/linux/man-pages/man3/inet.3.html)
 * [`inet_pton.3`](https://man7.org/linux/man-pages/man3/inet_pton.3.html)
 * [`inet_ntop.3`](https://man7.org/linux/man-pages/man3/inet_ntop.3.html)
 * [`ipv6.7`](https://man7.org/linux/man-pages/man7/ipv6.7.html)

Further reading:

 * Grothof, C. (2008), [*Migrating Code to IPv6
the good, the bad and the ugly*](https://grothoff.org/christian/rmv608.pdf), Colorado Research Institute for Security and Privacy.

See also:

 * [Nmap](https://nmap.org/)

## Author

2016 (c) Damien Bezborodov
