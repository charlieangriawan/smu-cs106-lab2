/*
 * Challenge 2
 */
#include "MicroBit.h"

#define MICROBIT_SLEEP_INTERVAL     200
#define TRANSMIT_POWER              7
#define TRANSMIT_FREQUENCY          10
#define MOTION_THRESHOLD            4
#define MOTION_BUFFER_THRESHOLD     5
#define DISPLAY_BUFFER_THRESHOLD    5
#define RADIO_BROADCAST_KEY         111
#define RADIO_REPORT_KEY            122
#define EMPTY                       "0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n"
#define FULL                        "255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n"
#define MEMBER                      "0,0,0,0,0\n"
#define LEADER                      "0,0,255,0,0\n"
#define CHANNEL1                    "255,0,0,0,0\n"
#define CHANNEL2                    "0,255,0,0,0\n"
#define CHANNEL3                    "0,0,255,0,0\n"
#define CHANNEL4                    "0,0,0,255,0\n"
#define CHANNEL5                    "0,0,0,0,255\n"
#define REPORT_CHANNEL              10

MicroBitImage ICON_UNLIT("0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n");
MicroBitImage ICON_LIT("255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n");

uint8_t BROADCAST_CHANNEL = 0;
bool IS_LEADER = false;
int PREV_SIGNAL_STRENGTH = 0;
bool MOTION_DETECTED = true;
int MOTION_BUFFER_CYCLE = MOTION_BUFFER_THRESHOLD;
int DISPLAY_BUFFER_CYCLE = DISPLAY_BUFFER_THRESHOLD;

void reinitializeGlobalTempVariables() {
    MOTION_DETECTED = false;
}

void displayLEDIndicator();
void handleButtonEvents(MicroBitEvent);

void broadcastSignal();
void onChannelSignal(MicroBitEvent);
void reportSignalDisruption();

MicroBit uBit;

int main()
{
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_ANY, MICROBIT_EVT_ANY, handleButtonEvents);

    uBit.radio.enable();
    uBit.radio.setTransmitPower(TRANSMIT_POWER);
    uBit.radio.setFrequencyBand(TRANSMIT_FREQUENCY);
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onChannelSignal);

    while (true) {
        reinitializeGlobalTempVariables();

        if (IS_LEADER) uBit.radio.setGroup(REPORT_CHANNEL);
        else uBit.radio.setGroup(BROADCAST_CHANNEL);

        uBit.sleep(MICROBIT_SLEEP_INTERVAL);

        if (BROADCAST_CHANNEL != 0) broadcastSignal();

        if (DISPLAY_BUFFER_CYCLE < DISPLAY_BUFFER_THRESHOLD) displayLEDIndicator();
        else if (IS_LEADER) {
            if (MOTION_DETECTED || (0 < MOTION_BUFFER_CYCLE && MOTION_BUFFER_CYCLE <= MOTION_BUFFER_THRESHOLD)) uBit.display.print(ICON_LIT);
            else uBit.display.print(ICON_UNLIT);
        } else {
            uBit.display.print(ICON_UNLIT);
        }

        MOTION_BUFFER_CYCLE++;
        DISPLAY_BUFFER_CYCLE++;
    }
}

void _decrementChannel() {
    if (BROADCAST_CHANNEL != 1) BROADCAST_CHANNEL--;
    else BROADCAST_CHANNEL = 5;
}

void _incrementChannel() {
    if (BROADCAST_CHANNEL != 5) BROADCAST_CHANNEL++;
    else BROADCAST_CHANNEL = 1;
}

void _selfAssignLeadership() {
    IS_LEADER = !IS_LEADER;
}

void handleButtonEvents(MicroBitEvent e) {
    if (e.source == MICROBIT_ID_RADIO) return;
    DISPLAY_BUFFER_CYCLE = 0;
    if (e.source == MICROBIT_ID_BUTTON_AB && e.value == MICROBIT_BUTTON_EVT_HOLD) _selfAssignLeadership();
    else if (e.source == MICROBIT_ID_BUTTON_A && e.value == MICROBIT_BUTTON_EVT_CLICK) _decrementChannel();
    else if (e.source == MICROBIT_ID_BUTTON_B && e.value == MICROBIT_BUTTON_EVT_CLICK) _incrementChannel();
}

void displayLEDIndicator() {
    char indicatorString[101] = "";

    if (IS_LEADER) strcat(indicatorString, LEADER);
    else strcat(indicatorString, MEMBER);

    strcat(indicatorString, EMPTY);

    if (BROADCAST_CHANNEL == 1) strcat(indicatorString, CHANNEL1);
    else if (BROADCAST_CHANNEL == 2) strcat(indicatorString, CHANNEL2);
    else if (BROADCAST_CHANNEL == 3) strcat(indicatorString, CHANNEL3);
    else if (BROADCAST_CHANNEL == 4) strcat(indicatorString, CHANNEL4);
    else if (BROADCAST_CHANNEL == 5) strcat(indicatorString, CHANNEL5);

    MicroBitImage indicatorImage(indicatorString);
    uBit.display.print(indicatorImage);
}

void broadcastSignal() {
    uBit.radio.setGroup(BROADCAST_CHANNEL);
    PacketBuffer buffer(1);
    buffer[0] = RADIO_BROADCAST_KEY;
    uBit.radio.datagram.send(buffer);
}

void _processBroadcastMessage(PacketBuffer buffer) {
    int signalStrength =  128 + buffer.getRSSI();

    int flux = abs(PREV_SIGNAL_STRENGTH - signalStrength);
    if (flux > MOTION_THRESHOLD) reportSignalDisruption();

    PREV_SIGNAL_STRENGTH = signalStrength;
}

void _processReportMessage(PacketBuffer buffer) {
    if (buffer[1] == 1) {
        MOTION_DETECTED = true;
        MOTION_BUFFER_CYCLE = 1;
    }
}

void onChannelSignal(MicroBitEvent) {
    PacketBuffer buffer = uBit.radio.datagram.recv();
    int key = buffer[0];
    if (key == RADIO_BROADCAST_KEY) _processBroadcastMessage(buffer);
    else if (key == RADIO_REPORT_KEY) _processReportMessage(buffer);
}

void reportSignalDisruption() {
    uBit.radio.setGroup(REPORT_CHANNEL);

    PacketBuffer buffer(2);
    buffer[0] = RADIO_REPORT_KEY;
    buffer[1] = 1;
    uBit.radio.datagram.send(buffer);
}
