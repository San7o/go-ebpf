//go:build ignore

#include <sys/syscall.h>   // SYS_ definitions
#include <linux/openat2.h> // open_how struct
#include <unistd.h>        // syscall function
#include <fcntl.h>         // file flags
#include <stdio.h>         // perror

int main(void)
{
  int fd;

  //long syscall(SYS_openat2, int dirfd, const char *pathname,
  //             struct open_how *how, size_t size);
  struct open_how how = {
    .flags = O_RDONLY
    // .mode = 0
    // .resolve = 0
  };
  fd = syscall(SYS_openat2, AT_FDCWD, "./secret/password.txt", &how, sizeof(struct open_how));
  if (fd < 0)
  {
    perror("openat2");
  }

  if (syscall(SYS_close, fd) < 0)
  {
    perror("close");
  }

  return 0;
}
