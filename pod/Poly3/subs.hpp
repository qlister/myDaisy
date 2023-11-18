
#include <string>

using namespace std;

int sub( void );


// base class for a voice
class voice{

private:
    string name;

public:
    voice(){}

    virtual string name( )
    {
        return "undefined";
    };

};

class square : public voice{

    string name( );

};