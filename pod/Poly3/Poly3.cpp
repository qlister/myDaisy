#include "daisy_pod.h"
#include "daisysp.h"
#include "dev/oled_ssd130x.h"

#include "Poly3.hpp"
#include "voice/voice.hpp"
#include "pod_hw.hpp"

#define CPU_LOAD_UPDATE_INTERVAL	500000UL		// Units of 1us


#define SKINNY_ROW_HEIGHT 13
#define DEFAULT_FONT Font_7x10
#define SMALL_FONT Font_6x8


using namespace daisy;
using namespace daisysp;

DaisyPod myPod;
CpuLoadMeter CpuLoad;

OledDisplay<SSD13054WireSpi132x64Driver> display;

//voice thisVoice;
voice voices[VOICE_COUNT];
Svf   filt;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	CpuLoad.OnBlockStart();

	myPod.ProcessAllControls();

    // Clear the output buffer as we are going to sum into it as a mix
    memset(*out, 0, 2*BLOCK_SIZE);

    if( myPod.button1.RisingEdge() )
    {
        #ifdef TEXT_ON
        	FixedCapStr<100>str("Button 1");
            myPod.seed.PrintLine(str);
        #endif
        voices[0].envelope::Trigger();
    }

    if( myPod.button2.RisingEdge() )
    {
        #ifdef TEXT_ON
        	FixedCapStr<100>str("Button 2");
            myPod.seed.PrintLine(str);
        #endif
        voices[0].envelope::Trigger();
    }
    
    for( int i=0; i<VOICE_COUNT; i++ )
    {
        voices[i].Process( out, size );
    }

/*
	for (size_t i = 0; i < size; i++)
	{
        // The oscillator's Process function synthesizes, and
        // returns the next sample.
        //float sample = osc.Process();
        volatile float sample = out[0][i];
		filt.Process(sample);
		sample = filt.Low();
		out[0][i] = sample;
        out[1][i] = sample;
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
#ifdef TEXT_ON
            char        buff[512];
            sprintf(buff,
                    "Note Received:\t%d\t%d\t%d\r\n",
                    m.channel,
                    m.data[0],
                    m.data[1]);
            myPod.seed.PrintLine(buff);
#endif
            // This is to avoid Max/MSP Note outs for now..
            if(m.data[1] != 0)
            {
                p = m.AsNoteOn();
//                thisVoice.SetFreq(mtof(p.note));
//                thisVoice.SetAmp((p.velocity / 127.0f));
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
#ifdef TEXT_ON
	myPod.seed.StartLog(true);
#endif
	myPod.SetAudioBlockSize(BLOCK_SIZE); // number of samples handled per callback
	myPod.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	CpuLoad.Init( myPod.AudioSampleRate(), BLOCK_SIZE );

    // OLED
    OledDisplay<SSD13054WireSpi132x64Driver>::Config display_config;
    display.Init(display_config);

    display.DrawLine( 10,10, 20, 20, 1 );
    display.DrawRect( 0,0, 127, 63, 1, false );
    display.WriteString( "Hello World", DEFAULT_FONT, true );
    display.Update();

	myPod.StartAdc();

    //thisVoice.Init( myPod.AudioSampleRate() );

    for( int i=0; i<VOICE_COUNT; i++ )
    {
        voices[i].Init( myPod.AudioSampleRate() );
    }

    filt.Init(myPod.AudioSampleRate());

	myPod.StartAudio(AudioCallback);

	// Initialse a simple timer to print the CPU load every CPU_LOAD_UPDATE_INTERVAL microseconds.
	uint32_t lastTime = System::GetUs();
	while(1) {

		if( ( System::GetUs() - lastTime ) > CPU_LOAD_UPDATE_INTERVAL ) {	// 500ms
			lastTime = System::GetUs();
#ifdef TEXT_ON
			// Print the load to the serial console
			FixedCapStr<100>str("Avg = ");
			str.AppendFloat( CpuLoad.GetAvgCpuLoad() );
			str.Append( "  Max = ");
			str.AppendFloat( CpuLoad.GetMaxCpuLoad() );
			myPod.seed.PrintLine(str);
#endif
		}

        myPod.midi.Listen();
        // Handle MIDI Events
        while(myPod.midi.HasEvents())
        {
            HandleMidiMessage( myPod.midi.PopEvent() );
        }

	}
}
