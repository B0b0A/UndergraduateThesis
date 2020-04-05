/*
 * tftspi.h
 *
 *  Created on: Mar 27, 2020
 *      Author: Bojan Aleksovski
 *      Used module from loboris.github
 */

#ifndef MAIN_INCLUDE_TFTSPI_H_
#define MAIN_INCLUDE_TFTSPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include "spi_master_lobo.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/spi_reg.h"
#include "driver/gpio.h"

#define TOUCH_TYPE_NONE		0
#define DISP_TYPE_ST7789V	0

// === Screen tft_orientation constants ===
#define PORTRAIT	0
#define LANDSCAPE	1
#define PORTRAIT_FLIP	2
#define LANDSCAPE_FLIP	3

// ** ESP-WROVER-KIT v3 configuration
// --------------------------------------------------------
#define DEFAULT_DISP_TYPE           DISP_TYPE_ST7789V
#define DEFAULT_TFT_DISPLAY_WIDTH   240
#define DEFAULT_TFT_DISPLAY_HEIGHT  320
#define DISP_COLOR_BITS_24          0x66
#define DEFAULT_GAMMA_CURVE         0
#define DEFAULT_SPI_CLOCK           26000000
#define TFT_INVERT_ROTATION         0
#define TFT_INVERT_ROTATION1        1
#define TFT_RGB_BGR                 0x00

#define USE_TOUCH	TOUCH_TYPE_NONE

#define PIN_NUM_MISO 25		// SPI MISO
#define PIN_NUM_MOSI 23		// SPI MOSI
#define PIN_NUM_CLK  19		// SPI CLOCK pin
#define PIN_NUM_CS   22		// Display CS pin
#define PIN_NUM_DC   21		// Display command/data pin
#define PIN_NUM_TCS   0		// Touch screen CS pin

#define PIN_NUM_RST  18  	// GPIO used for RESET control
#define PIN_NUM_BCKL  5     // GPIO used for backlight control
#define PIN_BCKL_ON   0     // GPIO value for backlight ON
#define PIN_BCKL_OFF  1     // GPIO value for backlight OFF
// --------------------------------------------------------

// CONFIG_PREDEFINED_DISPLAY_TYPE


// ##############################################################
// #### Global variables                                     ####
// ##############################################################

// ==== Converts colors to grayscale if 1 =======================
extern uint8_t tft_gray_scale;

// ==== Spi clock for reading data from display memory in Hz ====
extern uint32_t tft_max_rdclock;

// ==== Display dimensions in pixels ============================
extern int tft_width;
extern int tft_height;

// ==== Display type, DISP_TYPE_ILI9488 or DISP_TYPE_ILI9341 ====
extern uint8_t tft_disp_type;

// ==== Spi device handles for display and touch screen =========
extern spi_lobo_device_handle_t tft_disp_spi;
extern spi_lobo_device_handle_t tft_ts_spi;

// ##############################################################

