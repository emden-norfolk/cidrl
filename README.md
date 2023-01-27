# `cidrl` — CIDR List

List all IP addresses within a CIDR block (IPv4 and IPv6.)

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

```console
$ man cidrl6
$ cidrl6 fa01::0000:0a10/124
fa01::a10
fa01::a11
fa01::a12
fa01::a13
fa01::a14
fa01::a15
fa01::a16
fa01::a17
fa01::a18
fa01::a19
fa01::a1a
fa01::a1b
fa01::a1c
fa01::a1d
fa01::a1e
fa01::a1f
```

## Author

2016 (c) Damien Bezborodov
