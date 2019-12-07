//
//  Tracer.cpp
//  TimelineTracer
//
//  Created by Henrique on 12/6/19.
//

#include "Tracer.h"


using namespace cinder;

void TracerSession::beginRecording(const std::string &filename){
    outputStream.open( filename );
    writeHeader();
}


void TracerSession::endRecording(){
    writeFooter();
    outputStream.close();
}


void TracerSession::tween_start(uint32_t target, TimelineItemType type){
    TracerResult result;
    result.startTimePoint = std::chrono::high_resolution_clock::now();
    result.type = type;
    result.session = currentSessionId;
    results[target] = result;
}


void TracerSession::tween_end(uint32_t target){

        auto it=results.find ( target );
        assert( it != results.end() );
        auto result = it->second;
        
        auto endTimepoint = std::chrono::high_resolution_clock::now();
        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(result.startTimePoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
        long long duration = end - start;
        
        static bool first = true;
        if ( first == false ){
            outputStream << ",";
        }
        first = false;
        
        std::string cat;
        switch(result.type) {
            case TimelineItemType::eTween:
                cat = "Tween";
                break;
                
            case TimelineItemType::eTimeline:
                cat = "Timeline";
                break;
                
            case TimelineItemType::eTimelineItem:
                cat = "TimelineItem";
                
            case TimelineItemType::eCue:
                cat = "Cue";
                duration = 100000;
                break;
        }
        
        std::stringstream ss;
        ss << target;
        std::string name = ss.str();
        name = name + "-" + cat;
        
        outputStream << "{";
        outputStream << "\"cat\":\"" << cat << "\",";
        outputStream << "\"dur\":" << duration << ",";
        outputStream << "\"name\":\"" << name << "\",";
        outputStream << "\"ph\":\"X\",";
        outputStream << "\"pid\":" << result.session << ",";
        outputStream << "\"tid\":" << target << ",";
        outputStream << "\"ts\":" << start;
        outputStream << "}";
        
        results.erase ( it );
        outputStream.flush();
}


void TracerSession::writeHeader(){
    outputStream << "{\"otherData\": {},\"traceEvents\":[";
    outputStream.flush();
}

void TracerSession::writeFooter(){
    outputStream << "]}";
    outputStream.flush();
}
