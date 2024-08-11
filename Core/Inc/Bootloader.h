/*
 * Bootloader.h
 *
 *  Created on: Aug 9, 2024
 *      Author: DELL
 */

#ifndef INC_BOOTLOADER_H_
#define INC_BOOTLOADER_H_

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "usart.h"

#include "crc.h"

#define CBL_GET_VER_CMD            0x10
#define CBL_GET_HELP_CMD           0x11
#define CBL_GET_CID_CMD            0x12
#define CBL_GO_TO_ADDR_CMD         0x14
#define CBL_FLASH_ERASE_CMD        0x15
#define CBL_MEM_WRITE_CMD          0x16

#define CRC_VERIFING_FAILED        0x00
#define CRC_VERIFING_PASS          0x01

#define HOSTM_MAX_SIZE             200

#define SEND_NACK                  0x00
#define SEND_ACK                   0x01

#define CBL_VENDOR_ID              100
#define CBL_SW_MAJOR_VERSION       1
#define CBL_SW_MINOR_VERSION       1
#define CBL_SW_PATCH_VERSION       0

typedef enum{
	BL_NACK=0,
	BL_ACK
}BL_Status;


void BL_SendMessage(char *format, ...);
BL_Status BL_FetchHostCommand();

#endif /* INC_BOOTLOADER_H_ */
