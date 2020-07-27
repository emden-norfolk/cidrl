# CIDRL

List all IP Addresses within a CIDR block (IPv4 and IPv6.)

## Installation

```
make
sudo make install
```

## Usage

IPv4

```
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

IPv6

```
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

2017 Damien Bezborodow
