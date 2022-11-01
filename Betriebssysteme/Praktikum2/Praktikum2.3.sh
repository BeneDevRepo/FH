#!/bin/bash

cut -d: -f5 /etc/passwd | egrep "[^ ]+" | sort -d > benutzer.txt