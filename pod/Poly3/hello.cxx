
#include <iostream>
//#include "subs.hxx"
#include "voice.hxx"
#include "voice_square.hxx"

using namespace std;

int main(){

    square *mySquare;

    cout << "Hello today\n";

    mySquare = new square;
    cout << "Name = " << mySquare->name() << endl;

    voice *myVoice;
    myVoice = mySquare;
    cout <<  "Voice name = " << myVoice->name() << endl;

    return 0;
    
}
