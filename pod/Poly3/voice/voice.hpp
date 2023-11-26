
#pragma once
#ifndef MY_VOICE_H
#define MY_VOICE_H

#include "daisy_pod.h"
#include "daisysp.h"
#include "dynamics/envelope.hpp"
#include "osc/oscillator.hpp"

using namespace daisy;
using namespace daisysp;

class voice : public oscillator, public envelope {

public:
    voice(){};
    ~voice(){};

    void Init( float samplerate );
    void Process( AudioHandle::OutputBuffer buffer, size_t count  );
    void ProcessAdd( AudioHandle::OutputBuffer buffer, size_t count  );

};

#endif