#include "drv_i2c.h"
#include "platform.h"
#include "defines.h"

static void MX_I2C2_Init(void);

I2C_HandleTypeDef hi2c2;

void drv_i2c_init(void)
{
	MX_I2C2_Init();
}

void drv_i2c_tx(uint16_t DevAddress, uint8_t *data, uint16_t size)
{
	HAL_I2C_Master_Transmit(&hi2c2, DevAddress, data, size, 1000);
}

void drv_i2c_rx(uint16_t DevAddress, uint8_t *data, uint16_t size)
{
	HAL_I2C_Master_Receive(&hi2c2, DevAddress, data, size, 1000);
}


/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00303D5B;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
	  PANIC;
  }

  // Configure Analogue filter
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
	  PANIC;
  }

  // Configure Digital filter
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) {
	  PANIC;
  }
}
