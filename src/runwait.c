#include "common.h"
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void usage(const char *a){fprintf(stderr,"Usage: %s <cmd> [args]\n",a); exit(1);}
static double d(struct timespec a, struct timespec b){
 return (b.tv_sec-a.tv_sec)+(b.tv_nsec-a.tv_nsec)/1e9;}

int main(int c,char**v){
 if(c<2) usage(v[0]);
 
 struct timespec start,end;
 clock_gettime(CLOCK_MONOTONIC,&start);
 
 pid_t pid=fork();
 if(pid<0) DIE("fork");
 if(pid==0){
  //child process
  execvp(v[1],&v[1]);
  DIE("execvp"); //only reached if execv fails
 }
 
 int status;
 if(waitpid(pid,&status,0)<0) DIE("waitpid");  //parent waits for child
 
 clock_gettime(CLOCK_MONOTONIC,&end);
 double elapsed=d(start,end);
 
 //check child exit status
 if(WIFEXITED(status)){
  int exitcode=WEXITSTATUS(status);
  printf("pid=%d elapsed=%.3f exit=%d\n",pid,elapsed,exitcode);
 }else if(WIFSIGNALED(status)){
  int sig=WTERMSIG(status);
  printf("pid=%d elapsed=%.3f signal=%d\n",pid,elapsed,sig);
 }
 
 return 0;
}
