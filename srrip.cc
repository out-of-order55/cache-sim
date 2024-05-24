#include "common.h"
#include <inttypes.h>
#include <math.h>
#include "cache.h"
// void mem_read(uintptr_t block_num, uint8_t *buf);
// void mem_write(uintptr_t block_num, const uint8_t *buf);
namespace CacheSim{

SRRIP::SRRIP(int total_size,int cache_line_size,int associativity_width):Cache(total_size,cache_line_size,associativity_width){
    uint32_t total_index = total_size*1024/cache_line_size/associativity_width;
	cache_rrpv.resize(total_index,std::vector<uint8_t>(associativity_width, false));
}

void SRRIP::cache_read(uint32_t addr) {
    uint32_t index =(addr&index_mask)>>((uint32_t)log2(line_size));
    uint32_t tag   =addr>>(32-tag_bit);
    for(int i=0;i<associativity;i++){
      auto tagv = cache_tagv[index][i];
      
      if(tagv==(tag)){
        hit++;
        HitUpdate(i,tag,index,true);
        return;
      }
    }
    MissUpdate(tag,index,true);
    miss++;
    
  }
void SRRIP::cache_write(uint32_t addr) {
    uint32_t index =(addr&index_mask)>>((uint32_t)log2(line_size));
    uint32_t tag   =addr>>(32-tag_bit);
    for(int i=0;i<associativity;i++){
      auto tagv = cache_tagv[index][i];
      if(tagv==(tag)){
        hit++;
        HitUpdate(i,tag,index,false);
        return;
      }
    }
    MissUpdate(tag,index,false);
    miss++;
  }
void SRRIP::HitUpdate(uint32_t way,uint32_t tag,uint32_t index,bool op){
    if(!op){
        cache_dirty[index][way]=true;
    }
    cache_rrpv[index][way]=0;
}
void SRRIP::MissUpdate(uint32_t tag,uint32_t index,bool op){
    uint8_t Check3=0;
    int i=0;
        while(Check3<3){
        for(i=0;i<associativity;i++){
            if(cache_rrpv[index][i]==3){
            Check3=cache_rrpv[index][i];
            break;
            }
            else{
            ++cache_rrpv[index][i];
            }
        }
        }
        // for(int j=0;j<associativity;j++){
        //   printf("miss tag:%x index:%x rrpv %d update:%d  rrpvid:%d\n",tag,index,cache_rrpv[index][j],i,j);
        // }
        // printf("\n");
        cache_rrpv[index][i]=2;
        cache_tagv[index][i]=tag;
}
void SRRIP::display_statistic(void) {
    int total=hit+miss;
    printf("total hit %d %.3f%%\ntotal miss %d %.3f%%\n",hit,100*(float)hit/(float)total,miss,100*(float)miss/(float)total);
}
}