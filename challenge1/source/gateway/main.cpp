/*
 * Challenge 1 Gateway
 */
#include "MicroBit.h"

// Microbit final global variables
int MICROBIT_SLEEP_INTERVAL = 1000;
int LIGHTLEVEL_TRESHOLD = 20;
int INACTIVITY_TRESHOLD = 600;
MicroBitImage ICON_HELP("255,255,0,255,255\n0,255,0,255,0\n0,0,0,0,0\n0,255,255,255,0\n255,0,0,0,255\n");
MicroBitImage ICON_INACTIVE("0,255,0,255,0\n0,255,0,255,0\n0,255,0,255,0\n0,0,0,0,0\n0,255,0,255,0\n");

// Radio communications
#define DEVICE_WEARABLE                 9701
#define WEARABLE_EVT_HELP_ON            1
#define WEARABLE_EVT_HELP_OFF           2
#define WEARABLE_EVT_SHAKE_DETECTED     3

void lightSensor_onData(MicroBitEvent);
void wearable_onHelpRequest(MicroBitEvent);
void wearable_onHelpCancelation(MicroBitEvent);
void wearable_onMotionDetected(MicroBitEvent);

// State global variables
bool IS_NIGHT = false;
bool HELP_REQUESTED = false;
bool MOTION_DETECTED = false;
int MOTION_UNDETECTED_CYCLECOUNT = 0;

MicroBit uBit;

void processStateUpdate();

int main()
{
    uBit.init();

    uBit.radio.enable();

    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, lightSensor_onData);

    uBit.messageBus.listen(DEVICE_WEARABLE, WEARABLE_EVT_HELP_ON, wearable_onHelpRequest);
    uBit.messageBus.listen(DEVICE_WEARABLE, WEARABLE_EVT_HELP_OFF, wearable_onHelpCancelation);
    uBit.messageBus.listen(DEVICE_WEARABLE, WEARABLE_EVT_SHAKE_DETECTED, wearable_onMotionDetected);

    while (true) {
        processStateUpdate();
        uBit.sleep(MICROBIT_SLEEP_INTERVAL);
        MOTION_UNDETECTED_CYCLECOUNT++;
    }
}

void _displayIcons(bool help, bool inactive) {
    if (help) {
        uBit.display.print(ICON_HELP);
        uBit.sleep(MICROBIT_SLEEP_INTERVAL/2);
    }
    if (inactive) {
        uBit.display.print(ICON_INACTIVE);
    }
}

void processStateUpdate() {
    bool help = false, inactive = false;
    if (HELP_REQUESTED) {
        help = true;
        if (MOTION_UNDETECTED_CYCLECOUNT > 5) inactive = true;
    } else uBit.display.clear();
    if (!IS_NIGHT && MOTION_UNDETECTED_CYCLECOUNT > INACTIVITY_TRESHOLD) inactive = true;

    _displayIcons(help, inactive);
}

void lightSensor_onData(MicroBitEvent) {
    PacketBuffer buffer = uBit.radio.datagram.recv();
    unsigned char lightLevel = buffer[0];

    if (lightLevel > LIGHTLEVEL_TRESHOLD) IS_NIGHT = false;
    else IS_NIGHT = true;
}

void wearable_onHelpRequest(MicroBitEvent) {
    HELP_REQUESTED = true;
}

void wearable_onHelpCancelation(MicroBitEvent) {
    HELP_REQUESTED = false;
}

void wearable_onMotionDetected(MicroBitEvent) {
    MOTION_DETECTED = true;
    MOTION_UNDETECTED_CYCLECOUNT = 0;
}
