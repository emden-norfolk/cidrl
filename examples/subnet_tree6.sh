#!/bin/bash
#
# CIDRL IPv6 Example
#
# Split a /60 into /64 then /68 with tree view.

print_subnet () {
    if [ "$2" = true ]; then
        echo -n "├── "
    else 
        echo -n "└── "
    fi

    echo $1

    local line2
    local sub2net
    for line2 in $(cidrl6 -s68 $subnet); do
        if [ -z "$sub2net" ]; then
            sub2net=$line2
            continue
        fi

        print_sub2net $sub2net $2 true

        sub2net=$line2
    done

    print_sub2net $sub2net $2 false
}

print_sub2net () {
    if [ "$2" = true ]; then
        echo -n "│   "
    else 
        echo -n "    "
    fi

    if [ "$3" = true ]; then
        echo -n "├── "
    else 
        echo -n "└── "
    fi

    echo $1
}

network="fa01:0:0:ffa0::/60"

echo $network

for line in $(cidrl6 -s64 $network); do
    if [ -z "$subnet" ]; then
        subnet=$line
        continue
    fi

    print_subnet $subnet true

    subnet=$line
done

print_subnet $subnet false
