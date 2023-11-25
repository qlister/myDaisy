

#include "daisy_pod.h"
#include "daisysp.h"

#include "myOscillator.hpp"

using namespace daisy;
using namespace daisysp;

void myVoice::Process( float **buffer, int count ){

    myOscillator::Process( buffer, count );

    for( int i=0; i<count; i++ ){
        float raw = **buffer;
        float outval = raw * daisysp::AdEnv::Process();
        **buffer = outval;
        (*buffer)++;
        **buffer = outval;
    }

}

void myVoice::Init( float samplerate ){

    myOscillator::Init( samplerate );
    daisysp::AdEnv::Init( samplerate );
    
}