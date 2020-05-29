#include <bits/stdc++.h>
#include "Small/Small.hpp"

using namespace small;

int main() {

    init();
        alloc(movcl,(u8)0xFF);      // 0x0000
        alloc(movdx,(u16)0x0000);   // 0x0002
        alloc(intr,(u8)0x01);       // 0x0005
        alloc(movdx,(u16)0x000D);   // 0x0007 <-+
        alloc(intr,(u8)0x03);       // 0x000A   |
        alloc(quit);                // 0x000B   |
        alloc((u8)'7');             // 0x000C --+
    exec();
    die();

    return 0;
}