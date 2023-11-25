

#include "daisy_pod.h"
#include "daisysp.h"

#include "myOscillator.hpp"

using namespace daisy;
using namespace daisysp;

void myVoice::Process( AudioHandle::OutputBuffer buffer, size_t count ){

    myOscillator::Process( buffer, count );

    for( int i=0; i<count; i++ ){
        float raw = **buffer;
        float outval = raw * daisysp::AdEnv::Process();
        buffer[0][i] = outval;
        buffer[1][i] = outval; 
    }

}

void myVoice::Init( float samplerate ){

    myOscillator::Init( samplerate );
    daisysp::AdEnv::Init( samplerate );
    
}