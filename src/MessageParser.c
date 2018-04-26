#include "MessageParser.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define GPRMC_ID_STRING "GPRMC"

uint8_t fieldNumber = 0;
uint8_t fieldOffset = 0;
uint8_t checksum = 0;
bool receivingMessage = false;
bool receivingChecksum = false;
bool isRmcMessage = false;
char field[16];

rmc_t rmcMessage;

void MessageParser_init() {
  memset(field, '\0', sizeof(field));
}

void MessageParser_readByte(uint8_t byte, void (*rmc_cb)(rmc_t *rmcData))
{
  switch (byte)
  {
  case '$':
    checksum = 0;
    fieldNumber = 0;
    fieldOffset = 0;
    receivingMessage = true;
    isRmcMessage = false;
    break;

  case ',':
    checksum ^= byte;
    MessageParser_parseMessageField(rmc_cb);
    fieldNumber += 1;
    fieldOffset = 0;
    break;

  case '*':
    MessageParser_parseMessageField(rmc_cb);
    fieldNumber = RMC_CHECKSUM;
    fieldOffset = 0;
    if (receivingMessage)
    {
      receivingMessage = false;
      receivingChecksum = true;
    }
    break;

  case '\r':
  case '\n':
    if (receivingChecksum)
    {
      receivingChecksum = false;
      MessageParser_parseMessageField(rmc_cb);
      // rmc_cb();
    }
    break;

  default:
    field[fieldOffset] = byte;
    fieldOffset += 1;
    if (receivingMessage)
      checksum ^= byte;
    break;
  }
}

void MessageParser_parseMessageField(void (*rmc_cb)(rmc_t *rmc))
{
  uint8_t receivedChecksum;
  char tempBuf[4];
  float deg;

      switch (fieldNumber)
  {
  case RMC_ID:
    if (!strcmp(field, GPRMC_ID_STRING))
      isRmcMessage = true;
    else
      isRmcMessage = false;
    break;

  case RMC_UTC:
    memset(tempBuf, '\0', sizeof(tempBuf));
    strncpy(tempBuf, field, 2);
    rmcMessage.time.hours = (uint8_t)(atoi(tempBuf));

    memset(tempBuf, '\0', sizeof(tempBuf));
    memcpy(tempBuf, &field[2], 2);
    rmcMessage.time.minutes = (uint8_t)atoi(tempBuf);

    memset(tempBuf, '\0', sizeof(tempBuf));
    memcpy(tempBuf, &field[4], 2);
    rmcMessage.time.seconds = (uint8_t)atoi(tempBuf);

    memset(tempBuf, '\0', sizeof(tempBuf));
    if (strlen(field) > 9)
      memcpy(tempBuf, &field[7], 3);
    else
      memcpy(tempBuf, &field[7], 2);
    rmcMessage.time.milliseconds = (uint16_t)atoi(tempBuf);

    break;

  case RMC_GPS_STATUS:
    if (!strcmp(field, "A"))
    {
      rmcMessage.status = RMC_DATA_VALID;
    }
    else
      rmcMessage.status = RMC_DATA_INVALID;
    break;

  case RMC_LATITUDE:
    memset(tempBuf, '\0', sizeof(tempBuf));
    strncpy(tempBuf, field, 2);
    deg = atof(tempBuf);

    memset(tempBuf, '\0', sizeof(tempBuf));
    memcpy(tempBuf, &field[2], 8);
    deg += atof(tempBuf) / 60;

    rmcMessage.latitude = (uint32_t)(deg * 1000000);
    break;

  case RMC_NS:
    if (!strcmp(field, "N"))
      rmcMessage.ns = RMC_NORTH;
    else
      rmcMessage.ns = RMC_SOUTH;
    break;

  case RMC_LONGITUDE:
    memset(tempBuf, '\0', sizeof(tempBuf));
    strncpy(tempBuf, field, 3);
    deg = atof(tempBuf);

    memset(tempBuf, '\0', sizeof(tempBuf));
    memcpy(tempBuf, &field[3], 8);
    deg += atof(tempBuf) / 60;

    rmcMessage.longitude = (uint32_t)(deg * 1000000);
    break;

  case RMC_EW:
    if (!strcmp(field, "E"))
      rmcMessage.ew = RMC_EAST;
    else
      rmcMessage.ew = RMC_WEST;
    break;

  case RMC_SPEED:
    rmcMessage.speed = (uint16_t)(atof(field) * 1.852);
    break;

  case RMC_COURSE_OVER_GROUND:
    rmcMessage.course = (uint16_t)(atoi(field));
    break;

  case RMC_DATE:
    memset(tempBuf, '\0', sizeof(tempBuf));
    strncpy(tempBuf, field, 2);
    rmcMessage.date.day = (uint8_t)atoi(tempBuf);

    memset(tempBuf, '\0', sizeof(tempBuf));
    memcpy(tempBuf, &field[2], 2);
    rmcMessage.date.month = (uint8_t)atoi(tempBuf);

    memset(tempBuf, '\0', sizeof(tempBuf));
    memcpy(tempBuf, &field[4], 2);
    rmcMessage.date.year = 2000 + (uint16_t)atoi(tempBuf);
    break;

  case RMC_MAGNETIC_VARIATION:
    // not supported, should be blank
    break;

  case RMC_MAGNETIC_VARIATION_EW:
    // not supported, should be blank
    break;

  case RMC_POSITION_MODE:
    // not used
    break;

  case RMC_NAVIGATION_STATUS:
    // not used
    break;

  case RMC_CHECKSUM:
    receivedChecksum = (uint8_t)strtol(field, NULL, 16);
    if ((receivedChecksum == checksum) && isRmcMessage)
    {
      rmc_cb(&rmcMessage);
    }
    break;
  }
  memset(field, '\0', sizeof(field));
}
