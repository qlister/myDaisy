#pragma once
#ifndef MY_ENVELOPE_H
#define MY_ENVELOPE_H

#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

class envelope : public daisysp::AdEnv {

public:
    envelope(){};
    ~envelope(){};

    void Process( AudioHandle::OutputBuffer buffer, size_t count );

private:
    

};


#endif
