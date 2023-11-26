

#include "daisy_pod.h"
#include "daisysp.h"

#include "dynamics/envelope.hpp"
#include "osc/oscillator.hpp"
#include "voice.hpp"

using namespace daisy;
using namespace daisysp;

void voice::Process( AudioHandle::OutputBuffer buffer, size_t count ){

    oscillator::Process( buffer, count );
    envelope::Process( buffer, count );

}

void voice::Init( float samplerate ){

    oscillator::Init( samplerate );
    envelope::Init( samplerate );
    
}