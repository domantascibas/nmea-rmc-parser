#ifndef _MESSAGE_PARSER_H
#define _MESSAGE_PARSER_H

#include <stdint.h>

/**
 * @brief Data status indicator
 */
typedef enum {
  RMC_DATA_INVALID,
  RMC_DATA_VALID
} rmc_status_t;

/**
 * @brief North/South indicator
 */
typedef enum {
  RMC_NORTH,
  RMC_SOUTH
} rmc_ns_t;

/**
 * @brief East/West indicator
 */
typedef enum {
  RMC_EAST,
  RMC_WEST
} rmc_ew_t;

/**
 * @brief Time
 */
typedef struct
{
  uint8_t hours;         //!< 0-23
  uint8_t minutes;       //!< 0-59
  uint8_t seconds;       //!< 0-59
  uint16_t milliseconds; //!< 0-999
} rmc_time_t;

/**
 * @brief Date
 */
typedef struct
{
  uint8_t day;   //!< 1-31
  uint8_t month; //!< 1-12
  uint16_t year; //!< 2000-2999
} rmc_date_t;

/**
 * @brief RMC packet data
 */
typedef struct
{
  rmc_time_t time;
  rmc_status_t status;
  uint32_t latitude; //!< Latitude in decimal degrees multiplied by 1 000 000
  rmc_ns_t ns;
  uint32_t longitude; //!< Longitude in decimal degrees multiplied by 1 000 000
  rmc_ew_t ew;
  uint16_t speed;  //!< Speed in km/h
  uint16_t course; //!< Course in degrees
  rmc_date_t date;
} rmc_t;

typedef enum {
  RMC_ID,
  RMC_UTC,
  RMC_GPS_STATUS,
  RMC_LATITUDE,
  RMC_NS,
  RMC_LONGITUDE,
  RMC_EW,
  RMC_SPEED,
  RMC_COURSE_OVER_GROUND,
  RMC_DATE,
  RMC_MAGNETIC_VARIATION,
  RMC_MAGNETIC_VARIATION_EW,
  RMC_POSITION_MODE,
  RMC_NAVIGATION_STATUS,
  RMC_CHECKSUM
} rmc_field_t;

#ifdef __cplusplus
extern "C" {
#endif

void MessageParser_init(void);
void MessageParser_readByte(uint8_t, void (*rmc_cb)(rmc_t *));
void MessageParser_parseMessageField(void (*rmc_cb)(rmc_t *));

#ifdef __cplusplus
}
#endif

#endif