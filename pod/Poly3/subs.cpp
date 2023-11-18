
#include "voice.hpp"
#include "voice_square.hpp"
#include "Poly3.hpp"

using namespace std;


class base {

public:
        int var;
    
public:
    base ( int x ) {
        myDaisy->Print( "Base constructor\n" );
        var = x;
        // constructor
    };
    ~base () {
        // destructor
    };
    virtual int fn( void ) {
        return var;
    };
};

class mid : public base{

public:
    mid( int a ) : base( a ){
    }; 

    int fn( void ) {
        return 20;
    }

};

int sub( void ){

    class mid *mid_o;

    mid_o = new mid( 77 );
    myDaisy->PrintLine( "Mid instance of sub returns %s\n", mid_o->fn() );

    class base *base_o;
    base_o = new base( 88 );

    myDaisy->PrintLine( "Base instance of sub returns %d \n", base_o->fn() );

    base_o = mid_o;
    myDaisy->PrintLine( "Mid instance of sub using mid object returns %d", base_o->fn() );

    return 5;

};





