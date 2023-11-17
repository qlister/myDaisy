# myDaisy

An attempt at a basic **polyphonic synthesizer**.
I am interested in using this to learn C++ :-)

For hardware this uses a Daisy Seed module that is mounted on a Daisy Seed Pod.

The Daisy Seed is an ARM M7 based processor sitting on a PCB with some flash, a good stereo A->D and D->A

The Pod is a simplistic baseboard/motherboard for the Daisy Seed providing
some simple UI devices (LED's, rotary encoder, buttons).  It also incudes a headphone amp with volume control and a MIDI interface.
It is pretty much the minumum implementation required to get the Daisy Seed going.
