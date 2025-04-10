/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2022 <ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __RECORDER_SR_H__
#define __RECORDER_SR_H__

#include "esp_afe_sr_iface.h"
#include "esp_err.h"
#include "recorder_sr_iface.h"
#include "esp_mn_models.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FEED_TASK_STACK_SZ       (7 * 1024)
#define FETCH_TASK_STACK_SZ      (5 * 1024)
#define FEED_TASK_PRIO           (5)
#define FETCH_TASK_PRIO          (5)
#define FEED_TASK_PINNED_CORE    (0)
#define FETCH_TASK_PINNED_CORE   (1)
#define SR_OUTPUT_RB_SIZE        (6 * 1024)

/**
 * @brief SR processor handle
 */
typedef void *recorder_sr_handle_t;

/**
 * @brief SR processor configuration
 * @note  Since the detection of command words requires a clear starting point,
 *        the moment the wake word is detected is taken as the default start of detection.
 *        Therefore, if the wake word detection is disabled,
 *        the detection will use the `vad_state` detected by `esp-sr` as the start of detection.
 *        However, due to the fluctuation of this `vad_state`,
 *        the effectiveness of command word detection will be limited.
 */
typedef struct {
    afe_config_t *afe_cfg;                              /*!< Configuration of AFE */
    bool         multinet_init;                         /*!< Enable of speech command recognition */
    int          feed_task_core;                        /*!< Core id of feed task */
    int          feed_task_prio;                        /*!< Priority of feed task*/
    int          feed_task_stack;                       /*!< Stack size of feed task */
    int          fetch_task_core;                       /*!< Core id of fetch task */
    int          fetch_task_prio;                       /*!< Priority of fetch task */
    int          fetch_task_stack;                      /*!< Stack size of fetch task */
    int          rb_size;                               /*!< Ringbuffer size of recorder sr */
    char         *partition_label;                      /*!< Partition label which stored the model data */
    char         *mn_language;                          /*!< Command language for multinet to load */
    char         *wn_wakeword;                          /*!< Wake Word for WakeNet to load. This is useful when multiple Wake Words are selected in sdkconfig. Setting this to NULL will use the first found model. */
} recorder_sr_cfg_t;

#define DEFAULT_RECORDER_SR_CFG(fmt, partition, sr_type, afe_mode) {                  \
    .afe_cfg = afe_config_init(fmt, esp_srmodel_init(partition), sr_type, afe_mode),  \
    .multinet_init = true,                                                            \
    .feed_task_core = FEED_TASK_PINNED_CORE,                                          \
    .feed_task_prio = FEED_TASK_PRIO,                                                 \
    .feed_task_stack = FEED_TASK_STACK_SZ,                                            \
    .fetch_task_core = FETCH_TASK_PINNED_CORE,                                        \
    .fetch_task_prio = FETCH_TASK_PRIO,                                               \
    .fetch_task_stack = FETCH_TASK_STACK_SZ,                                          \
    .rb_size = SR_OUTPUT_RB_SIZE,                                                     \
    .partition_label = partition,                                                     \
    .mn_language = ESP_MN_CHINESE,                                                    \
    .wn_wakeword = NULL,                                                              \
}

/**
 * @brief Initialize sr processor, and the sr is disabled as default.
 *
 * @param cfg   Configuration of sr
 * @param iface User interface provide by recorder sr
 *
 * @return NULL    failed
 *         Others  SR handle
 */
recorder_sr_handle_t recorder_sr_create(recorder_sr_cfg_t *cfg, recorder_sr_iface_t **iface);

/**
 * @brief Destroy SR processor and recycle all resource
 *
 * @param handle SR processor handle
 *
 * @return ESP_OK
 *         ESP_FAIL
 */
esp_err_t recorder_sr_destroy(recorder_sr_handle_t handle);

/**
 * @brief Reset the speech commands
 *
 * @param handle        SR processor handle
 * @param command_str   String of the commands. more details on `https://github.com/espressif/esp-sr/blob/release/v1.0/docs/speech_command_recognition/README.md#2reset-api-on-the-fly`
 * @param err_phrase_id error string output
 *
 * @return ESP_OK
 *         ESP_FAIL
 */
esp_err_t recorder_sr_reset_speech_cmd(recorder_sr_handle_t handle, char *command_str, char *err_phrase_id);

#ifdef __cplusplus
}
#endif

#endif /*__RECORDER_SR_H__*/
