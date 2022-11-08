#!/bin/bash

ls -l -S $(du -b -d1 -S /usr | sort -n -r | head -1 | cut -f 2) | egrep "^-" | less