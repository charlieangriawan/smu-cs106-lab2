#include "MicroBit.h"

// uBit final global variables
int MICROBIT_SLEEP_INTERVAL = 500;

int TOTAL_SIGNAL_STRENGTH = 0;
int SIGNALER_COUNT = 0;
int REPORTED_SIGNALER_COUNT = 0;

int PREV_AVG_SIGNAL_STRENGTH = 0;

int RADIO_CHANNEL_1 = 101;
int RADIO_CHANNEL_2 = 202;

void onDataChannel1(MicroBitEvent);
void decrementInterval(MicroBitEvent);
void incrementInterval(MicroBitEvent);

void broadcastBaseSignal();
void sendFluxData();

MicroBit uBit;

int main()
{
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onDataChannel1);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_DOWN, decrementInterval);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_DOWN, incrementInterval);

    uBit.radio.enable();
    uBit.radio.setTransmitPower(7);
    uBit.radio.setFrequencyBand(100);
    uBit.radio.setGroup(RADIO_CHANNEL_1);

    uBit.display.print("M");

    while (true) {
        SIGNALER_COUNT = 0;
        REPORTED_SIGNALER_COUNT = 0;
        TOTAL_SIGNAL_STRENGTH = 0;

        uBit.sleep(MICROBIT_SLEEP_INTERVAL);

        broadcastBaseSignal();
        sendFluxData();
    }
}

void onDataChannel1(MicroBitEvent) {
    PacketBuffer buffer = uBit.radio.datagram.recv();
    // getRSSI range: -128 - -42
    // signal strength range: 0 - 86
    int signalStrength =  128 + buffer.getRSSI();

    TOTAL_SIGNAL_STRENGTH += signalStrength;

    SIGNALER_COUNT++;
    // buffer[0] represents sender's signaler count
    REPORTED_SIGNALER_COUNT = max(REPORTED_SIGNALER_COUNT, buffer[0]);

    // uBit.serial.printf("%d\r\n", SIGNALER_MEMBER_COUNT);
}

void decrementInterval(MicroBitEvent) {
    MICROBIT_SLEEP_INTERVAL -= 50;
    uBit.display.scroll(MICROBIT_SLEEP_INTERVAL);
}

void incrementInterval(MicroBitEvent) {
    MICROBIT_SLEEP_INTERVAL += 50;
    uBit.display.scroll(MICROBIT_SLEEP_INTERVAL);
}

void broadcastBaseSignal() {
    PacketBuffer buffer(1);

    // Store signaler count in base signal
    buffer[0] = SIGNALER_COUNT;
    uBit.radio.datagram.send(buffer);
}

void sendFluxData() {

    PacketBuffer buffer(1);

    int maxSignalerCount = max(SIGNALER_COUNT, REPORTED_SIGNALER_COUNT);

    // increase precision of signal strength
    int avgSigStrength = TOTAL_SIGNAL_STRENGTH * 10 / maxSignalerCount;

    int flux = abs(avgSigStrength - PREV_AVG_SIGNAL_STRENGTH);

    buffer[0] =  flux;

    uBit.serial.printf("FLUX: %d, ", flux);
    uBit.serial.printf("TOTAL: %d, CURR_AVG: %d, PREV_AVG: %d, COUNT: %d, REP: %d\r\n",
            TOTAL_SIGNAL_STRENGTH, avgSigStrength, PREV_AVG_SIGNAL_STRENGTH,
            SIGNALER_COUNT, REPORTED_SIGNALER_COUNT);

    uBit.radio.setGroup(RADIO_CHANNEL_2);
    uBit.radio.datagram.send(buffer);
    uBit.radio.setGroup(RADIO_CHANNEL_1);

    PREV_AVG_SIGNAL_STRENGTH = avgSigStrength;

    // reset uBit to channel 1 for broadcast and listen of channel 1 (base signal)
}
