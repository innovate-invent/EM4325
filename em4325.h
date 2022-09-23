/**
 * EM Microelectronic
 * EM4325 RFID transceiver IC ABI Library
 *
 * Nolan Woods (nolan@i2labs.ca)
 *
 * Please refer to ./LICENSE
 */

#pragma pack(push, 1)

struct tid {
    struct config {
        void reserved1 : 2;
        bool sms_enabled: 1;
        bool temp_sensor_uncalibrated : 1;
        void reserved2 : 2;
    };
    struct em_data {
        bool demo : 1;
        uint8_t uid_word_count : 3;
        uint8_t wafer_lot_digit5 : 4;
        uint8_t wafer_lot_digit4 : 4;
        uint8_t wafer_lot_digit3 : 4;
        uint8_t wafer_lot_digit2 : 4;
        uint8_t wafer_lot_digit1 : 4;
    };
    union {
        struct iso_e0_format {
            void reserved1 : 18;
            config config;
            uint8_t customer_number : 8;
            char serial[] : 32;
            uint16_t crc : 16;
            void reserved2 : 16 * 8;
            em_data em_data;
        } iso_e0;
        struct iso_e3_format {
            void reserved1 : 34;
            config config;
            uint8_t customer_number : 8;
            uint32_t serial : 32;
            uint16_t crc : 16;
            void reserved2 : 16 * 7;
            em_data em_data;
        } iso_e3;
        struct epc_format {
            void reserved1 : 26;
            config config;
            void reserved2 : 16;
            uint8_t customer_number : 8;
            uint32_t serial : 32;
            uint16_t crc : 16;
            void reserved3 : 16 * 7;
            em_data em_data;
        } epc;
        struct total_format {
            void reserved1 : 6;
            uint16_t customer_number : 10;
            uint32_t serial : 32;
            uint16_t crc : 16;
            void reserved2 : 16 * 9;
            em_data em_data;
        } total;
    } tid;
};
static_assert(sizeof tid == 16 * 2, "Compiler failed to pack TID bitfield into expected number of bytes");

enum struct monitor_delay_unit : uint8_t {
    second = 0,
    minute = 1,
    hour = 2,
    sample_interval = 3,
};

enum struct sample_interval_unit : uint8_t {
    second = 0,
    minute = 1,
    hour = 2,
    five_minutes = 3,
};

struct temp_sensor_control {
    void reserved1 : 1;
    bool reset_alarms_enable : 1;
    uint8_t under_temp_samples_threshold : 5;
    signed int under_temp_threshold : 9;
    void reserved2 : 1;
    bool time_stamp_required : 1;
    uint8_t over_temp_samples_threshold : 5;
    signed int over_temp_threshold : 9;
    monitor_delay_unit monitor_delay_unit : 2;
    uint8_t monitor_delay : 6;
    sample_interval_unit sample_interval_unit : 2;
    uint8_t sample_interval : 6;
};
static_assert(sizeof temp_sensor_control == 2 * 3, "Compiler failed to pack temp_sensor_control bitfield into expected number of bytes");

struct temp_sensor_calibration {
    uint16_t preserved : 11;
    signed int fine_trim: 5;
};
static_assert(sizeof temp_sensor_calibration == 2 * 1, "Compiler failed to pack temp_sensor_calibration bitfield into expected number of bytes");

struct io_control {
    bool pull_enable : 1;
    enum struct spi_config_mode {
        disabled = 0,
        slave_half_duplex = 1,
        master_full_duplex = 2,
        master_half_duplex = 3,
    } spi_config_mode : 2;
    uint8_t spi_mode : 2;
    bool aux_enable : 1;
    bool aux_out : 1;
    union {
        struct {
            bool alarms_out : 1;
            bool p3_enable : 1;
            bool p2_enable : 1;
            bool p1_enable : 1;
            bool p0_enable : 1;
            bool p3_output : 1;
            bool p2_output : 1;
            bool p1_output : 1;
            bool p0_output : 1;
        } gpio;
        struct {
            void reserved1 : 1;
            enum struct operation_mode {
                normal = 0,
                signaling_w_monitor = 2,
                signaling_w_comm_buffer = 3,
                rf_modem_state_machine_bypassed = 4,
                rf_modem_state_machine_limited = 6,
                rf_modem_state_machine_full = 7,
            } operation_mode : 3;
            enum struct register_file_pages {
                normal = 0,
                replace_page_1 = 1,
                replace_page_0_1 = 2,
                replace_page_1_2 = 3,
            } register_file_pages : 2;
            bool rf_read_protect_enable : 1;
            bool custom_commands_enable : 1;
            bool kill_command_disable : 1;
        } slave;
    } config;
};
static_assert(sizeof io_control == 2 * 1, "Compiler failed to pack io_control bitfield into expected number of bytes");

