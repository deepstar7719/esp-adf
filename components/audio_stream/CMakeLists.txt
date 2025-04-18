set(COMPONENT_SRCS "fatfs_stream.c"
                    "http_stream.c"
                    "http_playlist.c"
                    "raw_stream.c"
                    "spiffs_stream.c"
                    "tone_stream.c"
                    "tcp_client_stream.c"
                    "embed_flash_stream.c"
                    "pwm_stream.c")
set(COMPONENT_ADD_INCLUDEDIRS "include")

set(COMPONENT_PRIV_INCLUDEDIRS "lib/hls/include" "lib/gzip/include")
list(APPEND COMPONENT_SRCS  "lib/hls/hls_parse.c"
                            "lib/hls/hls_playlist.c"
                            "lib/hls/line_reader.c"
                            "lib/hls/join_path.c")

if("${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}" VERSION_LESS "5.0")
    list(APPEND COMPONENT_SRCS  "i2s_stream.c")
else()
    list(APPEND COMPONENT_SRCS  "i2s_stream_idf5.c")
endif()


list(APPEND COMPONENT_SRCS  "lib/gzip/gzip_miniz.c")

set(COMPONENT_REQUIRES audio_pipeline driver audio_sal esp_http_client tcp_transport spiffs audio_board esp-adf-libs bootloader_support esp_dispatcher esp_actions tone_partition mbedtls)

if((${IDF_TARGET} STREQUAL "esp32") OR (${IDF_TARGET} STREQUAL "esp32s3") OR (${IDF_TARGET} STREQUAL "esp32p4")  OR (${IDF_TARGET} STREQUAL "esp32c5"))
list(APPEND COMPONENT_SRCS "aec_stream.c")
list(APPEND COMPONENT_REQUIRES esp-sr)
if((${IDF_TARGET} STREQUAL "esp32") OR (${IDF_TARGET} STREQUAL "esp32s3") OR (${IDF_TARGET} STREQUAL "esp32p4"))
    list(APPEND COMPONENT_SRCS "algorithm_stream.c" "tts_stream.c")
endif()
endif()

register_component()
