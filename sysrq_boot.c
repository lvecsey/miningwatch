
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

  int fd = open("/proc/sysrq-trigger", O_WRONLY);

  ssize_t bytes_written;
  
  if (fd==-1) {
    perror("open");
    return -1;
  }

  bytes_written = write(fd, "b", 1);
  if (bytes_written != 1) {
    perror("write");
    return -1;
  }
  
  return 0;

}
