#include "MicroBit.h"

MicroBit uBit;

int main()
{
    uBit.init();

    uBit.display.scroll("CHALLENGE 2!");

    release_fiber();
}
