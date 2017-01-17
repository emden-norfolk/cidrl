Note: This is written in C. There is also a PHP version under the `php` directory.

# cidrl
List All IP Addresses within a CIDR Block

## To Compile

```
gcc -o cidrl cidrl.c -Wall
```

## Usage

```
./cidrl 192.168.0.1/28
192.168.0.0
192.168.0.1
192.168.0.2
192.168.0.3
192.168.0.4
192.168.0.5
192.168.0.6
192.168.0.7
192.168.0.8
192.168.0.9
192.168.0.10
192.168.0.11
192.168.0.12
192.168.0.13
192.168.0.14
192.168.0.15
```
