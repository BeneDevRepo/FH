#!/bin/bash

printf '\n2.2.1:\n'
egrep "^[A-Z][0-9]{4,},([A-Z-]*,){2}(AF|C|D|F|HD|P),[0-9]+(\.[0-9]+)?,[A-Z]+[0-9]+$" results.csv | wc -l

printf '\n2.2.2:\n'
cut -d, -f1 results.csv | sort -n | egrep "^[A-Z][0-9]{4,}"

printf '\n2.2.3:\n'
cut -d, -f2,3,4 results.csv | egrep ",[^,]*,HD.*"