#include "MicroBit.h"

// Microbit global variables
int MICROBIT_SLEEP_INTERVAL = 10;

// Radio communications
#define DEVICE_WEARABLE                 9701
#define WEARABLE_EVT_HELP_ON            1
#define WEARABLE_EVT_HELP_OFF           2
#define WEARABLE_EVT_SHAKE_DETECTED     3

void wearable_onHelpRequest(MicroBitEvent);
void wearable_onHelpCancelation(MicroBitEvent);
void wearable_onMotionDetected(MicroBitEvent);

MicroBit uBit;

int main()
{
    uBit.init();

    uBit.radio.enable();

    uBit.messageBus.listen(DEVICE_WEARABLE, WEARABLE_EVT_HELP_ON, wearable_onHelpRequest);
    uBit.messageBus.listen(DEVICE_WEARABLE, WEARABLE_EVT_HELP_OFF, wearable_onHelpCancelation);
    uBit.messageBus.listen(DEVICE_WEARABLE, WEARABLE_EVT_SHAKE_DETECTED, wearable_onMotionDetected);

    uBit.display.print("G");
    while (true) {
        uBit.sleep(MICROBIT_SLEEP_INTERVAL);
    }
}

void wearable_onHelpRequest(MicroBitEvent e) {
    uBit.serial.printf("help requested ");
}

void wearable_onHelpCancelation(MicroBitEvent e) {
    uBit.serial.printf("help request canceled ");
}

void wearable_onMotionDetected(MicroBitEvent e) {
    uBit.serial.printf("motion detected ");
}
