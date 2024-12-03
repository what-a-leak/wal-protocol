#include <stdio.h>
#include "serial.h"
#include "ssd1306.h"

#define I2C_MASTER_SCL_IO 6        /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 7        /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0    /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000   /*!< I2C master clock frequency */

static ssd1306_handle_t ssd1306_dev = NULL;

void screen() {
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    ssd1306_dev = ssd1306_create(I2C_MASTER_NUM, SSD1306_I2C_ADDRESS);
    ssd1306_refresh_gram(ssd1306_dev);
    ssd1306_clear_screen(ssd1306_dev, 0x00);

    char data_str[10] = {0};
    sprintf(data_str, "X STR");
    ssd1306_draw_string(ssd1306_dev, 70, 16, (const uint8_t *)data_str, 16, 1);
    ssd1306_refresh_gram(ssd1306_dev);
}

void app_main(void)
{
    /* init m8 */
    serial_init(9600, UART_NUM_0);
    screen();

    /* loop */
    const char *message = "Hello, World!\n";
    while (1)
    {
        uart_write_bytes(UART_NUM_0, message, 15);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
    }
}
