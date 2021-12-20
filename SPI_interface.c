#include "SPI_interface.h"

static void (*callBackAPI)(void);

void SPIx_gInit(SPI_Type* SPIx, uint32_t mode, uint32_t pres, uint32_t FF, uint32_t DL, uint32_t ssnControl) {
		/*SPIx config*/
	FL_SPI_InitTypeDef spiInitStruct = {0};
    FL_GPIO_InitTypeDef gpioInitStruct = {0};
    spiInitStruct.baudRate = pres; // FL_SPI_BAUDRATE_DIV2
    spiInitStruct.bitOrder = FF; // FL_SPI_BIT_ORDER_MSB_FIRST
    spiInitStruct.clockPhase = FL_SPI_PHASE_EDGE1;
    spiInitStruct.clockPolarity = FL_SPI_POLARITY_NORMAL;
    spiInitStruct.dataWidth = DL; // FL_SPI_DATA_WIDTH_8B
    spiInitStruct.mode = mode; // FL_SPI_WORK_MODE_MASTER
    spiInitStruct.softControl = ssnControl; //FL_ENABLE;
    spiInitStruct.transferMode = FL_SPI_TRANSFER_MODE_FULL_DUPLEX;
    
    /*set the SPI configurations and enable the SPI*/
    FL_SPI_Init(SPIx, &spiInitStruct);

    /*GPIO pins config*/
    if(SPIx==SPI1) {
    	gpioInitStruct.pin = FL_GPIO_PIN_2 | FL_GPIO_PIN_3 | FL_GPIO_PIN_4 | FL_GPIO_PIN_5;
	    gpioInitStruct.mode = FL_GPIO_MODE_DIGITAL;
	    gpioInitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	    gpioInitStruct.pull = FL_DISABLE;
	    gpioInitStruct.remapPin = FL_DISABLE;

	    FL_GPIO_Init(GPIOD, &gpioInitStruct);
    }
    else if(SPIx==SPI2) {
    	gpioInitStruct.pin = FL_GPIO_PIN_7 | FL_GPIO_PIN_8 | FL_GPIO_PIN_9 | FL_GPIO_PIN_10;
	    gpioInitStruct.mode = FL_GPIO_MODE_DIGITAL;
	    gpioInitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	    gpioInitStruct.pull = FL_DISABLE;
	    gpioInitStruct.remapPin = FL_DISABLE;

	    FL_GPIO_Init(GPIOC, &gpioInitStruct);
    }
	
}

/*this function is used for Txing and Rxing and Txing only*/

SPI_comm_status_t SPIx_gTX(SPI_Type* SPIx, uint8_t* TxRxBuf, uint32_t BufSize) {
	FL_SPI_SetSSNPin(SPIx, FL_SPI_SSN_LOW);

	if(TxRxBuf != '\0') {
		while(BufSize--) {
			FL_SPI_WriteTXBuff(SPIx, *TxRxBuf++);
		    while(!(FL_SPI_IsActiveFlag_TXBuffEmpty(SPIx)));
		    while(!(FL_SPI_IsActiveFlag_RXBuffFull(SPIx)));
		    FL_SPI_ReadRXBuff(SPIx);
		}
	}

	FL_SPI_SetSSNPin(SPIx, FL_SPI_SSN_HIGH);

	return SPI_COMM_FINISHED;
}

SPI_comm_status_t SPIx_gRX(SPI_Type* SPIx, uint8_t* RxBuf, uint32_t RxBufSize) {
	FL_SPI_SetSSNPin(SPIx, FL_SPI_SSN_LOW);
	if(RxBuf != '\0') {
		while(RxBufSize--) {
			FL_SPI_WriteTXBuff(SPIx, 0x00); // 0x00 is dummy value
	    while(!(FL_SPI_IsActiveFlag_RXBuffFull(SPIx)));
	    *RxBuf++ = FL_SPI_ReadRXBuff(SPIx);
		}
	}
	
	FL_SPI_SetSSNPin(SPIx, FL_SPI_SSN_HIGH);
	return SPI_COMM_FINISHED;
}

void SPI_gSetCallBack(void(*callBack)(void)) {
	callBackAPI = callBack;
}

void SPI_gTest(void) {
	uint8_t wBuffer[8] = {0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};
	uint8_t rBuffer[8];
	SPIx_gInit(SPI1, FL_SPI_WORK_MODE_MASTER, FL_SPI_BAUDRATE_DIV2, FL_SPI_BIT_ORDER_MSB_FIRST, FL_SPI_DATA_WIDTH_8B, FL_ENABLE);
	SPIx_gTX(SPI1, wBuffer, 8);
    SPIx_gRX(SPI1, rBuffer, 8);
}