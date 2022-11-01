#!/bin/bash

printf "\n2-1-1:\n"
sort -d phone.book | column -t -s!

printf "\n2-1-2:\n"
sort -nr phone.book | column -t -s!

printf "\n2-1-3:\n"
cut -d! -f3 phone.book

printf "\n2-1-4:\n"
cat phone.book | tr '!' ':'

printf "\n2-1-5:\n"
head -1 phone.book

printf "\n2-1-6:\n"
grep -E "(Hans!|Wolf!)" phone.book