#!/bin/bash

scp /home/gitlab-runner/builds/jb2D3yF-/0/students/DO6_CICD.ID_356283/shinabzo_student.21_school.ru/DO6_CICD-1/src/part2/cat/s21_cat server@10.10.0.2:~/
scp /home/gitlab-runner/builds/jb2D3yF-/0/students/DO6_CICD.ID_356283/shinabzo_student.21_school.ru/DO6_CICD-1/src/part2/grep/s21_grep server@10.10.0.2:~/
ssh server@10.10.0.2 "echo 123 | sudo -S mv s21_cat /usr/local/bin"
ssh server@10.10.0.2 "echo 123 | sudo -S mv s21_grep /usr/local/bin"
