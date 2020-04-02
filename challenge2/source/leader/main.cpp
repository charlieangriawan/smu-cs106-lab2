#include "MicroBit.h"

// MicroBit final global variables
#define MICROBIT_SLEEP_INTERVAL 1000
#define FLUX_THRESHOLD 120

int CHANGED_COUNT = 0;
int BUFF_AVG_FLUX = 0;
int RAW_TOTAL_FLUX = 0;
int SIGNALER_COUNT = 0;

MicroBitImage ICON_BOX("255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n");
MicroBitImage BLANK_BOX("0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n");


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
        long prevTime = uBit.systemTime();
        SIGNALER_COUNT = 0;
        RAW_TOTAL_FLUX = 0;

        uBit.sleep(MICROBIT_SLEEP_INTERVAL);

        int newAvgFlux = RAW_TOTAL_FLUX * 10 / SIGNALER_COUNT;

        // Detect changes to flux level
        if (abs(newAvgFlux - BUFF_AVG_FLUX) > 3) {
            CHANGED_COUNT++;
        } else {
            CHANGED_COUNT = 0;
        }

        // Accept a change in buffered flux if more than one changed data point
        if (CHANGED_COUNT > 1) {
            CHANGED_COUNT = 0;
            BUFF_AVG_FLUX = newAvgFlux;
        }

        if (BUFF_AVG_FLUX >= FLUX_THRESHOLD) {
            uBit.display.print(ICON_BOX);
        } else {
            uBit.display.print(BLANK_BOX);
        }

        // uBit.serial.printf("new flux: %d, BUFF FLUX: %d\r\n", newAvgFlux, BUFF_AVG_FLUX);
        uBit.serial.printf("AVG: %d, COUNT: %d, TIME: %lu\r\n",
                newAvgFlux, SIGNALER_COUNT, uBit.systemTime() - prevTime);
    }
}

void onDataChannel2(MicroBitEvent) {
    PacketBuffer buffer = uBit.radio.datagram.recv();
    // Signal Strength range: 0 - 86
    int rateOfChange = buffer[0];
    RAW_TOTAL_FLUX += rateOfChange;
    SIGNALER_COUNT++;
    // uBit.serial.printf("%d:%d \r\n", SIGNALER_COUNT, rateOfChange);
}
