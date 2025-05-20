#include <lab_inferencing.h>               // Edge Impulse model interface
#include <Adafruit_MPU6050.h>              // MPU6050 sensor driver
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <flatbuffers/flatbuffers.h>

Adafruit_MPU6050 mpu;

#define SAMPLE_RATE_MS 10                  // Sample every 10 milliseconds
#define CAPTURE_DURATION_MS 1000           // Capture gesture data for 1 second
#define FEATURE_SIZE EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE
#define BUTTON_PIN 3                       // Define button pin (adjust to your setup)

unsigned long last_sample_time = 0;
unsigned long capture_start_time = 0;
int sample_count = 0;
bool capturing = false;

float features[FEATURE_SIZE];

// Edge Impulse callback to access feature data
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);     // Set button pin as input with pull-up

    Serial.println("Initializing MPU6050...");
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) delay(10);
    }

    // Configure sensor range and filtering
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("MPU6050 initialized successfully");
}

void loop() {
    // Wait for button press to start capturing gesture
    if (digitalRead(BUTTON_PIN) == LOW && !capturing) {
        capturing = true;
        sample_count = 0;
        capture_start_time = millis();
        last_sample_time = millis();
        Serial.println("Button pressed, start capture");
    }

    if (capturing) {
        capture_accelerometer_data();
    }
}

// Collect accelerometer data into features[] array
void capture_accelerometer_data() {
    if (millis() - last_sample_time >= SAMPLE_RATE_MS) {
        last_sample_time = millis();

        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        if (sample_count < FEATURE_SIZE / 3) {
            int idx = sample_count * 3;
            features[idx] = a.acceleration.x;
            features[idx + 1] = a.acceleration.y;
            features[idx + 2] = a.acceleration.z;
            sample_count++;
        }

        // When time is up, run inference
        if (millis() - capture_start_time >= CAPTURE_DURATION_MS) {
            capturing = false;
            Serial.println("Capture complete");
            run_inference();
        }
    }
}

// Run gesture classification using Edge Impulse model
void run_inference() {
    if (sample_count * 3 < FEATURE_SIZE) {
        Serial.println("ERROR: Not enough data for inference");
        return;
    }

    ei_impulse_result_t result = { 0 };
    signal_t features_signal;
    features_signal.total_length = FEATURE_SIZE;
    features_signal.get_data = &raw_feature_get_data;

    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false);
    if (res != EI_IMPULSE_OK) {
        Serial.print("ERR: Failed to run classifier (");
        Serial.print(res);
        Serial.println(")");
        return;
    }

    print_inference_result(result);
}

// Print prediction result to Serial monitor
void print_inference_result(ei_impulse_result_t result) {
    float max_value = 0;
    int max_index = -1;

    // Find the label with the highest confidence score
    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        if (result.classification[i].value > max_value) {
            max_value = result.classification[i].value;
            max_index = i;
        }
    }

    // Output predicted label and confidence
    if (max_index != -1) {
        String label = ei_classifier_inferencing_categories[max_index];

        Serial.print("Prediction: ");
        Serial.print(label);
        Serial.print(" (");
        Serial.print(max_value * 100, 2);
        Serial.println("%)");
    }
}
