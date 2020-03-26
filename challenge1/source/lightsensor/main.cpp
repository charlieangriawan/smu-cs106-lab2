#include "MicroBit.h"

MicroBit uBit;

int main()
{
    uBit.init();

    uBit.display.scroll("LIGHTSENSOR!");

    release_fiber();
}
