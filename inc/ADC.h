/******************************************************************************
 * This file is a part of the Music Sequencer Project for SM2                 *
 ******************************************************************************/

/**
 * @file ADC.h
 * @author Kumka, Potoczek
 * @date Jan 2021
 * @brief File containing declarations for ADC
 * @ver 0.1
 */

#include "MKL05Z4.h"
//----------------------------------------------
// ADC0_CFG1
//----------------------------------------------
// clock divisor
#define ADIV_1            0x00
#define ADIV_2            0x01
#define ADIV_4            0x02
#define ADIV_8            0x03
// resolution (single ended).
#define MODE_8            0x00
#define MODE_12           0x01
#define MODE_10           0x02
//#define MODE_16           0x03

// clock
#define ADICLK_BUS        0x00
#define ADICLK_BUS_2      0x01
#define ADICLK_ALTCLK     0x02
#define ADICLK_ADACK      0x03
//---------------------------------------------
// ADC0_CFG2
//---------------------------------------------
// sampling time length
#define ADLSTS_20          0x00
#define ADLSTS_12          0x01
#define ADLSTS_6           0x02
#define ADLSTS_2           0x03
//-----------------------------------------------
//ADCSC2
//-----------------------------------------------
// reference voltage
#define REFSEL_EXT         0x00
#define REFSEL_ALT         0x01
#define REFSEL_RES         0x02 
#define REFSEL_RES_EXT     0x03 
//-----------------------------------------------
//ADC0_SC3
//-----------------------------------------------
// averaging
#define AVGS_4             0x00
#define AVGS_8             0x01
#define AVGS_16            0x02
#define AVGS_32            0x03
//************************************************
uint8_t ADC_Init(void);

void triggerADC(void);

uint8_t setSpeed(void);
