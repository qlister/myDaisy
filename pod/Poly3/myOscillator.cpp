

#include "daisy_pod.h"
#include "daisysp.h"

#include "myOscillator.hpp"

using namespace daisy;
using namespace daisysp;

void myOscillator::Process( AudioHandle::OutputBuffer buffer, size_t count ){

    for( size_t i=0; i<count; i++ ){
        float sample = Oscillator::Process();
        buffer[0][i] = sample;
        buffer[1][i] = sample; 
    }
    
};
