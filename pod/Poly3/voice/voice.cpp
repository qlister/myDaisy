

#include "daisy_pod.h"
#include "daisysp.h"

#include "dynamics/envelope.hpp"
#include "osc/oscillator.hpp"
#include "voice.hpp"
#include "../Poly3.hpp"

using namespace daisy;
using namespace daisysp;



void voice::Process( AudioHandle::OutputBuffer buffer, size_t count ){

    oscillator::Process( buffer, count );
    envelope::Process( buffer, count );

}

void voice::ProcessAdd( AudioHandle::OutputBuffer buffer, size_t count ){

    float buf[ 2 * BLOCK_SIZE];
    float* in = buf;
//    oscillator::Process( in, count );
    envelope::Process( (AudioHandle::OutputBuffer)buf, count );
    float* out = *buffer;
    for( int i=0; i<count; i++ ){
        
    }


}

void voice::Init( float samplerate ){

    oscillator::Init( samplerate );
    envelope::Init( samplerate );

    // setup some defaults
    oscillator::SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
    oscillator::SetFreq(100.0f);
    oscillator::SetAmp(.5f);
    envelope::SetCurve(-15.0f);
    envelope::SetTime(ADENV_SEG_ATTACK, 0.002f);
    envelope::SetTime(ADENV_SEG_DECAY, 2.6f);
    envelope::SetMax(1.f);
    envelope::SetMin(0.f);
    
}