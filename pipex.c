//
// Created by jtuomi on 12/6/24.
//
int main(int argc, char *argv[]) {
  int fd[2];

  if (argc < 2) {
    ft_printf("Usage: pipex <command>\n");
    return 1;
  }
  pid_t pid;
  if (pipe(fd) < 0) {
    perror("pipe");
    exit(1);
  }
}