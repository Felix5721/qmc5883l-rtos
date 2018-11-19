#include "qmc5883l.h"

#define QMC5883L_MEASUREMENT_TIMEOUT 100
#define timeout_expired(start, len) ((uint32_t)(sdk_system_get_time() - (start)) >= (len))

bool qmc5883l_init(i2c_dev_t *dev) {
	return qmc5883l_init_all(dev, NULL, NULL, NULL, NULL);
}

bool qmc5883l_init_all(i2c_dev_t *dev, qmc5883l_mode_t *mode, qmc5883l_rate_t *rate, qmc5883l_range_t *range, qmc5883l_oversample_t *os) {
	qmc5883l_mode_t m = QMC5883L_CONFIG_CONT;
	if (mode != NULL) {
		m = *mode;
	}
	qmc5883l_rate_t rt = QMC5883L_CONFIG_200HZ;
	if (rate != NULL) {
		rt = *rate;
	}
	qmc5883l_range_t rg = QMC5883L_CONFIG_8GAUSS;
	if (range != NULL) {
		rg = *range;
	}
	qmc5883l_oversample_t osmpl = QMC5883L_CONFIG_OS512;
	if (os != NULL) {
		osmpl = *os;
	}
	uint8_t config = ((uint8_t)(osmpl)<<6) | ((uint8_t)(rg)<<4) | ((uint8_t)(rt)<<2) | (uint8_t)(m);
	uint8_t reg = (uint8_t)QMC5883L_CONFIG; 
	qmc5883l_reset(dev);
	if(i2c_slave_write(dev->bus, dev->addr, &reg, &config, 1)){
		return false;
	}
	return true;
}

bool qmc5883l_data_ready(i2c_dev_t *dev) {
	uint8_t res = 0;
	uint8_t reg = (uint8_t)QMC5883L_STATUS;
	i2c_slave_read(dev->bus, dev->addr, &reg, &res, 1);

	if (res & QMC5883L_STATUS_DRDY) {
		return true;
	}
	return false;
}

bool qmc5883l_get_data(i2c_dev_t *dev, qmc5883l_data_t *data){
	//check if device is operating and if data is ready;
	if(qmc5883l_get_mode(dev) == QMC5883L_CONFIG_STANDBY) {
		return false;
	} 
	uint32_t start = sdk_system_get_time();
	while (!qmc5883l_data_ready(dev)) {
		if(timeout_expired(start, QMC5883L_MEASUREMENT_TIMEOUT)) {
			return false;
		}
	}
	//read data from device
	uint8_t buf[6];
	uint8_t reg = (uint8_t)QMC5883L_X_LSB;
	i2c_slave_read(dev->bus, dev->addr, &reg, &buf, 6);
	data->x = (uint16_t)buf[QMC5883L_X_MSB] << 8 | buf[QMC5883L_X_LSB];
	data->y = (uint16_t)buf[QMC5883L_Y_MSB] << 8 | buf[QMC5883L_Y_LSB];
	data->z = (uint16_t)buf[QMC5883L_Z_MSB] << 8 | buf[QMC5883L_Z_LSB];
	return true;
}

void qmc5883l_reset(i2c_dev_t *dev) {
	uint8_t val = 1;
	uint8_t reg = (uint8_t)QMC5883L_RESET;
	i2c_slave_write(dev->bus, dev->addr, &reg, &val, 1);
}

uint8_t qmc5883l_get_config(i2c_dev_t *dev) {
	uint8_t res = 0;
	uint8_t reg = (uint8_t)QMC5883L_CONFIG;
	i2c_slave_read(dev->bus, dev->addr, &reg, &res, 1);
	return res;
}

qmc5883l_mode_t qmc5883l_get_mode(i2c_dev_t *dev) {
	uint8_t config = qmc5883l_get_config(dev);
	qmc5883l_mode_t mode = config & 0b00000011;
	return mode;
}

void qmc5883l_set_mode(i2c_dev_t *dev, qmc5883l_mode_t mode) {
	uint8_t config = qmc5883l_get_config(dev);
	config = (config & 0b11111100) | mode;
	uint8_t reg = (uint8_t)QMC5883L_CONFIG; 
	i2c_slave_write(dev->bus, dev->addr, &reg, &config, 1);
}

qmc5883l_rate_t qmc5883l_get_rate(i2c_dev_t *dev) {
	uint8_t config = qmc5883l_get_config(dev);
	qmc5883l_rate_t rate = (config & 0b00001100) >> 2;
	return rate;
}

void qmc5883l_set_rate(i2c_dev_t *dev, qmc5883l_rate_t rate) {
	uint8_t config = qmc5883l_get_config(dev);
	config = (config & 0b11110011) | ((uint8_t)rate << 2);
	uint8_t reg = (uint8_t)QMC5883L_CONFIG; 
	i2c_slave_write(dev->bus, dev->addr, &reg, &config, 1);
}

qmc5883l_range_t qmc5883l_get_range(i2c_dev_t *dev) {
	uint8_t config = qmc5883l_get_config(dev);
	qmc5883l_range_t range = (config & 0b00110000) >> 4;
	return range;
}

void qmc5883l_set_range(i2c_dev_t *dev, qmc5883l_range_t range) {
	uint8_t config = qmc5883l_get_config(dev);
	config = (config & 0b11001111) | ((uint8_t)range << 4);
	uint8_t reg = (uint8_t)QMC5883L_CONFIG; 
	i2c_slave_write(dev->bus, dev->addr, &reg, &config, 1);
}

qmc5883l_oversample_t qmc5883l_get_oversample(i2c_dev_t *dev) {
	uint8_t config = qmc5883l_get_config(dev);
	qmc5883l_oversample_t oversample = (config & 0b11000000) >> 6;
	return oversample;
}

void qmc5883l_set_oversample(i2c_dev_t *dev, qmc5883l_oversample_t oversample) {
	uint8_t config = qmc5883l_get_config(dev);
	config = (config & 0b00111111) | ((uint8_t)oversample << 6);
	uint8_t reg = (uint8_t)QMC5883L_CONFIG; 
	i2c_slave_write(dev->bus, dev->addr, &reg, &config, 1);
}
