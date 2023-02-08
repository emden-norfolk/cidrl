#!/bin/bash

network="fa01:0:0:ffa0::/64"
hosts="fa01::ffa0:b:20:0:3d44 fa01:0:0:ffb0:3::ae90"

for host in $hosts; do

    cidrl6 -e$host $network
    case $? in
        64)
            printf "The host \033[1m$host\033[0m \033[0;31mdoes not exist\033[0m in the network \033[1m$network\033[0m.\n"
            ;;
        1)
            exit 1
            ;;
        0)
            printf "The host \033[1m$host\033[0m \033[0;32mexists\033[0m in the network \033[1m$network\033[0m.\n"
            ;;
    esac
done