// 24-bit color type structure
typedef struct __attribute__((__packed__)) {
//typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} color_t;

// ==== Display commands constants ====
#define TFT_INVOFF     0x20
#define TFT_INVONN     0x21
#define TFT_DISPOFF    0x28
#define TFT_DISPON     0x29
#define TFT_MADCTL	   0x36
#define TFT_PTLAR 	   0x30
#define TFT_ENTRYM 	   0xB7

#define TFT_CMD_NOP			0x00
#define TFT_CMD_SWRESET		0x01
#define TFT_CMD_RDDID		0x04
#define TFT_CMD_RDDST		0x09

#define TFT_CMD_SLPIN		0x10
#define TFT_CMD_SLPOUT		0x11
#define TFT_CMD_PTLON		0x12
#define TFT_CMD_NORON		0x13

#define TFT_CMD_RDMODE		0x0A
#define TFT_CMD_RDMADCTL	0x0B
#define TFT_CMD_RDPIXFMT	0x0C
#define TFT_CMD_RDIMGFMT	0x0D
#define TFT_CMD_RDSELFDIAG  0x0F

#define TFT_CMD_GAMMASET	0x26

#define TFT_CMD_FRMCTR1		0xB1
#define TFT_CMD_FRMCTR2		0xB2
#define TFT_CMD_FRMCTR3		0xB3
#define TFT_CMD_INVCTR		0xB4
#define TFT_CMD_DFUNCTR		0xB6

#define TFT_CMD_PWCTR1		0xC0
#define TFT_CMD_PWCTR2		0xC1
#define TFT_CMD_PWCTR3		0xC2
#define TFT_CMD_PWCTR4		0xC3
#define TFT_CMD_PWCTR5		0xC4
#define TFT_CMD_VMCTR1		0xC5
#define TFT_CMD_VMCTR2		0xC7

#define TFT_CMD_RDID1		0xDA
#define TFT_CMD_RDID2		0xDB
#define TFT_CMD_RDID3		0xDC
#define TFT_CMD_RDID4		0xDD

#define TFT_CMD_GMCTRP1		0xE0
#define TFT_CMD_GMCTRN1		0xE1

#define TFT_CMD_POWERA		0xCB
#define TFT_CMD_POWERB		0xCF
#define TFT_CMD_POWER_SEQ	0xED
#define TFT_CMD_DTCA		0xE8
#define TFT_CMD_DTCB		0xEA
#define TFT_CMD_PRC			0xF7
#define TFT_CMD_3GAMMA_EN	0xF2

#define ST_CMD_VCOMS       0xBB
#define ST_CMD_FRCTRL2      0xC6
#define ST_CMD_PWCTR1		0xD0

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_MH  0x04

#define TFT_CASET		0x2A
#define TFT_PASET		0x2B
#define TFT_RAMWR		0x2C
#define TFT_RAMRD		0x2E
#define TFT_CMD_PIXFMT	0x3A

#define TFT_CMD_DELAY	0x80


// Initialization sequence for ILI7749
// ====================================
static const uint8_t ST7789V_init[] = {
#if PIN_NUM_RST
  15,                   					        // 15 commands in list
#else
  16,                   					        // 16 commands in list
  TFT_CMD_SWRESET, TFT_CMD_DELAY,					//  1: Software reset, no args, w/delay
  200,												//     200 ms delay
#endif
  TFT_CMD_FRMCTR2, 5, 0x0c, 0x0c, 0x00, 0x33, 0x33,
  TFT_ENTRYM, 1, 0x45,
  ST_CMD_VCOMS, 1, 0x2B,
  TFT_CMD_PWCTR1, 1, 0x2C,
  TFT_CMD_PWCTR3, 2, 0x01, 0xff,
  TFT_CMD_PWCTR4, 1, 0x11,
  TFT_CMD_PWCTR5, 1, 0x20,
  ST_CMD_FRCTRL2, 1, 0x0f,
  ST_CMD_PWCTR1, 2, 0xA4, 0xA1,
  TFT_CMD_GMCTRP1, 14, 0xD0, 0x00, 0x05, 0x0E, 0x15, 0x0D, 0x37, 0x43, 0x47, 0x09, 0x15, 0x12, 0x16, 0x19,
  TFT_CMD_GMCTRN1, 14, 0xD0, 0x00, 0x05, 0x0D, 0x0C, 0x06, 0x2D, 0x44, 0x40, 0x0E, 0x1C, 0x18, 0x16, 0x19,
  TFT_MADCTL, 1, (MADCTL_MX | TFT_RGB_BGR),			// Memory Access Control (tft_orientation)
  TFT_CMD_PIXFMT, 1, DISP_COLOR_BITS_24,            // *** INTERFACE PIXEL FORMAT: 0x66 -> 18 bit; 0x55 -> 16 bit
  TFT_CMD_SLPOUT, TFT_CMD_DELAY, 120,				//  Sleep out,	//  120 ms delay
  TFT_DISPON, TFT_CMD_DELAY, 120,
};

// ==== Public functions =========================================================

// == Low level functions; usually not used directly ==
esp_err_t wait_trans_finish(uint8_t free_line);
void disp_spi_transfer_cmd(int8_t cmd);
void disp_spi_transfer_cmd_data(int8_t cmd, uint8_t *data, uint32_t len);
void drawPixel(int16_t x, int16_t y, color_t color, uint8_t sel);
void send_data(int x1, int y1, int x2, int y2, uint32_t len, color_t *buf);
void TFT_pushColorRep(int x1, int y1, int x2, int y2, color_t data, uint32_t len);
int read_data(int x1, int y1, int x2, int y2, int len, uint8_t *buf, uint8_t set_sp);
color_t readPixel(int16_t x, int16_t y);
int touch_get_data(uint8_t type);


// Deactivate display's CS line
//========================
esp_err_t disp_deselect();

// Activate display's CS line and configure SPI interface if necessary
//======================
esp_err_t disp_select();


// Find maximum spi clock for successful read from display RAM
// ** Must be used AFTER the display is initialized **
//======================
uint32_t find_rd_speed();


// Change the screen rotation.
// Input: m new rotation value (0 to 3)
//=================================
void _tft_setRotation(uint8_t rot);

// Initialize all pins used by display driver
// ** MUST be executed before SPI interface initialization
//=================
void TFT_PinsInit();

// Perform display initialization sequence
// Sets tft_orientation to landscape; clears the screen
// * All pins must be configured
// * SPI interface must already be setup
// * 'tft_disp_type', 'COLOR_BITS', 'tft_width', 'tft_height' variables must be set
//======================
void TFT_display_init();

#ifdef __cplusplus
}
#endif

#endif /* MAIN_INCLUDE_TFTSPI_H_ */
