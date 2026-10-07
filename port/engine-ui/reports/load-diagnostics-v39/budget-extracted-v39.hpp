#pragma once
#include <chrono>
#include <cstdint>
    struct QaBudgetStopV39{const char* reason;};
    struct QaBudgetV39{
      std::chrono::steady_clock::time_point start=std::chrono::steady_clock::now();
      std::uint64_t units{};
      void check_graph(std::uint64_t floors,std::uint64_t nodes,std::uint64_t edges){
        if(floors>64)throw QaBudgetStopV39{"floor cap64"};
        if(nodes>4096)throw QaBudgetStopV39{"node cap4096"};
        if(edges>65536)throw QaBudgetStopV39{"edge cap65536"};
        check_time();
      }
      void check_time(){
        if(std::chrono::steady_clock::now()-start>=std::chrono::milliseconds(1000))
          throw QaBudgetStopV39{"elapsed cap1000ms"};
      }
      void step(){check_time();if(units>=25000)throw QaBudgetStopV39{"iteration cap25000"};++units;}
    };
