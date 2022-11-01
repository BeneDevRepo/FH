#!/bin/bash

ls -S $(du -b -d1 -S /usr | sort -n -r | head -1 | cut -f 2) | less