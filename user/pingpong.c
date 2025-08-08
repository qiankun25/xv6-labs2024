#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
main()
{
 char buf[512];
 int fp[2],cp[2];// 父子写读管道
 pipe(fp);
 pipe(cp);
 if(fork() == 0)
{
 close(fp[1]);
 close(cp[0]);
 read(fp[0],buf,1);
 int cpid = getpid();
 printf("%d: received ping\n",cpid);
 write(cp[1],"",1);
 close(fp[0]);
 close(cp[1]);
}
 else
 {
  close(cp[1]);
  close(fp[0]);
  write(fp[1],"",1);
  read(cp[0],buf,1);
  int fpid = getpid();
  printf("%d: received pong\n",fpid);
  close(fp[1]);
  close(cp[0]);
 }
exit(0);
}
