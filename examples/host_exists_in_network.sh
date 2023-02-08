#!/bin/bash
network="192.168.50.16/29"
hosts="192.168.50.15 192.168.50.16 192.168.50.17 192.168.50.24"
for host in $hosts; do
    exists=`cidrl -e$host $network`
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
