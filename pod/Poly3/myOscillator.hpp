
#pragma once
#ifndef MY_OSCILLATOR_H
#define MY_OSCILLATOR_H

#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

class myOscillator : public daisysp::Oscillator {

public:
    myOscillator(){};
    ~myOscillator(){};

    void Process( AudioHandle::OutputBuffer buffer, size_t count );

private:
    uint8_t waveform_;

};

class myVoice : public myOscillator, public daisysp::AdEnv {

public:
    myVoice(){};
    ~myVoice(){};

    void Init( float samplerate );
    void Process( AudioHandle::OutputBuffer buffer, size_t count  );

};



#endif

