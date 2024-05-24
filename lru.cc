#include "common.h"
#include <inttypes.h>
#include <math.h>
#include "cache.h"
// void mem_read(uintptr_t block_num, uint8_t *buf);
// void mem_write(uintptr_t block_num, const uint8_t *buf);
namespace CacheSim{

LRU::LRU(int total_size,int cache_line_size,int associativity_width):Cache(total_size,cache_line_size,associativity_width){
    uint32_t total_index = total_size*1024/cache_line_size/associativity_width;
	cache_lru.resize(total_index,std::vector<bool>(associativity_width-1, false));
}

void LRU::cache_read(uint32_t addr) {
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
void LRU::cache_write(uint32_t addr) {
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
void LRU::HitUpdate(uint32_t way,uint32_t tag,uint32_t index,bool op){
    if(!op){
		cache_dirty[index][way]=true;
    }
}
void LRU::MissUpdate(uint32_t tag,uint32_t index,bool op){

auto lru = cache_lru[index][0];

if(op==true){
	
	if(associativity==2){
	if(lru==true){
		cache_tagv[index][0]=tag;
		cache_dirty[index][0]=false;
		cache_lru[index][0]=false;
	}
	else{
		cache_tagv[index][1]=tag;
		cache_lru[index][0]=true;
		cache_dirty[index][1]=false;
	}
	}
	else if(associativity==4){
	lru=cache_lru[index][2];
	if(lru==true){
		lru=cache_lru[index][1];
		if(lru==true){
		cache_tagv[index][3]=tag;
		cache_dirty[index][3]=false;
		}
		else{
		cache_tagv[index][2]= tag;
		cache_dirty[index][2]=false;
		}
		cache_lru[index][2]=false;
		cache_lru[index][1]=!lru;
	}
	else {
		lru=cache_lru[index][0];
		if(lru==true){
		cache_tagv[index][1]=tag;
		cache_dirty[index][1]=false;
		}
		else{
		cache_tagv[index][0]= tag;
		cache_dirty[index][0]=false;
		}
		cache_lru[index][2]=false;
		cache_lru[index][0]=!lru;
	}
	}
	else if(associativity==8){
	lru=cache_lru[index][6];
	if(lru==true){
		lru=cache_lru[index][5];
		if(lru==true){
		lru=cache_lru[index][3];
		if(lru==true){
			cache_tagv[index][7]=tag;
			cache_dirty[index][7]=false;
		}
		else{
			cache_tagv[index][6]= tag;
			cache_dirty[index][6]=false;
		}
		cache_lru[index][6]=false;
		cache_lru[index][5]=!lru;
		cache_lru[index][3]=!lru;
		}
		else {
		lru=cache_lru[index][2];
		if(lru==true){
			cache_tagv[index][5]=tag;
			cache_dirty[index][5]=false;
		}
		else{
			cache_tagv[index][4]= tag;
			cache_dirty[index][4]=false;
		}
		cache_lru[index][6]=false;
		cache_lru[index][5]=!lru;
		cache_lru[index][2]=!lru;
		}
	}
	else{
		lru=cache_lru[index][4];
		if(lru==true){
		lru=cache_lru[index][1];
		if(lru==true){
			cache_tagv[index][3]=tag;
			cache_dirty[index][3]=false;
		}
		else{
			cache_tagv[index][2]= tag;
			cache_dirty[index][2]=false;
		}
		cache_lru[index][6]=false;
		cache_lru[index][4]=!lru;
		cache_lru[index][1]=!lru;
		}
		else {
		lru=cache_lru[index][0];
		if(lru==true){
			cache_tagv[index][1]=tag;
			cache_dirty[index][1]=false;
		}
		else{
			cache_tagv[index][0]= tag;
			cache_dirty[index][0]=false;
		}
		cache_lru[index][6]=false;
		cache_lru[index][4]=!lru;
		cache_lru[index][0]=!lru;
		}
	}
	}
	else{
	assert(0);
	}
}
else{
	if(associativity==2){
	if(lru==true){
		cache_tagv[index][0]=tag;
		cache_dirty[index][0]=false;
		cache_lru[index][0]=true;
	}
	else{
		cache_tagv[index][1]=tag;
		cache_lru[index][0]=true;
		cache_dirty[index][1]=true;
	}
	}
	else if(associativity==4){
	lru=cache_lru[index][2];
	if(lru==true){
		lru=cache_lru[index][1];
		if(lru==true){
		cache_tagv[index][3]=tag;
		cache_dirty[index][3]=true;
		}
		else{
		cache_tagv[index][2]= tag;
		cache_dirty[index][2]=true;
		}
		cache_lru[index][2]=false;
		cache_lru[index][1]=!lru;
	}
	else {
		lru=cache_lru[index][0];
		if(lru==true){
		cache_tagv[index][1]=tag;
		cache_dirty[index][1]=true;
		}
		else{
		cache_tagv[index][0]= tag;
		cache_dirty[index][0]=true;
		}
		cache_lru[index][2]=false;
		cache_lru[index][0]=!lru;
	}
	}
	else if(associativity==8){
	lru=cache_lru[index][6];
	if(lru==true){
		lru=cache_lru[index][5];
		if(lru==true){
		lru=cache_lru[index][3];
		if(lru==true){
			cache_tagv[index][7]=tag;
			cache_dirty[index][7]=true;
		}
		else{
			cache_tagv[index][6]= tag;
			cache_dirty[index][6]=true;
		}
		cache_lru[index][6]=false;
		cache_lru[index][5]=!lru;
		cache_lru[index][3]=!lru;
		}
		else {
		lru=cache_lru[index][2];
		if(lru==true){
			cache_tagv[index][5]=tag;
			cache_dirty[index][5]=true;
		}
		else{
			cache_tagv[index][4]= tag;
			cache_dirty[index][4]=true;
		}
		cache_lru[index][6]=false;
		cache_lru[index][5]=!lru;
		cache_lru[index][2]=!lru;
		}
	}
	else{
		lru=cache_lru[index][4];
		if(lru==true){
		lru=cache_lru[index][1];
		if(lru==true){
			cache_tagv[index][3]=tag;
			cache_dirty[index][3]=true;
		}
		else{
			cache_tagv[index][2]= tag;
			cache_dirty[index][2]=true;
		}
		cache_lru[index][6]=false;
		cache_lru[index][4]=!lru;
		cache_lru[index][1]=!lru;
		}
		else {
		lru=cache_lru[index][0];
		if(lru==true){
			cache_tagv[index][1]=tag;
			cache_dirty[index][1]=true;
		}
		else{
			cache_tagv[index][0]= tag;
			cache_dirty[index][0]=true;
		}
		cache_lru[index][6]=false;
		cache_lru[index][4]=!lru;
		cache_lru[index][0]=!lru;
		}
	}
	}
	else{
	assert(0);
	}
}
}
void LRU::display_statistic(void) {
	int total=hit+miss;
	printf("total hit %d %.3f%%\ntotal miss %d %.3f%%\n",hit,100*(float)hit/(float)total,miss,100*(float)miss/(float)total);
}
}