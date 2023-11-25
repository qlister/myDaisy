

#include "daisy_pod.h"
#include "daisysp.h"

#include "myOscillator.hpp"

using namespace daisy;
using namespace daisysp;

void myOscillator::Process( float **buffer, int count ){

    for( int i=0; i<count; i++ ){
        float sample = Oscillator::Process();
        **buffer = sample;
        (*buffer)++;
        **buffer = sample;
        (*buffer)++;
    }
    
};
