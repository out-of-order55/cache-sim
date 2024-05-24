#ifndef __CACHE_H__
#define __CACHE_H__
#include <inttypes.h>
#include <list>
#include <iostream>
#include <vector>
namespace CacheSim{


    enum class ReplacementPolicy {
        LRU,
        SRRIP
    };
    class Cache{
    public:
        int hit=0;
        int miss=0;
        uint32_t tag_bit;
        uint32_t index_bit;
        uint32_t line_size;
        uint32_t associativity;
        uint32_t size;
        uint32_t index_num;
        uint32_t index_mask;
        virtual void cache_read(uint32_t addr)=0;
        virtual void cache_write(uint32_t addr)=0;
        virtual void display_statistic(void)=0;
    protected:
        Cache(int total_size,int cache_line_size,int associativity_width);
        std::vector<std::vector<uint32_t>> cache_tagv; // 2D vector for tags
        std::vector<std::vector<bool>> cache_dirty; // 2D vector for tags
        virtual  void HitUpdate(uint32_t way,uint32_t tag,uint32_t index,bool op)=0;
        virtual  void MissUpdate(uint32_t tag,uint32_t index,bool op)=0;
        // void LRU(uint32_t tag,uint32_t index,bool op);//PLRU
        // void SRRIP(uint32_t tag,uint32_t index,bool op,bool hit,uint8_t HitWay);
    };
    class LRU:public Cache{
        public:
            void cache_read(uint32_t addr);
            void cache_write(uint32_t addr);
            void display_statistic(void);
            std::vector<std::vector<bool>> cache_lru; //
            LRU(int total_size,int cache_line_size,int associativity_width);
            //
            //          0 
            //      4     
            //          1
            //  6
            //          2
            //      5
            //          3
            // 
            void HitUpdate(uint32_t way,uint32_t tag,uint32_t index,bool op);
            void MissUpdate(uint32_t tag,uint32_t index,bool op);
    };
    class SRRIP:public Cache{
        public:
            void cache_read(uint32_t addr);
            void cache_write(uint32_t addr);
            void display_statistic(void); 
            SRRIP(int total_size,int cache_line_size,int associativity_width);
            std::vector<std::vector<uint8_t>> cache_rrpv; //2 bit min->0 max->3
            void HitUpdate(uint32_t way,uint32_t tag,uint32_t index,bool op);
            void MissUpdate(uint32_t tag,uint32_t index,bool op);
    };
    class Select {
    private:
        Cache* cacheSim; 
    public:
        Select(int total_size,int cache_line_size,int associativity_width,ReplacementPolicy policy);
        ~Select() {
            delete cacheSim;
        }
        void read(uint32_t addr);
        void write(uint32_t addr);
        void display(void); 
    };
}

#endif
