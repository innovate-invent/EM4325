/**
 * EM Microelectronic
 * EM4325 RFID transceiver IC ABI Library
 *
 * Nolan Woods (nolan@i2labs.ca)
 *
 * Please refer to ./LICENSE
 */

#import <cstdlib>
#import "em4325.h"

uint16_t crc16(void* buffer, size_t size) {
    uint16_t crc = crc_init;
    for (size_t i = 0; i < size; ++i) {
        crc ^= buffer[i];
        for (uint8_t j = 0; j < 8; ++i) {
            crc >>= 1;
            if (crc & 0x0001) crc ^= 0x8408;
        }
    }
    return crc;
}

uid GetUID(rfid_callback) {
    uint16 buffer[3] = {0xE000, rand() % 0xFFFF, 0};
    buffer[2] = crc16(&buffer, 2);
    uint8_t response[17];
    *rfid_callback(&buffer, sizeof buffer, &response);

}

sensor_data_response GetSensorData(rfid_callback, bool uid, bool new_sample) {

}

void SendSPI(rfid_callback, void* send_buffer, size_t send_buffer_len, void* receive_buffer, size_t receive_buffer_len, spi_initial_delay, spi_delay) {

}

void ResetAlarms(rfid_callback) {

}