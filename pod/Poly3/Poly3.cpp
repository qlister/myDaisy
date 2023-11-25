#include "daisy_pod.h"
#include "daisysp.h"
#include "dev/oled_ssd130x.h"

#include "voice.hpp"
#include "voice_square.hpp"
#include "Poly3.hpp"

#define CPU_LOAD_UPDATE_INTERVAL	500000UL		// Units of 1us

using namespace daisy;
using namespace daisysp;

DaisyPod myPod;
CpuLoadMeter CpuLoad;

OledDisplay<SSD130x4WireSpi128x64Driver> display;

DaisySeed *mySeed;
daisysp::Oscillator osc;
Svf        filt;

// null edit

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	CpuLoad.OnBlockStart();

	myPod.ProcessAllControls();
	for (size_t i = 0; i < size; i++)
	{
        // The oscillator's Process function synthesizes, and
        // returns the next sample.
        float sample = osc.Process();
		filt.Process(sample);
		sample = filt.Low();
		out[0][i] = sample;
        out[1][i] = sample;
	}
/*
	for (size_t i = 0; i < size; i++)
	{
        // The oscillator's Process function synthesizes, and
        // returns the next sample.
        float sine_signal = osc.Process();
        out[0][i] = sine_signal;
        out[1][i] = sine_signal;
	}
*/
	CpuLoad.OnBlockEnd();
}

// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();
            char        buff[512];
            sprintf(buff,
                    "Note Received:\t%d\t%d\t%d\r\n",
                    m.channel,
                    m.data[0],
                    m.data[1]);
            mySeed->PrintLine(buff);
            // This is to avoid Max/MSP Note outs for now..
            if(m.data[1] != 0)
            {
                p = m.AsNoteOn();
                osc.SetFreq(mtof(p.note));
                osc.SetAmp((p.velocity / 127.0f));
            }
        }
        break;
        case ControlChange:
        {
            ControlChangeEvent p = m.AsControlChange();
            switch(p.control_number)
            {
                case 1:
                    // CC 1 for cutoff.
                    filt.SetFreq(mtof((float)p.value));
                    break;
                case 2:
                    // CC 2 for res.
                    filt.SetRes(((float)p.value / 127.0f));
                    break;
                default: break;
            }
            break;
        }
        default: break;
    }
}



int main(void)
{

    //square *mySquare;

	myPod.Init();
	mySeed = &myPod.seed;
	mySeed->StartLog(true);

//    mySquare = new square;

//    voice *myVoice;
//    myVoice = mySquare;
//	mySeed->PrintLine( "Voice name = %s\n", myVoice->name() );

	myPod.SetAudioBlockSize(32); // number of samples handled per callback
	myPod.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	CpuLoad.Init( 48.0e3, 32 );

	myPod.StartAdc();

	// We initialize the oscillator with the sample rate of the hardware
    // this ensures that the frequency of the Oscillator will be accurate.
    osc.Init(myPod.AudioSampleRate());
    osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
    filt.Init(myPod.AudioSampleRate());

	myPod.StartAudio(AudioCallback);

	// Initialse a simple timer to print the CPU load every CPU_LOAD_UPDATE_INTERVAL microseconds.
	uint32_t lastTime = System::GetUs();
	while(1) {

		if( ( System::GetUs() - lastTime ) > CPU_LOAD_UPDATE_INTERVAL ) {	// 500ms
			lastTime = System::GetUs();

			// Print the load to the serial console
			FixedCapStr<100>str("Avg = ");
			str.AppendFloat( CpuLoad.GetAvgCpuLoad() );
			str.Append( "  Max = ");
			str.AppendFloat( CpuLoad.GetMaxCpuLoad() );
			mySeed->PrintLine(str);
		}

        myPod.midi.Listen();
        // Handle MIDI Events
        while(myPod.midi.HasEvents())
        {
            HandleMidiMessage(myPod.midi.PopEvent());
        }


	}
}
