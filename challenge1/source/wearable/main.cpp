#include "MicroBit.h"

// Microbit global variables
int MICROBIT_SLEEP_INTERVAL = 10;

// Radio communications
#define DEVICE_WEARABLE                 9701
#define WEARABLE_EVT_HELP_ON            1
#define WEARABLE_EVT_HELP_OFF           2
#define WEARABLE_EVT_SHAKE_DETECTED     3

void buttonA_onDown(MicroBitEvent);
void buttonB_onDown(MicroBitEvent);
void accelerometer_onShake(MicroBitEvent);

MicroBit uBit;

int main()
{
    uBit.init();

    uBit.radio.enable();
    uBit.radio.event.listen(DEVICE_WEARABLE, MICROBIT_EVT_ANY);

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_DOWN, buttonA_onDown);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_DOWN, buttonB_onDown);
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, accelerometer_onShake);

    uBit.display.print("W");
    while (true) {
        uBit.sleep(MICROBIT_SLEEP_INTERVAL);
    }
}

void buttonA_onDown(MicroBitEvent e) {
    MicroBitEvent(DEVICE_WEARABLE, WEARABLE_EVT_HELP_ON);
}

void buttonB_onDown(MicroBitEvent e) {
    MicroBitEvent(DEVICE_WEARABLE, WEARABLE_EVT_HELP_OFF);
}

void accelerometer_onShake(MicroBitEvent e) {
    MicroBitEvent(DEVICE_WEARABLE, WEARABLE_EVT_SHAKE_DETECTED);
}
