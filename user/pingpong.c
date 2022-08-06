#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

int main(int argc,char *argv[])
{
    int p[2];
    pipe(p);
    char str[5];

    if(fork()==0){
        read(p[0],str,4);
        fprintf(1,"%d: received %s\n",getpid(),str);
        write(p[1],"pong",4);
    }else{
        write(p[1],"ping",4);
        wait(0);
        read(p[0],str,4);
        fprintf(1,"%d: received %s\n",getpid(),str);
    }
    exit(0);
}
