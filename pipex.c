#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pipex.h"
#include "libft/libft.h"
//
// Created by jtuomi on 12/6/24.
//
int main(int argc, char *argv[]) {
  int fd[2];

  if (argc < 5) {
    ft_printf("Usage: pipex <file> <command> <command> <file>\n");
    return 1;
  }
  pid_t pid;
  if (pipe(fd) < 0) {
    perror("pipe");
    exit(1);
  }
  pid = fork();
}