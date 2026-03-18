#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void
print_indent(int depth)
{
  for(int i = 0; i < depth; i++)
    printf("  ");
}

char*
base_name(char *path)
{
  char *p;

  for(p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  return p + 1;
}

void
print_entry(char *path, int depth, int isdir)
{
  if(depth == 0){
    if(isdir){
      if(strcmp(path, "/") == 0)
        printf("/\n");
      else
        printf("%s/\n", path);
    } else {
      printf("%s\n", path);
    }
    return;
  }

  print_indent(depth);
  if(isdir)
    printf("%s/\n", base_name(path));
  else
    printf("%s\n", base_name(path));
}

void
tree(char *path, int depth)
{
  int fd;
  struct stat st;
  struct dirent de;
  char buf[512];
  char *p;

  if(stat(path, &st) < 0){
    fprintf(2, "tree: cannot stat %s\n", path);
    if(depth == 0)
      exit(1);
    return;
  }

  print_entry(path, depth, st.type == T_DIR);

  if(st.type != T_DIR)
    return;

  fd = open(path, 0);
  if(fd < 0){
    fprintf(2, "tree: cannot open %s\n", path);
    if(depth == 0)
      exit(1);
    return;
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
    fprintf(2, "tree: path too long %s\n", path);
    close(fd);
    if(depth == 0)
      exit(1);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  if(*(p - 1) != '/')
    *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if(strcmp(p, ".") == 0 || strcmp(p, "..") == 0)
      continue;

    tree(buf, depth + 1);
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc > 2){
    fprintf(2, "usage: tree [directory]\n");
    exit(1);
  }

  if(argc == 1)
    tree(".", 0);
  else
    tree(argv[1], 0);

  exit(0);
}