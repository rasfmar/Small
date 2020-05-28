#include <bits/stdc++.h>
#include "Small/Small.hpp"

using namespace small;

int main() {

    init();
    mov(_reg16::ax,0x5678);
    mov(_reg16::bx,0x1234);
    dump();
    die();

    return 0;
}