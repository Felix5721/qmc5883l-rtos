//esp sdk
#include "espressif/esp_common.h"
#include "esp/uart.h"

//rtos
#include "FreeRTOS.h"
#include "task.h"

//device drivers
#include "i2c/i2c.h"
#include "qmc5883l.h"


#define I2C_BUS 0
#define SCL_PIN 4
#define SDA_PIN 5

#define magformat "X:\t%d\nY:\t%d\nZ:\t%d\n--------------------------------------\n"

i2c_dev_t* magnetometer;

void magnetometerReader(void *pvParameters) {
	printf("looking for Magnetometer\n");
	while(!qmc5883l_init(magnetometer)) {
		printf("Device not found!\n");
	}

	qmc5883l_data_t data;
	while(1) {
		if( qmc5883l_get_data(magnetometer, &data) ) {
			printf(magformat, data.x, data.y, data.z);
		} else {
			while(!qmc5883l_init(magnetometer)) {
				printf("Device not found!\n");
			}
		}
		vTaskDelay(100);
	}
}

void user_init(void) {
	uart_set_baud(0, 115200);

	//connected hardware
	i2c_init(I2C_BUS, SCL_PIN, SDA_PIN, I2C_FREQ_100K);
	magnetometer = malloc(sizeof(i2c_dev_t));
	magnetometer->addr = QMC5883L_ADDR;
	magnetometer->bus = I2C_BUS;

	//start tasks
	xTaskCreate(magnetometerReader, "readData", 512, NULL, 2, NULL);
}
