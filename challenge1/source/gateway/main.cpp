#include "MicroBit.h"

// Microbit global variables
int MICROBIT_SLEEP_INTERVAL = 10;

// Radio communications
#define DEVICE_WEARABLE         4000
#define WEARABLE_EVT_HELP_ON    1

void wearable_onhelpRequest(MicroBitEvent);

MicroBit uBit;

int main()
{
    uBit.init();

    uBit.radio.enable();

    uBit.messageBus.listen(DEVICE_WEARABLE, WEARABLE_EVT_HELP_ON, wearable_onhelpRequest);

    uBit.display.print("G");
    while (true) {
        uBit.sleep(MICROBIT_SLEEP_INTERVAL);
    }
}

void wearable_onhelpRequest(MicroBitEvent e)
{
    uBit.serial.printf("help requested ");
}
