#include "MicroBit.h"

// Microbit global variables
int MICROBIT_SLEEP_INTERVAL = 10;

// Radio communications
#define DEVICE_WEARABLE         4000
#define WEARABLE_EVT_HELP_ON    1

void buttonA_onDown(MicroBitEvent);

MicroBit uBit;

int main()
{
    uBit.init();

    uBit.radio.enable();
    uBit.radio.event.listen(DEVICE_WEARABLE, MICROBIT_EVT_ANY);

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_DOWN, buttonA_onDown);

    uBit.display.print("W");
    while (true) {
        uBit.sleep(MICROBIT_SLEEP_INTERVAL);
    }
}

void buttonA_onDown(MicroBitEvent e) {
    MicroBitEvent(DEVICE_WEARABLE, WEARABLE_EVT_HELP_ON);
}
