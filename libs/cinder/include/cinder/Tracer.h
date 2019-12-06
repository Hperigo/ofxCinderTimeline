//
//  Tracer.hpp
//  TimelineTracer
//
//  Created by Henrique on 12/6/19.
//

#ifndef Tracer_hpp
#define Tracer_hpp

#include <stdio.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

namespace cinder {
    
    enum TimelineItemType{
        eTimelineItem,
        eTween,
        eCue,
        eTimeline
    };
    
    struct TracerResult {
        TimelineItemType type;
        uint32_t session = 0;
        std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
    };
    
    struct TracerSession{
        
        static TracerSession* get(){
            static TracerSession t;
            return &t;
        }
        
        void beginRecording( const std::string& filename );
        void endRecording();

        void tween_start( uint32_t target, TimelineItemType type );
        void tween_end( uint32_t target );
    
        
        uint32_t getNewId(){
            uint32_t i = numberOfIds;
            numberOfIds += 1;
            return i;
        }
        
        void setSessionId( uint32_t i ){
            currentSessionId = i;
        }
        
        uint32_t getSessionId(){
            return currentSessionId;
        }
        
    protected:
        TracerSession(){ }
        void writeHeader();
        void writeFooter();
        
        std::map<size_t, TracerResult> results;
        
        uint32_t numberOfIds = 0;
        
        uint32_t currentSessionId = 0;
        
        std::ofstream outputStream;
        std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
    };

    
}

#endif /* Tracer_hpp */
