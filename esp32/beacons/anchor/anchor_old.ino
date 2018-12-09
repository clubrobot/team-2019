#include <Arduino.h>
#include "pin.h"
#include <SPI.h>
#include "../../common/SerialTalks.h"
#include "../../common/DWM1000/DW1000.h"



// messages used in the ranging protocol
// TODO replace by enum
#define POLL 0
#define POLL_ACK 1
#define RANGE 2
#define RANGE_REPORT 3
#define RANGE_FAILED 255
// message flow state
volatile byte expectedMsgId = POLL;
// message sent/received state
volatile boolean sentAck = false;
volatile boolean receivedAck = false;
// protocol error state
boolean protocolFailed = false;
// timestamps to remember
DW1000Time timePollSent;
DW1000Time timePollReceived;
DW1000Time timePollAckSent;
DW1000Time timePollAckReceived;
DW1000Time timeRangeSent;
DW1000Time timeRangeReceived;
// last computed range/time
DW1000Time timeComputedRange;
// data buffer
#define LEN_DATA 16
byte data[LEN_DATA];
// watchdog and reset period
uint32_t lastActivity;
uint32_t resetPeriod = 250;
// reply times (same on both sides for symm. ranging)
uint16_t replyDelayTimeUS = 3000;
// ranging counter (per second)
uint16_t successRangingCount = 0;
uint32_t rangingCountPeriod = 0;
float samplingRate = 0;

void receiver()
{
  DW1000.newReceive();
  DW1000.setDefaults();
  // so we don't need to restart the receiver manually
  DW1000.receivePermanently(true);
  DW1000.startReceive();
}

void noteActivity()
{
  // update activity timestamp, so that we do not reach "resetPeriod"
  lastActivity = millis();
  digitalWrite(PIN_LED_FAIL, LOW);
  digitalWrite(PIN_LED_OK, HIGH);
}

void resetInactive()
{
  // anchor listens for POLL
  expectedMsgId = POLL;
  receiver();
  noteActivity();
  digitalWrite(PIN_LED_FAIL, HIGH);
  digitalWrite(PIN_LED_OK, LOW);
}

void handleSent()
{
  // status change on sent success
  sentAck = true;
}

void handleReceived()
{
  // status change on received success
  receivedAck = true;
}

void transmitPollAck()
{
  DW1000.newTransmit();
  DW1000.setDefaults();
  data[0] = POLL_ACK;
  // delay the same amount as ranging tag
  DW1000Time deltaTime = DW1000Time(replyDelayTimeUS, DW1000Time::MICROSECONDS);
  DW1000.setDelay(deltaTime);
  DW1000.setData(data, LEN_DATA);
  DW1000.startTransmit();
}

void transmitRangeReport(float curRange)
{
  DW1000.newTransmit();
  DW1000.setDefaults();
  data[0] = RANGE_REPORT;
  // write final ranging result
  memcpy(data + 1, &curRange, 4);
  DW1000.setData(data, LEN_DATA);
  DW1000.startTransmit();
}

void transmitRangeFailed()
{
  DW1000.newTransmit();
  DW1000.setDefaults();
  data[0] = RANGE_FAILED;
  DW1000.setData(data, LEN_DATA);
  DW1000.startTransmit();
  digitalWrite(PIN_LED_FAIL,HIGH);
  digitalWrite(PIN_LED_OK, LOW);
}


/*
 * RANGING ALGORITHMS
 * ------------------
 * Either of the below functions can be used for range computation (see line "CHOSEN
 * RANGING ALGORITHM" in the code).
 * - Asymmetric is more computation intense but least error prone
 * - Symmetric is less computation intense but more error prone to clock drifts
 *
 * The anchors and tags of this reference example use the same reply delay times, hence
 * are capable of symmetric ranging (and of asymmetric ranging anyway).
 */

void computeRangeAsymmetric()
{
  // asymmetric two-way ranging (more computation intense, less error prone)
  DW1000Time round1 = (timePollAckReceived - timePollSent).wrap();
  DW1000Time reply1 = (timePollAckSent - timePollReceived).wrap();
  DW1000Time round2 = (timeRangeReceived - timePollAckSent).wrap();
  DW1000Time reply2 = (timeRangeSent - timePollAckReceived).wrap();
  DW1000Time tof = (round1 * round2 - reply1 * reply2) / (round1 + round2 + reply1 + reply2);
  // set tof timestamp
  timeComputedRange.setTimestamp(tof);
}

void computeRangeSymmetric()
{
  // symmetric two-way ranging (less computation intense, more error prone on clock drift)
  DW1000Time tof = ((timePollAckReceived - timePollSent) - (timePollAckSent - timePollReceived) +
                    (timeRangeReceived - timePollAckSent) - (timeRangeSent - timePollAckReceived)) *
                   0.25f;
  // set tof timestamp
  timeComputedRange.setTimestamp(tof);
}

/*
 * END RANGING ALGORITHMS
 * ----------------------
 */

