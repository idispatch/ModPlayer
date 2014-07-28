#!/bin/sh
exec ./get.py -s `cat countries.list | sed 's/|.*//'` | tee all.json