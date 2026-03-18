#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[512];
  char *xargv[MAXARG];
  int i, j;
  int base_argc;

  if(argc < 2){
    fprintf(2, "usage: xargs command [args ...]\n");
    exit(1);
  }

  base_argc = argc - 1;
  if(base_argc + 1 >= MAXARG){
    fprintf(2, "xargs: too many arguments\n");
    exit(1);
  }

  for(i = 1; i < argc; i++){
    xargv[i - 1] = argv[i];
  }

  j = 0;
  while(read(0, &buf[j], 1) == 1){
    if(buf[j] == '\n'){
      buf[j] = 0;

      if(j > 0){
        xargv[base_argc] = buf;
        xargv[base_argc + 1] = 0;

        if(fork() == 0){
          exec(xargv[0], xargv);
          fprintf(2, "xargs: exec %s failed\n", xargv[0]);
          exit(1);
        }
        wait(0);
      }

      j = 0;
    } else {
      j++;
      if(j >= sizeof(buf) - 1){
        fprintf(2, "xargs: input too long\n");
        exit(1);
      }
    }
  }

  if(j > 0){
    buf[j] = 0;
    xargv[base_argc] = buf;
    xargv[base_argc + 1] = 0;

    if(fork() == 0){
      exec(xargv[0], xargv);
      fprintf(2, "xargs: exec %s failed\n", xargv[0]);
      exit(1);
    }
    wait(0);
  }

  exit(0);
}