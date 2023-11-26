
#pragma once
#ifndef MY_OSCILLATOR_H
#define MY_OSCILLATOR_H

#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

class oscillator : public daisysp::Oscillator {

public:
    oscillator(){};
    ~oscillator(){};

    void Process( AudioHandle::OutputBuffer buffer, size_t count );

private:
    uint8_t waveform_;

};

#endif

