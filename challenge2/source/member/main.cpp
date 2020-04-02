#include "MicroBit.h"

// uBit final global variables
int const MICROBIT_SLEEP_INTERVAL = 500;

int TOTAL_SIGNAL_STRENGTH = 0;
int SIGNALER_MEMBER_COUNT = 0;

int PREV_AVG_SIGNAL_STRENGTH = 0;

int RADIO_CHANNEL_1 = 101;
int RADIO_CHANNEL_2 = 202;

void onDataChannel1(MicroBitEvent);

void broadcastBaseSignal();
void sendFluxData();

MicroBit uBit;

int INTERVAL_ID = 0;

int main()
{
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onDataChannel1);

    uBit.radio.enable();
    uBit.radio.setTransmitPower(7);
    uBit.radio.setFrequencyBand(100);
    uBit.radio.setGroup(RADIO_CHANNEL_1);

    uBit.display.print("M");

    while (true) {
        SIGNALER_MEMBER_COUNT = 0;
        TOTAL_SIGNAL_STRENGTH = 0;

        uBit.sleep(MICROBIT_SLEEP_INTERVAL);

        broadcastBaseSignal();
        sendFluxData();

        INTERVAL_ID++;
    }
}

void onDataChannel1(MicroBitEvent) {
    PacketBuffer buffer = uBit.radio.datagram.recv();
    // getRSSI range: -128 - -42
    // signal strength range: 0 - 86
    int signalStrength =  128 + buffer.getRSSI();

    TOTAL_SIGNAL_STRENGTH += signalStrength;
    SIGNALER_MEMBER_COUNT++;

    // uBit.serial.printf("%d\r\n", SIGNALER_MEMBER_COUNT);
}

void broadcastBaseSignal() {

    PacketBuffer buffer(1);

    buffer[0] = 0; 
    uBit.radio.datagram.send(buffer);
}

void sendFluxData() {
    uBit.radio.setGroup(RADIO_CHANNEL_2);

    PacketBuffer buffer(1);

    // increase precision of signal strength
    int averageSignalStrength = TOTAL_SIGNAL_STRENGTH * 10  / SIGNALER_MEMBER_COUNT;
    // int averageSignalStrength = TOTAL_SIGNAL_STRENGTH * 1000 / SIGNALER_MEMBER_COUNT;
    int flux = abs(averageSignalStrength - PREV_AVG_SIGNAL_STRENGTH);
    buffer[0] =  flux;

    uBit.serial.printf("%d\r\n", flux);

    uBit.radio.datagram.send(buffer);

    PREV_AVG_SIGNAL_STRENGTH = averageSignalStrength;

    // reset uBit to channel 1 for broadcast and listen of channel 1 (base signal)
    uBit.radio.setGroup(RADIO_CHANNEL_1);
}
