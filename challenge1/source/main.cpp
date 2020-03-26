#include "MicroBit.h"

MicroBit uBit;

int main()
{
    uBit.init();

    uBit.display.scroll("CHALLENGE 1!");

    release_fiber();
}
