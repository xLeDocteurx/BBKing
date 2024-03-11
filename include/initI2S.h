#include <driver/i2s.h>

#include <GlobalVars.h>

bool initI2S()
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = DMA_WAV_SAMPLE_RATE,
        .bits_per_sample = DMA_BITS_PER_SAMPLE,
        .channel_format = DMA_CHANNEL_FORMAT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
        .dma_buf_count = DMA_WAV_BUFFER_COUNT,
        .dma_buf_len = DMA_WAV_BUFFER_SIZE,

        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = -1,
    };

    esp_err_t ret_i2s_driver_install = i2s_driver_install(
        I2S_NUM_0,
        &i2s_config, 0, NULL);
    if (ret_i2s_driver_install != ESP_OK)
    {
        if (ret_i2s_driver_install == ESP_FAIL)
        {
            printf("Failed to install I2S driver\n");
            return false;
        }
        else
        {
            printf("Failed to install I2S driver (%s)\n", esp_err_to_name(ret_i2s_driver_install));
            return false;
        }
    }

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCK_IO_NUM,
        .ws_io_num = I2S_WS_IO_NUM,
        .data_out_num = I2S_DATA_OUT_NUM,
        .data_in_num = I2S_PIN_NO_CHANGE // we are not interested in I2S data into the ESP32
    };

    esp_err_t ret_i2s_set_pin = i2s_set_pin(
        I2S_NUM_0,
        &pin_config);

    if (ret_i2s_set_pin != ESP_OK)
    {
        if (ret_i2s_set_pin == ESP_FAIL)
        {
            printf("Failed to set I2S pins\n");
            return false;
        }
        else
        {
            printf("Failed to I2S pins (%s)\n", esp_err_to_name(ret_i2s_set_pin));
            return false;
        }
    }

    return true;
}
