#include "MicroBit.h"

// Microbit final global variables
int MICROBIT_SLEEP_INTERVAL = 200;

int SIGNALER_MEMBER_COUNT = 0;
int TOTAL_SIGNAL_STRENGTH = 0;

void onDataChannel1(MicroBitEvent);

void broadcastBaseSignal();
void sendFluxData();

MicroBit uBit;

int main()
{
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onDataChannel1);

    uBit.radio.enable();
    uBit.radio.setTransmitPower(7);
    uBit.radio.setFrequencyBand(100);
    uBit.radio.setGroup(1);

    uBit.display.print("M");

    while (true) {
        SIGNALER_MEMBER_COUNT = 0;
        TOTAL_SIGNAL_STRENGTH = 0;

        uBit.sleep(MICROBIT_SLEEP_INTERVAL);

        broadcastBaseSignal();
        sendFluxData();
    }
}

void onDataChannel1(MicroBitEvent) {
    PacketBuffer buffer = uBit.radio.datagram.recv();
    // signal strength range: 0 - 86
    int signalStrength =  128 + buffer.getRSSI();

    uBit.serial.printf("%d \r\n", signalStrength);

    TOTAL_SIGNAL_STRENGTH += signalStrength;
    SIGNALER_MEMBER_COUNT++;
}

void broadcastBaseSignal() {
    PacketBuffer buffer(1);
    buffer[0] = 0; 
    uBit.radio.datagram.send(buffer);
}

void sendFluxData() {
    uBit.radio.setGroup(2);

    PacketBuffer buffer(1);
    buffer[0] = TOTAL_SIGNAL_STRENGTH/SIGNALER_MEMBER_COUNT; 
    uBit.radio.datagram.send(buffer);

    uBit.radio.setGroup(1);
}
