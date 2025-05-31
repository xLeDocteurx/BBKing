#include <driver/i2s.h>
// #include <driver/i2s_common.h>
// #include <driver/i2s_std.h>
// #include <driver/i2s_pdm.h>
// #include <driver/i2s_tdm.h>

#include <Defs.h>
#include <Init.h>

bool initI2S(State *statePointer)
// bool initI2S(i2s_chan_handle_t *tx_handle, i2s_chan_handle_t *rx_handle)
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = DMA_WAV_SAMPLE_RATE,
        .bits_per_sample = DMA_BITS_PER_SAMPLE,
        .channel_format = DMA_CHANNEL_FORMAT,
        // .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
        .dma_buf_count = DMA_WAV_BUFFER_COUNT,
        .dma_buf_len = DMA_WAV_BUFFER_SIZE,

        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = -1,

        // .mclk_multiple = I2S_MCLK_MULTIPLE_128,
        // .bits_per_chan = I2S_BITS_PER_CHAN_16BIT
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

    // // TODO : error handling

    // /* Allocate a pair of I2S channel */
    // i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    // /* Allocate for TX and RX channel at the same time, then they will work in full-duplex mode */
    // // i2s_new_channel(&chan_cfg, &(statePointer->tx_handle), NULL);
    // // i2s_new_channel(&chan_cfg, tx_handle, NULL);
    // // i2s_new_channel(&chan_cfg, &(statePointer->tx_handle), &(statePointer->rx_handle));
    // i2s_new_channel(&chan_cfg, tx_handle, rx_handle);

    // /* Set the configurations for BOTH TWO channels, since TX and RX channel have to be same in full-duplex mode */
    // i2s_std_config_t std_cfg = {
    //     // .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(32000),
    //     // .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(48000),
    //     .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(DMA_WAV_SAMPLE_RATE),
    //     // .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
    //     .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
    //     .gpio_cfg = {
    //         // .mclk = I2S_GPIO_UNUSED,
    //         .mclk = GPIO_NUM_0,
    //         // .bclk = GPIO_NUM_4,
    //         // .bclk = I2S_BCK_IO_NUM,
    //         .bclk = I2S_BCK_IO_NUM,
    //         // .ws = GPIO_NUM_5,
    //         // .ws = I2S_WS_IO_NUM,
    //         .ws = I2S_WS_IO_NUM,
    //         // .dout = GPIO_NUM_18,
    //         // .dout = I2S_DATA_OUT_NUM,
    //         .dout = I2S_DATA_OUT_NUM,
    //         // .din = GPIO_NUM_19,
    //         .din = I2S_GPIO_UNUSED,
    //         .invert_flags = {
    //             .mclk_inv = false,
    //             .bclk_inv = false,
    //             .ws_inv = false,
    //         },
    //     },
    // };
    // // i2s_channel_init_std_mode(statePointer->tx_handle, &std_cfg);
    // // i2s_channel_init_std_mode(statePointer->rx_handle, &std_cfg);
    // i2s_channel_init_std_mode(*tx_handle, &std_cfg);
    // i2s_channel_init_std_mode(*rx_handle, &std_cfg);

    // // i2s_channel_enable(statePointer->tx_handle);
    // // i2s_channel_enable(statePointer->rx_handle);

    return true;
}
