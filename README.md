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

## Author

2016 (c) Damien Bezborodov
