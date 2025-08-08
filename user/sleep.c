#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
 if(argc < 2)
 {
  fprintf(2,"Usage: sleep seconds\n");
  exit(1);
 }
 int t = atoi(argv[1]);
 if( t <= 0)
  {
    write(2,"ugment is error!",1);
    exit(0);
  }
 sleep(t);
 exit(0);
}
