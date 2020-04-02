#include "MicroBit.h"

// Microbit final global variables
int MICROBIT_SLEEP_INTERVAL = 1000;

int PREV_TOTAL_RATE_OF_CHANGE = 0;
int TOTAL_RATE_OF_CHANGE = 0;

MicroBit uBit;

void onDataChannel2(MicroBitEvent);

int main()
{
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onDataChannel2);

    uBit.radio.setGroup(2);

    uBit.radio.enable();

    uBit.display.print("L");

    while (true) {
        PREV_TOTAL_RATE_OF_CHANGE = TOTAL_RATE_OF_CHANGE;
        TOTAL_RATE_OF_CHANGE = 0;
        uBit.sleep(MICROBIT_SLEEP_INTERVAL);

        int DIFF_RATE_OF_CHANGE = PREV_TOTAL_RATE_OF_CHANGE - TOTAL_RATE_OF_CHANGE;
        // uBit.serial.printf("%d \r\n", DIFF_RATE_OF_CHANGE);
        if (DIFF_RATE_OF_CHANGE > 100 || DIFF_RATE_OF_CHANGE < -100) {
            uBit.serial.printf("%s \r\n", "you move");
        } else {
            uBit.serial.printf("%s \r\n", "you no move");
        }
    }
}

void onDataChannel2(MicroBitEvent) {
    PacketBuffer buffer = uBit.radio.datagram.recv();
    // signal strength range: 0 - 86
    // int signalStrength =  128 + buffer.getRSSI();
    int rateOfChange = buffer[0];
    // uBit.serial.printf("%d \r\n", rateOfChange);

    TOTAL_RATE_OF_CHANGE += rateOfChange;
}
