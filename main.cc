#include "common.h"
#include <time.h>
#include <unistd.h>
#include "cache.h"

int total_size_width,associativity_width,total_size;
CacheSim::ReplacementPolicy  policy;
void pargs_analysis(char *argv[]){
  char *arg = argv[1];
  total_size=atoi(arg);
  arg = argv[2];
  total_size_width=atoi(arg);
  arg = argv[3];
  associativity_width=atoi(arg);
  arg = argv[4];
  policy=static_cast<CacheSim::ReplacementPolicy>(atoi(arg));
}

int main(int argc, char *argv[]) {
  pargs_analysis(argv);
  CacheSim::Select cacheSim(total_size,total_size_width,associativity_width,policy);

/////////////////working//////////////
  // FILE *file;
  // char line[100]; 
  // uint32_t addr;
  // file = fopen("nemu-log.txt", "r");
  // if (file == NULL) {
  //     perror("无法打开文件");
  //     return 1;
  // }
  // int i=0;
  // while (fgets(line, sizeof(line), file)) {
  //     sscanf(line, "%x", &addr);
  //     ICache.cache_read(addr);
  //     i++;
  // }
  // printf("%d\n",i);
  // fclose(file);
///////////////////////////////////////
  for(int i=0;i<=0x1f;i+=4){
    cacheSim.read(i);
  }
  cacheSim.display();
  return 0;
}
