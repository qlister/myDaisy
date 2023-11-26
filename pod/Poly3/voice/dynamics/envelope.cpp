
#include "daisy_pod.h"
#include "daisysp.h"

#include "envelope.hpp"

using namespace daisy;
using namespace daisysp;


void envelope::Process( AudioHandle::OutputBuffer buffer, size_t count ){

    float* ptr = *buffer; 
    for( size_t i=0; i<count; i++ ){
        const float sample = daisysp::AdEnv::Process();
        *ptr = sample;
        ptr++;
        *ptr = sample;
        ptr++; 
        //buffer[0][i] = sample;
        //buffer[1][i] = sample; 
    }
    
};
