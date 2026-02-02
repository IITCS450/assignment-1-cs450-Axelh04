#include "common.h"
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
static void usage(const char *a){fprintf(stderr,"Usage: %s <pid>\n",a); exit(1);}
static int isnum(const char*s){for(;*s;s++) if(!isdigit(*s)) return 0; return 1;}
int main(int c,char**v){
 if(c!=2||!isnum(v[1])) usage(v[0]);
 
 char path[256];
 char state;
 int ppid;
 unsigned long utime,stime;
 int vmrss=0;
 
 //get state, ppid, utime, stime from stat
 snprintf(path,sizeof(path),"/proc/%s/stat",v[1]);
 FILE*f=fopen(path,"r");
 if(!f) DIE("fopen stat");
 
 int pid;
 char comm[256];
 if(fscanf(f,"%d %s %c %d",&pid,comm,&state,&ppid)<4){
  fclose(f);
  DIE_MSG("parse stat failed");
 }
 
 //skip to utime and stime
 unsigned long tmp;
 for(int i=0;i<9;i++){
  if(fscanf(f,"%lu",&tmp)!=1){
   fclose(f);
   DIE_MSG("parse stat skip");
  }
 }
 if(fscanf(f,"%lu %lu",&utime,&stime)!=2){
  fclose(f);
  DIE_MSG("parse stat times");
 }
 fclose(f);
 
 //get command name
 snprintf(path,sizeof(path),"/proc/%s/cmdline",v[1]);
 f=fopen(path,"r");
 if(!f) DIE("fopen cmdline");
 char cmdline[1024]={0};
 fread(cmdline,1,sizeof(cmdline)-1,f);
 fclose(f);
 
 //get vmrss from status
 snprintf(path,sizeof(path),"/proc/%s/status",v[1]);
 f=fopen(path,"r");
 if(!f) DIE("fopen status");
 char line[256];
 while(fgets(line,sizeof(line),f)){
  if(sscanf(line,"VmRSS: %d",&vmrss)==1) break;
 }
 fclose(f);
 
 //convert cpu ticks to seconds
 long ticks=sysconf(_SC_CLK_TCK);
 double cpu=(utime+stime)/(double)ticks;
 
 printf("PID:%s\n",v[1]);
 printf("State:%c\n",state);
 printf("PPID:%d\n",ppid);
 printf("Cmd:%s\n",cmdline);
 printf("CPU:%d %.3f\n",(int)(utime+stime),cpu);
 printf("VmRSS:%d\n",vmrss);
 
 return 0;
}
