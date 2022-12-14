#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *tg;

char* getname(char *path){
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

int checkdot(char* path){
    char* buf=getname(path);

    if(strlen(path)==1)
      return 0;
    else if(buf[0]=='.'&&buf[1]==' ')
        return 1;
    else if(buf[0]=='.'&&buf[1]=='.'&&buf[2]==' ')
        return 1;
    else
       return 0;
}

void find(char *path){
  if(strcmp(getname(path),tg)==0){
      printf("%s\n",path);
  }

  if(checkdot(path)){
    return;
  }

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      find(buf);
    }
    break;
  }
  close(fd);
}

int main(int argc,char *argv[]){
    if(argc<3){
        printf("Usage: find . a.c\n");
        exit(1);
    }
    
    tg=argv[2];
    int ltg=strlen(tg);
    memset(tg+ltg,' ',DIRSIZ-ltg);
    find(argv[1]);

    exit(0);
}