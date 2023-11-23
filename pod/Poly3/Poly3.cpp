#include "daisy_pod.h"
#include "daisysp.h"
#include "dev/oled_ssd130x.h"

#include "voice.hpp"
#include "voice_square.hpp"
#include "Poly3.hpp"

#define CPU_LOAD_UPDATE_INTERVAL	500000UL		// Units of 1us

using namespace daisy;
using namespace daisysp;

DaisyPod hw;
CpuLoadMeter CpuLoad;

OledDisplay<SSD130x4WireSpi128x64Driver> display;

DaisySeed *myDaisy;
daisysp::Oscillator osc;

// null edit

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	CpuLoad.OnBlockStart();

	hw.ProcessAllControls();
	for (size_t i = 0; i < size; i++)
	{
        // The oscillator's Process function synthesizes, and
        // returns the next sample.
        float sine_signal = osc.Process();
        out[0][i] = sine_signal;
        out[1][i] = sine_signal;
	}

	CpuLoad.OnBlockEnd();
}

int main(void)
{

    //square *mySquare;

	hw.Init();
	myDaisy = &hw.seed;
	myDaisy->StartLog(true);

//    mySquare = new square;

//    voice *myVoice;
//    myVoice = mySquare;
//	myDaisy->PrintLine( "Voice name = %s\n", myVoice->name() );

	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	CpuLoad.Init( 48.0e3, 4 );

	hw.StartAdc();

	// We initialize the oscillator with the sample rate of the hardware
    // this ensures that the frequency of the Oscillator will be accurate.
    osc.Init(hw.AudioSampleRate());
	hw.StartAudio(AudioCallback);

	// Simple timer to print the CPU load every 100ms.
	uint32_t lastTime = System::GetUs();

	// int i=0;
	while(1) {
		uint32_t thisTime = System::GetUs();

		//myDaisy->PrintLine( "Last %lu: This %lu: diff %lu ", lastTime, thisTime, thisTime - lastTime );
		//FixedCapStr<100>str("");

		//myDaisy->PrintLine(str);
//		myDaisy->PrintLine( "Last %lu: This %lu: diff %lu ", lastTime, thisTime, thisTime - lastTime );


		if( ( thisTime - lastTime ) > CPU_LOAD_UPDATE_INTERVAL ) {	// 100ms
			
			myDaisy->PrintLine( "Last %lu: This %lu: diff %lu ", lastTime, thisTime, thisTime - lastTime );

			lastTime = System::GetUs();

			myDaisy->PrintLine( "New Last %lu", lastTime );
			myDaisy->PrintLine( "Diff %lu", System::GetUs() - thisTime );

			/*
			FixedCapStr<100>str("");
			str.AppendInt( i ++ );
			str.Append("  Avg = ");
			str.AppendFloat( CpuLoad.GetAvgCpuLoad() );
			str.Append( "  Max = ");
			str.AppendFloat( CpuLoad.GetMaxCpuLoad() );*/
			//myDaisy->PrintLine(str);
		}
	}
}
