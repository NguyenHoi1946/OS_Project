#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

struct procinfo {
  int pid;
  int ppid;
  int state;
  uint64 sz;
  char name[16];    
};

int
main(int argc, char *argv[])
{
  struct procinfo info;

  int pid = getpid();

  if(procinfo(pid, &info) < 0){
    printf("procinfo failed\n");
    exit(1);
  }

  printf("pid: %d\n", info.pid);
  printf("ppid: %d\n", info.ppid);
  printf("state: %d\n", info.state);
  printf("size: %ld\n", info.sz);
  printf("name: %s\n", info.name);

  exit(0);
}