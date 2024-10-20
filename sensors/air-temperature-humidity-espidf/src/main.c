#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"

#include "memory.h"
#include "wifi.h"
#include "mqtt.h"

#define I2C_MASTER_SCL_IO GPIO_NUM_22 /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO GPIO_NUM_21 /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_1      /*!< I2C master i2c port number */
#define I2C_MASTER_FREQ_HZ 100000     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0   /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0   /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS 1000

#define SHT35_SENSOR_ADDR 0x44   /*!< SHT35 sensor address */
#define SHT35_CMD_READOUT 0x2C06 /*!< SHT35 readout command */

static const char *TAG = "Sht35";

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
            .clk_speed = I2C_MASTER_FREQ_HZ}};
    i2c_param_config(I2C_MASTER_NUM, &conf);
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode,
                              I2C_MASTER_RX_BUF_DISABLE,
                              I2C_MASTER_TX_BUF_DISABLE, 0);
}

static esp_err_t sht35_read_temp_hum(float *temperature, float *humidity)
{
    uint8_t data[6];
    esp_err_t ret;

    // Send measurement command
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT35_SENSOR_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, (SHT35_CMD_READOUT >> 8) & 0xFF, true);
    i2c_master_write_byte(cmd, SHT35_CMD_READOUT & 0xFF, true);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error sending command: %s", esp_err_to_name(ret));
        return ret;
    }

    // Delay to allow measurement to complete
    vTaskDelay(pdMS_TO_TICKS(50));

    // Read measurement data
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT35_SENSOR_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, sizeof(data), I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error reading data: %s", esp_err_to_name(ret));
        return ret;
    }

    uint16_t raw_temp = (data[0] << 8) | data[1];
    uint16_t raw_hum = (data[3] << 8) | data[4];

    *temperature = -45.0 + 175.0 * ((float)raw_temp / 65535.0);
    *humidity = 100.0 * ((float)raw_hum / 65535.0);

    return ESP_OK;
}

void app_main(void)
{
    initMemory();
    wifi_init_sta();
    mqtt_app_start();

    ESP_ERROR_CHECK(i2c_master_init());

    float temperature, humidity;
    while (1)
    {
        if (sht35_read_temp_hum(&temperature, &humidity) == ESP_OK)
        {
            ESP_LOGI(TAG, "Temperature: %.2f°C, Humidity: %.2f%%", temperature, humidity);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read data from SHT35 sensor");
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}