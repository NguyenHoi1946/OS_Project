#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  char *nargv[32];

  if(argc < 3){
    fprintf(2, "Usage: trace mask command\n");
    exit(1);
  }

  if(trace(atoi(argv[1])) < 0){
    fprintf(2, "trace: failed\n");
    exit(1);
  }

  for(i = 2; i < argc && i - 2 < 31; i++){
    nargv[i - 2] = argv[i];
  }
  nargv[i - 2] = 0;

  exec(nargv[0], nargv);
  fprintf(2, "exec %s failed\n", nargv[0]);
  exit(1);
}