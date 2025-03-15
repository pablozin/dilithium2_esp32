#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_cpu.h"
#include "randombytes.h"
#include "sign.h"
#include "taskpriorities.h"

#define MLEN 59
#define CTXLEN 15
#define NTESTS 10

static const char *TAG = "test_dilithium_perf";

TaskFunction_t test_dilithium_performance_task(void *pvParameters) {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    size_t i, j;
    int ret;
    size_t mlen, smlen;
    uint8_t b;
    uint8_t ctx[CTXLEN] = {0};
    uint8_t m[MLEN + CRYPTO_BYTES];
    uint8_t m2[MLEN + CRYPTO_BYTES];
    uint8_t sm[MLEN + CRYPTO_BYTES];
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    esp_cpu_cycle_count_t cycles_keypair, cycles_sign, cycles_open, start_cycles;

    snprintf((char*)ctx, CTXLEN, "test_dilithium");

    ESP_LOGI(TAG, "Dilithium Performance Test task started");
    ESP_LOGI(TAG, "NTESTS = %d", NTESTS);

    
    const uint32_t cpu_freq_hz = 160000000; //Frequency in Hz to measure the time of keypair , change it in menuconfig
  //Running iterations ensures robustness, identifies errors, and provides accurate performance measurements, ensuring reliable and secure cryptographic operations.
    for(i = 0; i < NTESTS; ++i) {
        randombytes(m, MLEN);

        start_cycles = esp_cpu_get_cycle_count();
        crypto_sign_keypair(pk, sk);
        cycles_keypair = esp_cpu_get_cycle_count() - start_cycles;

        start_cycles = esp_cpu_get_cycle_count();
        crypto_sign(sm, &smlen, m, MLEN, ctx, CTXLEN, sk);
        cycles_sign = esp_cpu_get_cycle_count() - start_cycles;

        start_cycles = esp_cpu_get_cycle_count();
        ret = crypto_sign_open(m2, &mlen, sm, smlen, ctx, CTXLEN, pk);
        cycles_open = esp_cpu_get_cycle_count() - start_cycles;

        if(ret) {
            ESP_LOGE(TAG, "Verification failed in iteration %d", i);
            goto fail;
        }
        if(smlen != MLEN + CRYPTO_BYTES) {
            ESP_LOGE(TAG, "Signed message lengths wrong in iteration %d", i);
            goto fail;
        }
        if(mlen != MLEN) {
            ESP_LOGE(TAG, "Message lengths wrong in iteration %d", i);
            goto fail;
        }
        for(j = 0; j < MLEN; ++j) {
            if(m2[j] != m[j]) {
                ESP_LOGE(TAG, "Messages don't match in iteration %d", i);
                goto fail;
            }
        }

        randombytes((uint8_t *)&j, sizeof(j));
        do {
            randombytes(&b, 1);
        } while(!b);
        sm[j % (MLEN + CRYPTO_BYTES)] += b;
        ret = crypto_sign_open(m2, &mlen, sm, smlen, ctx, CTXLEN, pk);
        if(!ret) {
            ESP_LOGE(TAG, "Trivial forgeries possible in iteration %d", i);
            goto fail;
        }

        // Convert cycles to time in ms 
        float keypair_time_ms = (float)cycles_keypair / cpu_freq_hz * 1000;
        float sign_time_ms = (float)cycles_sign / cpu_freq_hz * 1000;
        float open_time_ms = (float)cycles_open / cpu_freq_hz * 1000;
        //output
        ESP_LOGI(TAG, "Iteration %d:", i);
        ESP_LOGI(TAG, "  Keypair time:    %.3f ms", keypair_time_ms);
        ESP_LOGI(TAG, "  Sign time:       %.3f ms", sign_time_ms);
        ESP_LOGI(TAG, "  Open time:       %.3f ms", open_time_ms);
        vTaskDelay(pdMS_TO_TICKS(1));

    }

    ESP_LOGI(TAG, "------------------------------------");
    ESP_LOGI(TAG, "Performance Test PASS");
    ESP_LOGI(TAG, "CRYPTO_PUBLICKEYBYTES = %d", CRYPTO_PUBLICKEYBYTES);
    ESP_LOGI(TAG, "CRYPTO_SECRETKEYBYTES = %d", CRYPTO_SECRETKEYBYTES);
    ESP_LOGI(TAG, "CRYPTO_BYTES = %d", CRYPTO_BYTES);

    goto end;

fail:
    ESP_LOGE(TAG, "Performance Test FAIL!");

end:
    vTaskDelete(NULL);
    return 0;
}

void app_main(void)
{
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;

    xReturned = xTaskCreatePinnedToCore(
                    test_dilithium_performance_task, /* Function that implements the task. */
                    "Pablo", /* Text name for the task. */
                    80000, /* Stack size - keep large! */
                    ( void * ) 1, /* Parameter passed into the task. */
                    1, /*MAIN_TASK_PRIORITY */
                    &xHandle, /* Used to pass out the created task's handle. */
                    0); /* Core ID */

    if( xReturned == pdPASS )
    {
        ESP_LOGI(TAG, "Performance task created successfully");
    } else {
        ESP_LOGE(TAG, "Failed performance");
    }

    return;
}
