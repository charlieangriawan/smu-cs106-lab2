#include "MicroBit.h"

// MicroBit final global variables
#define MICROBIT_SLEEP_INTERVAL 1000

int CHANGED_COUNT = 0;
int BUFF_AVG_FLUX = 0;
int RAW_TOTAL_FLUX = 0;
int SIGNALER_COUNT = 0;

MicroBitImage ICON_BOX("255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n");


MicroBit uBit;

void onDataChannel2(MicroBitEvent);

int main()
{
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onDataChannel2);

    uBit.radio.setGroup(202);

    uBit.radio.enable();

    uBit.display.print("L");

    while (true) {
        SIGNALER_COUNT = 0;
        RAW_TOTAL_FLUX = 0;

        uBit.sleep(MICROBIT_SLEEP_INTERVAL);

        int newAvgFlux = RAW_TOTAL_FLUX / SIGNALER_COUNT;

        // Detect changes to flux level
        if (abs(newAvgFlux - BUFF_AVG_FLUX) > 2) {
            CHANGED_COUNT++;
        } else {
            CHANGED_COUNT = 0;
        }

        // Accept a change in buffered flux if more than one changed data point
        if (CHANGED_COUNT > 1) {
            CHANGED_COUNT = 0;
            BUFF_AVG_FLUX = newAvgFlux;
        }

        if (BUFF_AVG_FLUX > 10) {
            uBit.display.print(ICON_BOX);
        } else {
            uBit.display.print("");
        }
    }
}

void onDataChannel2(MicroBitEvent) {
    PacketBuffer buffer = uBit.radio.datagram.recv();
    // Signal Strength range: 0 - 86
    int rateOfChange = buffer[0];
    RAW_TOTAL_FLUX += rateOfChange;
    SIGNALER_COUNT++;
    // uBit.serial.printf("%d:%d \r\n", SIGNALER_MEMBER_COUNT, rateOfChange);
}
