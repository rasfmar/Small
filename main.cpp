#include <bits/stdc++.h>
#include "Small/Small.hpp"

using namespace small;

int main() {

    init();
    //------------ SOURCE --------------//
        alloc(  movcl,  (u8)0xFF    );  // red component set to 255
    // loop                             //
        alloc(  movdx,  (u16)0x0000 );  // green, blue components set to 0
        alloc(  intr,   (u8)0x02    );  // change background color of text
        alloc(  movdx,  (u16)0x0014 );  // pass address of space character to dx register
        alloc(  intr,   (u8)0x03    );  // print char to stdout
        alloc(  deccl               );  // decrement cl register 5 times
        alloc(  deccl               );  //      because cl is the red component
        alloc(  deccl               );  //
        alloc(  deccl               );  //
        alloc(  deccl               );  //
        alloc(  jmp,    (u16)0x0002 );  // jump back to address of loop
    //------------- DATA ---------------//
        alloc(          (u8)' '     );
    exec();
    die();

    return 0;
}