#!/bin/bash

file="file"

cat $file | sed 'y/qwertyuiopasdfghjklzxcvbnm/QWERTYUIOPASDFGHJKLZXCVBNM'
