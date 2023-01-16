// ------------------------------------------------------------
// Configuration structure ------------------------------------

struct conf {  
  // Synced values
  float   PHOTONS;
  float   CHROMA;
  float   MOOD;
  float   BASE_HUE;
  uint8_t LIGHTSHOW_MODE;
  bool    MIRROR_ENABLED;

  // Private values
  uint32_t SAMPLE_RATE;
  uint8_t  NOTE_OFFSET;
  uint8_t  SQUARE_ITER;
  uint32_t MAGNITUDE_FLOOR;
  uint8_t  LED_TYPE;
  uint16_t LED_COUNT;
  uint16_t MAX_BLOCK_SIZE;
  uint16_t SAMPLES_PER_CHUNK;
  float    GAIN;
  int32_t  DC_OFFSET; // (TODO)
  uint32_t WAVEFORM_NOISE_FLOOR; // (TODO)
  uint8_t  ESPNOW_CHANNEL; // (TODO)
  bool     IS_MAIN_UNIT;

  uint32_t VERSION;
};

conf CONFIG = { // Defaults of the CONFIG struct  
  // Synced values
  1.00, // PHOTONS
  0.00, // CHROMA
  0.05, // MOOD
  0.00, // BASE_HUE
  LIGHT_MODE_GDFT, // LIGHTSHOW_MODE
  true,            // MIRROR_ENABLED (>= 3.0.0 defaults yes)

  // Private values
  DEFAULT_SAMPLE_RATE, // SAMPLE_RATE
  12,                  // NOTE_OFFSET
  1,                   // SQUARE_ITER
  1000,                // MAGNITUDE_FLOOR
  LED_NEOPIXEL,        // LED_TYPE
  128,                 // LED_COUNT
  1600,                // MAX_BLOCK_SIZE
  256,                 // SAMPLES_PER_CHUNK
  0.0,                 // GAIN
  0,                   // DC_OFFSET
  0,                   // WAVEFORM_NOISE_FLOOR
  3,                   // ESPNOW_CHANNEL
  false,               // IS_MAIN_UNIT

  FIRMWARE_VERSION,    // VERSION
};

conf CONFIG_DEFAULTS; // Used for resetting to default values at runtime

// ------------------------------------------------------------
// Goertzel structure (generated in system.h) -----------------

struct freq {
  float    target_freq;
  int32_t  coeff_q14;

  uint16_t block_size;
  float    block_size_recip;
  uint8_t  zone;
  
  float a_weighting_ratio;
  float window_mult;
};
freq frequencies[NUM_FREQS];

// ------------------------------------------------------------
// Hann window lookup table (generated in system.h) -----------

int16_t window_lookup[2048] = { 0 };

// ------------------------------------------------------------
// A-weighting lookup table (parsed in system.h) --------------

float a_weight_table[13][2] = {
  { 10,    -70.4 },  // hz, db
  { 20,    -50.5 },
  { 40,    -34.6 },
  { 80,    -22.5 },
  { 160,   -13.4 },
  { 315,    -6.6 },
  { 630,    -1.9 },
  { 1000,    0.0 },
  { 1250,    0.6 },
  { 2500,    1.3 },
  { 5000,    0.5 },
  { 10000,  -2.5 },
  { 20000,  -9.3 }
};

// ------------------------------------------------------------
// Spectrograms (GDFT.h) --------------------------------------

float note_spectrogram[NUM_FREQS] = {0};
float note_spectrogram_smooth[NUM_FREQS] = {0};
float note_spectrogram_long_term[NUM_FREQS] = {0};
float note_chromagram[12]  = {0};

float smoothing_follower    = 0.0;
float smoothing_exp_average = 0.0;

// ------------------------------------------------------------
// Audio samples (i2s_audio.h) --------------------------------

int32_t i2s_samples_raw[1024]              = { 0 };
short sample_window[SAMPLE_HISTORY_LENGTH] = { 0 };
short sample_chunk[1024]                   = { 0 };

// ------------------------------------------------------------
// Noise calibration (noise_cal.h) ----------------------------

bool noise_complete = true;
float noise_samples[NUM_FREQS] = { 1 };
uint16_t noise_iterations = 0;

// ------------------------------------------------------------
// Display buffers (led_utilities.h) --------------------------

CRGB leds[128];
CRGB leds_temp[128];
CRGB leds_last[128];
CRGB leds_aux [128];
CRGB leds_fade[128];
CRGB leds_out[STRIP_LED_COUNT];

// ------------------------------------------------------------
// Benchmarking (system.h) ------------------------------------

Ticker cpu_usage;
volatile uint16_t function_id = 0;
volatile uint16_t function_hits[32] = {0};
float FPS = 0.0;

// ------------------------------------------------------------
// SensorySync P2P network (p2p.h) ----------------------------

bool main_override = true;
uint32_t last_rx_time = 0;

// ------------------------------------------------------------
// Buttons (buttons.h) ----------------------------------------

// TODO: Similar structs for knobs
struct button{
  uint8_t pin = 0;
  uint32_t last_down = 0;
  uint32_t last_up = 0;
  bool pressed = false;
};

button noise_button;
button mode_button;

// ------------------------------------------------------------
// Settings tracking (system.h) -------------------------------

uint32_t last_setting_change = 0;
bool settings_updated = false;

// ------------------------------------------------------------
// Serial buffer (serial_menu.h) ------------------------------

char command_buf[128] = {0};
uint8_t command_buf_index = 0;

bool stream_audio = false;
bool stream_fps = false;
bool stream_max_mags = false;
bool stream_max_mags_followers = false;
bool stream_magnitudes = false;
bool stream_spectrogram = false;

bool debug_mode = false;
uint32_t serial_iter = 0;

// ------------------------------------------------------------
// Spectrogram normalization (GDFT.h) -------------------------

float max_mags[NUM_ZONES] = { 0.000 };
float max_mags_followers[NUM_ZONES] = { 0.000 };
float mag_targets[NUM_FREQS] = { 0.000 };
float mag_followers[NUM_FREQS] = { 0.000 };
float mag_float_last[NUM_FREQS] = { 0.000 };
float magnitudes[NUM_FREQS] = { 0.000 };

// ------------------------------------------------------------
// Look-ahead smoothing (GDFT.h) ------------------------------

const uint8_t spectrogram_history_length = 3;
float spectrogram_history[spectrogram_history_length][64];
uint8_t spectrogram_history_index = 0;

// ------------------------------------------------------------
// Used for converting for storage in LittleFS (bridge_fs.h) --

union bytes_32 {
  uint32_t long_val;
  int32_t  long_val_signed;
  float    long_val_float;
  uint8_t  bytes[4];
};

// ------------------------------------------------------------
// Used for GDFT mode (lightshow_modes.h) ---------------------

uint8_t brightness_levels[NUM_FREQS] = { 0 };

// WIP BELOW --------------------------------------------------

float MASTER_BRIGHTHNESS = 0.0;
float last_sample = 0;

float retro_bulbs[NUM_FREQS]        = {0};
float retro_heat_targets[NUM_FREQS] = {0};

float scale_scores[24] = {0.0};
