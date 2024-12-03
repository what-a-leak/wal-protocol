#include "ssd1306.h"

#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000

static ssd1306_handle_t ssd1306_dev = NULL;

void screen_init(gpio_num_t scl, gpio_num_t sda, uint32_t speed)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .master.clk_speed = speed,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL};
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    ssd1306_dev = ssd1306_create(I2C_MASTER_NUM, SSD1306_I2C_ADDRESS);
    ssd1306_refresh_gram(ssd1306_dev);
    ssd1306_clear_screen(ssd1306_dev, 0x00);
}

void screen_draw()
{
    char data_str[10] = {0};
    sprintf(data_str, "Z STR");
    ssd1306_draw_string(ssd1306_dev, 70, 16, (const uint8_t *)data_str, 16, 1);
    ssd1306_refresh_gram(ssd1306_dev);
}