

#include "daisy_pod.h"
#include "daisysp.h"

#include "oscillator.hpp"

using namespace daisy;
using namespace daisysp;

void oscillator::Process( AudioHandle::OutputBuffer buffer, size_t count ){

    for( size_t i=0; i<count; i++ ){
        const float sample = daisysp::Oscillator::Process();
        buffer[0][i] = sample;
        buffer[1][i] = sample; 
    }
    
};