void setup()
{

  pinMode(PIN_VBAT, INPUT);
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_ENABLE_KEY, OUTPUT);
  pinMode(PIN_LED_OK, OUTPUT);
  pinMode(PIN_LED_FAIL, OUTPUT);
  pinMode(PIN_UWB_RST, OUTPUT);
  pinMode(PIN_IRQ, INPUT);
  pinMode(PIN_ESYNCHRO, INPUT);
  pinMode(PIN_SYNCHRO, INPUT);
  // DEBUG monitoring
  Serial.begin(SERIALTALKS_BAUDRATE);
  talks.begin(Serial);
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("### DW1000-arduino-ranging-anchor ###"));
  // initialize the driver
  DW1000.(PIN_IRQ, PIN_SPICLK, PIN_SPIMISO, PIN_SPIMOSI, PIN_UWB_RST);
  DW1000.select(PIN_SPICSN);
  Serial.println(F("DW1000 initialized ..."));
  // general configuration
  DW1000.newConfiguration();
  DW1000.setDefaults();
  DW1000.setDeviceAddress(1);
  DW1000.setNetworkId(10);
  DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_ACCURACY);
  DW1000.commitConfiguration();
  Serial.println(F("Committed configuration ..."));
  // DEBUG chip info and registers pretty printed
  char msg[128];
  DW1000.getPrintableDeviceIdentifier(msg);
  Serial.print("Device ID: ");
  Serial.println(msg);
  DW1000.getPrintableExtendedUniqueIdentifier(msg);
  Serial.print("Unique ID: ");
  Serial.println(msg);
  DW1000.getPrintableNetworkIdAndShortAddress(msg);
  Serial.print("Network ID & Device Address: ");
  Serial.println(msg);
  DW1000.getPrintableDeviceMode(msg);
  Serial.print("Device mode: ");
  Serial.println(msg);
  // attach callback for (successfully) sent and received messages
  DW1000.attachSentHandler(handleSent);
  DW1000.attachReceivedHandler(handleReceived);
  // anchor starts in receiving mode, awaiting a ranging poll message
  receiver();
  noteActivity();
  // for first time ranging frequency computation
  rangingCountPeriod = millis();
}

void loop()
{
  talks.execute();

  int32_t curMillis = millis();
  if (!sentAck && !receivedAck)
  {
    // check if inactive
    if (curMillis - lastActivity > resetPeriod)
    {
      resetInactive();
    }
    return;
  }
  // continue on any success confirmation
  if (sentAck)
  {
    sentAck = false;
    byte msgId = data[0];
    if (msgId == POLL_ACK)
    {
      DW1000.getTransmitTimestamp(timePollAckSent);
      noteActivity();
    }
  }
  if (receivedAck)
  {
    receivedAck = false;
    // get message and parse
    DW1000.getData(data, LEN_DATA);
    byte msgId = data[0];
    if (msgId != expectedMsgId)
    {
      // unexpected message, start over again (except if already POLL)
      protocolFailed = true;
    }
    if (msgId == POLL)
    {
      // on POLL we (re-)start, so no protocol failure
      protocolFailed = false;
      DW1000.getReceiveTimestamp(timePollReceived);
      expectedMsgId = RANGE;
      transmitPollAck();
      noteActivity();
    }
    else if (msgId == RANGE)
    {
      DW1000.getReceiveTimestamp(timeRangeReceived);
      expectedMsgId = POLL;
      if (!protocolFailed)
      {
        timePollSent.setTimestamp(data + 1);
        timePollAckReceived.setTimestamp(data + 6);
        timeRangeSent.setTimestamp(data + 11);
        // (re-)compute range as two-way ranging is done
        computeRangeAsymmetric(); // CHOSEN RANGING ALGORITHM
        transmitRangeReport(timeComputedRange.getAsMicroSeconds());
        float distance = timeComputedRange.getAsMeters()-0.6;
        //Serial.print("Range: ");
        Serial.print(distance);
        //Serial.print(" m");
        //Serial.print("\t RX power: ");
        Serial.print(";");
        Serial.print(DW1000.getReceivePower());
        //Serial.print(" dBm");
        //Serial.print("\t Sampling: ");
        Serial.println("");
        /*Serial.print(samplingRate);
        Serial.print(" Hz");
        Serial.print("\t times : ");
        float t1 = timePollAckSent.getAsMicroSeconds()- timePollReceived.getAsMicroSeconds();
        Serial.print(t1);
        Serial.print("\t");
        //Serial.print(";");
        float t2 = timeRangeSent.getAsMicroSeconds() - timePollAckReceived.getAsMicroSeconds();
        Serial.print(t2);
        //Serial.print(";");
        Serial.print("\tdiff : ");
        Serial.println(t1-t2);
*/
        
        //Serial.print("FP power is [dBm]: "); Serial.print(DW1000.getFirstPathPower());
        //Serial.print("RX power is [dBm]: "); Serial.println(DW1000.getReceivePower());
        //Serial.print("Receive quality: "); Serial.println(DW1000.getReceiveQuality());
        // update sampling rate (each second)
        successRangingCount++;
        if (curMillis - rangingCountPeriod > 1000)
        {
          samplingRate = (1000.0f * successRangingCount) / (curMillis - rangingCountPeriod);
          rangingCountPeriod = curMillis;
          successRangingCount = 0;
        }
      }
      else
      {
        transmitRangeFailed();
      }

      noteActivity();
    }
  }
}
