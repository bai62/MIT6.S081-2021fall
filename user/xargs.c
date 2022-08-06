#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include  "kernel/param.h"

#define BUF_SIZE 512

int main(int argc, char* argv[]){
    if(argc<2){
        printf("Usage: echo hello too | xargs echo bye\n");
        exit(1);
    }

    char* targv[MAXARG+1];
    char buf[BUF_SIZE];
    char* temp=buf;
    int cnt=0;
    for(int i=1;i<argc;i++)
        targv[cnt++]=argv[i];
    while(read(0,temp,1)){
        if(*temp=='\n'){
            if(fork()==0){
                *temp=0;
                targv[cnt]=buf;
                exec(targv[0],targv);
            }else{
                wait(0);
                temp=buf;
            }
        }else{
            temp++;
        }
    }
    exit(0);
}