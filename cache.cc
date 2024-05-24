#include "common.h"
#include <inttypes.h>
#include <math.h>
#include "cache.h"

namespace CacheSim{

    Cache::Cache(int total_size,int cache_line_size,int associativity_width){
        index_num = total_size*1024/cache_line_size/associativity_width;
        associativity  = associativity_width;
        line_size = cache_line_size;
        index_bit = (uint32_t)log2(index_num);
        tag_bit   = 32-(uint32_t)log2(line_size)-index_bit;
        cache_tagv.resize(index_num, std::vector<uint32_t>(associativity, 0xffffffff));
        cache_dirty.resize(index_num, std::vector<bool>(associativity, false));
        //////////////////////////////////////////
        for(int i=0;i<32;i++){
            if(i<log2(line_size)||i>=log2(line_size)+log2(index_num)){
            index_mask &= ~(1<<i);
            }
            else{
            index_mask |= (1<<i);
            } 
        }
        printf("Cache info\n \
        total size:%dk\n \
        Cache line size:%dB\n \
        Associativity:%d\n \
        index num per way:%d\n \
        tag_bit:%d\n \
        index_bit:%d\n",total_size,cache_line_size,associativity_width,index_num,tag_bit,index_bit);
    }
    Select::Select(int total_size,int cache_line_size,int associativity_width,ReplacementPolicy policy) {
        switch (policy) {
            case ReplacementPolicy::LRU:
                printf("ReplacementPolicy->LRU\n");
                cacheSim = new LRU(total_size,cache_line_size,associativity_width);
                break;
            case ReplacementPolicy::SRRIP:
            printf("ReplacementPolicy->SRRIP\n");
                cacheSim = new SRRIP(total_size,cache_line_size,associativity_width);
                break;
        }
    }

    void Select::read(uint32_t addr){
        cacheSim->cache_read(addr);
    };
    void Select::write(uint32_t addr){
        cacheSim->cache_write(addr);
    };
    void Select::display(void){
        cacheSim->display_statistic();
    }; 
}