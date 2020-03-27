#include "MicroBit.h"

// Microbit global variables
int MICROBIT_SLEEP_INTERVAL = 5000;

MicroBit uBit;

int main()
{
    uBit.init();
    uBit.radio.enable();

    uBit.display.print("L");
    while (true) {
        unsigned char lightLevel = uBit.display.readLightLevel();
        
        PacketBuffer buffer(1);
        buffer[0] = lightLevel;

        uBit.radio.datagram.send(buffer);

        uBit.sleep(MICROBIT_SLEEP_INTERVAL);
    }
}
