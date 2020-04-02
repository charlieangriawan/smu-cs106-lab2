#include "MicroBit.h"

// MicroBit final global variables
int MICROBIT_SLEEP_INTERVAL = 1000;

int TOTAL_RATE_OF_CHANGE = 0;
int SIGNALER_MEMBER_COUNT = 0;

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
        SIGNALER_MEMBER_COUNT = 0;
        TOTAL_RATE_OF_CHANGE = 0;

        uBit.sleep(MICROBIT_SLEEP_INTERVAL);

        // uBit.serial.printf("FLUX:%d %d \r\n",
        //         TOTAL_RATE_OF_CHANGE / SIGNALER_MEMBER_COUNT,
        //         SIGNALER_MEMBER_COUNT);
        if (TOTAL_RATE_OF_CHANGE / SIGNALER_MEMBER_COUNT >= 10) {
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
    TOTAL_RATE_OF_CHANGE += rateOfChange;
    SIGNALER_MEMBER_COUNT++;
    // uBit.serial.printf("%d:%d \r\n", SIGNALER_MEMBER_COUNT, rateOfChange);

}
