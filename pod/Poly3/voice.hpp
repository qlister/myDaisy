
#ifndef VOICE_HXX

#include <string>

using namespace std;

// base class for a voice
class voice{

private:
    string namex;
    int x;      // Added to play with GIT
    int y;      // and another change

public:
    voice(){}

    virtual string name( ) {
        return "undefined";
    };

}; 


#define VOICE_HXX

#endif