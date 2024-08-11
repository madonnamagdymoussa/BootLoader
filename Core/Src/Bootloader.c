/*
 * Bootloader.c
 *
 *  Created on: Aug 9, 2024
 *      Author: DELL
 */

#include "Bootloader.h"

static uint8_t Host_buffer[200];

static uint8_t BL_CRC_verify(uint8_t *pdata, uint32_t DataLen, uint32_t HostCRC);

void BL_SendMessage(char *format, ...){

/*char *format: This is a format string, similar to what you use in printf.
 * It specifies how the subsequent arguments should be formatted (e.g., integers, strings, etc.).*/

/*...: This represents a variable number of arguments.
 * The function can accept additional arguments after the format string,
 * which will be used to populate the format string.*/

 char message[100]={0};

/*va_list args: This is a type used to handle the variable arguments. va_list
 * is a standard way to work with functions that take a variable number of arguments.*/
    va_list args;

/*va_start(args, format): This macro initializes args to retrieve the additional arguments after format.
*The first argument to va_start is the va_list variable, and the second argument is the last fixed parameter
*The  before the variable arguments start (in this case, format).*/
    va_start(args,format);


/*vsprintf(message, format, args): This function formats the string according to the format string and the variable arguments,
 * then stores the result in the message buffer. It's similar to sprintf, but it works with a va_list instead of regular arguments.*/
  vsprintf(message,format,args);


  /*&huart2: This is a pointer to the UART handle structure.
   * huart2 likely represents UART2, meaning the message is sent over the UART2 peripheral.*/

  /*(uint8_t*)message: The message buffer is cast to an uint8_t*,
   * as the UART function expects a pointer to uint8_t (bytes) rather than char*/

  /*sizeof(message): This specifies the number of bytes to send. Since message is 100 bytes,
   * it sends 100 bytes over UART, even if the message is shorter. This could result in
   * transmitting unwanted trailing null bytes.*/

  /*HAL_MAX_DELAY: This is the timeout duration for the transmission.
   *HAL_MAX_DELAY typically means the function will wait indefinitely until the transmission is complete.*/
   HAL_UART_Transmit(&huart2, (uint8_t *)message, sizeof(message), HAL_MAX_DELAY);


   /*va_end(args): This macro cleans up the va_list when you’re done with it.
    *It’s important to call this to avoid undefined behavior.*/
   va_end(args);
}


BL_Status BL_FetchHostCommand(){

	BL_Status status=BL_NACK;
	HAL_StatusTypeDef Hal_status = HAL_ERROR;
	uint8_t DataLen=0;
	Hal_status=HAL_UART_Transmit(&huart2,Host_buffer,1,HAL_MAX_DELAY);

	if(Hal_status != HAL_OK){
		status=BL_NACK;
	}

	else{
		DataLen=Host_buffer[0];
		Hal_status=HAL_UART_Transmit(&huart2,&Host_buffer[1],DataLen,HAL_MAX_DELAY);
		if(Hal_status !=HAL_OK){
			status=BL_NACK;
		}
		else{

			switch(Host_buffer[1])
			{
			case CBL_GET_VER_CMD:
				BL_SendMessage("read the version of bootloader");
				break;

			case CBL_GET_HELP_CMD:
				BL_SendMessage("read the help of bootloader");
				break;

			case CBL_GET_CID_CMD:
				BL_SendMessage("read the id of bootloader");
				break;

			case CBL_GO_TO_ADDR_CMD:
				BL_SendMessage("jump to the address");
				break;

			case CBL_FLASH_ERASE_CMD:
				BL_SendMessage("erase the flash");
				break;

			case CBL_MEM_WRITE_CMD:
				BL_SendMessage("write to flash");
				break;

			default :status =BL_NACK;
			}
		}
	}
}

static uint8_t BL_CRC_verify(uint8_t *pdata, uint32_t DataLen, uint32_t HostCRC){

	uint8_t crc_status=CRC_VERIFING_FAILED;
	uint32_t MCU_CRC=0;
    uint32_t dataBuffer=0;

	for(uint8_t count=0; count<DataLen;count++){
		dataBuffer=(uint32_t)pdata[count];
		MCU_CRC=HAL_CRC_Accumulate(&hcrc,&dataBuffer,1);

	}
	if(HostCRC == MCU_CRC){
		crc_status=CRC_VERIFING_PASS;
	}
	else{
		crc_status=CRC_VERIFING_FAILED;
	}

	return crc_status;
}







