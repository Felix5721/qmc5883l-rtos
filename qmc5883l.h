#ifndef qmc5883l_h_INCLUDED
#define qmc5883l_h_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include <i2c/i2c.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* The default I2C address of this chip */
#define QMC5883L_ADDR 0x0D

/* Register numbers */
typedef enum {
	QMC5883L_X_LSB = 0,
	QMC5883L_X_MSB = 1,
	QMC5883L_Y_LSB = 2,
	QMC5883L_Y_MSB = 3,
	QMC5883L_Z_LSB = 4,
	QMC5883L_Z_MSB = 5,
	QMC5883L_STATUS = 6,
	QMC5883L_TEMP_LSB = 7,
	QMC5883L_TEMP_MSB = 8,
	QMC5883L_CONFIG = 9,
	QMC5883L_CONFIG2 = 10,
	QMC5883L_RESET = 11,
	QMC5883L_RESERVED = 12,
	QMC5883L_CHIP_ID = 13
} qmc5883l_register_t;

/* Bit values for the STATUS register */
typedef enum {
	QMC5883L_STATUS_DRDY = 1,
	QMC5883L_STATUS_OVL = 2,
	QMC5883L_STATUS_DOR = 4
} qmc5883l_state_t;

/* Oversampling values for the CONFIG register */
typedef enum {
	QMC5883L_CONFIG_OS512 = 0,
	QMC5883L_CONFIG_OS256 = 1,
	QMC5883L_CONFIG_OS128 = 2,
	QMC5883L_CONFIG_OS64  = 3
} qmc5883l_oversample_t;

/* Range values for the CONFIG register */
typedef enum {
	QMC5883L_CONFIG_2GAUSS = 0,
	QMC5883L_CONFIG_8GAUSS = 1
} qmc5883l_range_t;

/* Rate values for the CONFIG register */
typedef enum {
	QMC5883L_CONFIG_10HZ   = 0, 
	QMC5883L_CONFIG_50HZ   = 1,
	QMC5883L_CONFIG_100HZ  = 2,
	QMC5883L_CONFIG_200HZ  = 3 
} qmc5883l_rate_t;

/* Mode values for the CONFIG register */
typedef enum {
	QMC5883L_CONFIG_STANDBY = 0,
	QMC5883L_CONFIG_CONT = 1
} qmc5883l_mode_t;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} qmc5883l_data_t;

bool qmc5883l_init(i2c_dev_t *dev);
bool qmc5883l_init_all(i2c_dev_t *dev, qmc5883l_mode_t *mode, qmc5883l_rate_t *rate, qmc5883l_range_t *range, qmc5883l_oversample_t *sample);
bool qmc5883l_data_ready(i2c_dev_t *dev);
void qmc5883l_reset(i2c_dev_t *dev);
uint8_t qmc5883l_get_config(i2c_dev_t *dev);

qmc5883l_mode_t qmc5883l_get_mode(i2c_dev_t *dev);
void qmc5883l_set_mode(i2c_dev_t *dev, qmc5883l_mode_t mode);

qmc5883l_oversample_t qmc5883l_get_oversample(i2c_dev_t *dev);
void qmc5883l_set_oversample(i2c_dev_t *dev, qmc5883l_oversample_t os);

qmc5883l_range_t qmc5883l_get_range(i2c_dev_t *dev);
void qmc5883l_set_range(i2c_dev_t *dev, qmc5883l_range_t range);

qmc5883l_rate_t qmc5883l_get_rate(i2c_dev_t *dev);
void qmc5883l_set_rate(i2c_dev_t *dev, qmc5883l_rate_t rate);

bool qmc5883l_get_data(i2c_dev_t *dev, qmc5883l_data_t* data);

#ifdef __cplusplus
}
#endif

#endif // qmc5883l_h_INCLUDED

