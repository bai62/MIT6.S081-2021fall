#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

void pm(int pr){
    int t,x;
    if(!read(pr,&t,4)){
        exit(0);
    }

    int p[2];
    pipe(p);
    fprintf(1,"prime %d\n",t);

    if(fork()==0){
        close(p[1]);
        close(pr);
        pm(p[0]);
    }else{
        while(read(pr,&x,4)){
            if(x%t!=0)
                write(p[1],&x,4);
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}

int main(int argc,char *argv[])
{
    int p[2];
    pipe(p);

    if(fork()==0){
        close(p[1]);
        pm(p[0]);
    }else{
        for(int i=2;i<=35;i++)
            write(p[1],&i,4);
        close(p[1]);
        wait(0);
    }
    exit(0);
}