struct battery_management {
    uint8_t field_detect_duty : 2;
    uint8_t sleep_timeout : 2;
    uint8_t initial_command_timeout : 4;
    uint8_t idle_timeout_units : 2;
    uint8_t idle_timeout : 4;
    enum struct bap_mode_sensitivity {
        maximum = 0,
        normal = 1,
        degraded = 2,
        minimum = 3,
    } bap_mode_sensitivity : 2;
    enum struct aux_event {
        field_present = 0,
        inventory = 1,
        singulated = 2,
        selected = 3,
    } aux_event : 2;
    bool rtf_idle_timeout_enable : 1;
    bool total_mute_timeout_enable : 1;
    uint8_t sleep_timeout : 3;
    uint8_t total_tagmsg_count : 6;
    bool alarm_blink : 1;
    bool lbd_level : 1;
    bool bap_control_enable : 1;
};
static_assert(sizeof battery_management == 2 * 2, "Compiler failed to pack battery_management bitfield into expected number of bytes");

struct total {
    bool page_link_enable : 1;
    uint8_t fixed_slot_count : 3;
    bool mute_fn_includes_simplified_detection : 1;
    bool adaptive_holdoff_enable : 1;
    bool include_sensor_data : 1;
    enum struct data_encoding_type {
        ppe = 0,
        fm0 = 1,
        miller_m2 = 2,
        miller_m4 = 3,
    } data_encoding_type;
    enum struct backscatter_link_frequency {
        khz128 = 0,
        khz256 = 1,
        khz320 = 2,
        khz512or640 = 3,
    } backscatter_link_frequency;
    uint8_t max_holdoff_time : 2;
    uint8_t initial_listen_time : 3;
};
static_assert(sizeof total == 2 * 1, "Compiler failed to pack total bitfield into expected number of bytes");

struct sensor_data {
    bool low_battery : 1;
    bool aux : 1;
    bool over_temp : 1;
    bool under_temp : 1;
    bool p3 : 1;
    bool monitor_enabled : 1;
    void reserved1 : 1;
    signed int temperature : 9;
    uint8_t aborted_temp_measurements : 6;
    uint8_t under_temp_count : 5;
    uint8_t over_temp_count : 5;
};

typedef void (*rfid_callback)(void* write, size_t write_len, void* read);

union uid {
    uint8_t format : 8;
    struct {
        uint8_t format : 8;
        uint8_t manufacturer : 8;
        uint64_t serial : 48;
    } iso_e0;
    struct {
        uint8_t format : 8;
        uint8_t manufacturer : 8;
        uint16_t user_memory : 16;
        uint64_t serial : 48;
    } iso_e3;
    struct {
        uint8_t format : 8;
        void xtid : 1;
        uint16_t mask_designer : 11;
        uint16_t model : 12;
        uint16_t xtid_header: 16;
        uint64_t serial : 48;
    } epc;
    struct {
        uint8_t format : 8;
        uint8_t mask_designer : 6;
        uint16_t customer : 10;
        uint32_t serial : 32;
        uint16_t crc : 16;
    } total;
};

struct sensor_data_response {
    void reserved1 : 1;

};

enum struct spi_sclk {
    khz40 = 0,
    khz80 = 1,
    khz160 = 2,
    khz320 = 3,
};

enum struct spi_initial_delay {
    sclk = 0,
    uS50 = 1,
    uS500 = 2,
    uS5000 = 3,
};

enum struct spi_delay {
    none = 0,
    uS50 = 1,
    uS100 = 2,
    uS500 = 3,
};

#pragma pack(pop)

const uint16_t crc_init = 0x3791;

uint16_t crc16(void*, size_t);
uid GetUID(rfid_callback);
sensor_data_response GetSensorData(rfid_callback, bool uid, bool new_sample);
void SendSPI(rfid_callback, void* send_buffer, size_t send_buffer_len, void* receive_buffer, size_t receive_buffer_len, spi_initial_delay, spi_delay);
void ResetAlarms(rfid_callback);