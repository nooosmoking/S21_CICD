#!/bin/bash
scp -i src/ssh ./src/cat/s21_cat root@192.168.1.3:/usr/local/bin 
scp -i src/ssh ./src/grep/s21_grep root@192.168.1.3:/usr/local/bin
