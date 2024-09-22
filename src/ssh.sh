#!/bin/bash

scp part2/cat/s21_cat server@10.10.0.2:~/
scp part2/grep/s21_grep server@10.10.0.2:~/
ssh server@10.10.0.2 "echo 123 | sudo -S mv s21_cat /usr/local/bin"
ssh server@10.10.0.2 "echo 123 | sudo -S mv s21_grep /usr/local/bin"
