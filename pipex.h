#ifndef PIPEX_H
#define PIPEX_H
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int open_pipex(const char *pipex_path);
void close_pipex(int fd);
int read_pipex(int fd, void *buf, size_t count);
int write_pipex(int fd, const void *buf, size_t count);
#endif