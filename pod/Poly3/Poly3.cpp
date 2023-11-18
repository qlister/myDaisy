#include "daisy_pod.h"
#include "daisysp.h"

//#include "subs.hxx"
#include "voice.hpp"
#include "voice_square.hpp"
#include "Poly3.hpp"

using namespace daisy;
using namespace daisysp;

DaisyPod hw;

DaisySeed *myDaisy;

// null edit

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	hw.ProcessAllControls();
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = in[0][i];
		out[1][i] = in[1][i];
	}
}

int main(void)
{

    square *mySquare;

	hw.Init();
	hw.seed.StartLog(true);
	myDaisy = &hw.seed;

    hw.seed.PrintLine( "Hello today\n" );

    mySquare = new square;

	hw.seed.PrintLine( "Name %s\n", mySquare->name() );

    voice *myVoice;
    myVoice = mySquare;
	hw.seed.PrintLine( "Voice name = %s\n", myVoice->name() );

	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAdc();
	hw.StartAudio(AudioCallback);
	while(1) {}
}
