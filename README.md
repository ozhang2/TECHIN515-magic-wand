## Setup Instructions and How to Run

### 1. Install Dependencies

Make sure you have the following installed:

- **Arduino IDE** with **ESP32 board support**  
  → Install from Boards Manager: `esp32 by Espressif Systems`

- **Required Arduino libraries** (install via Library Manager):
  - `Adafruit_MPU6050`
  - `Adafruit_Sensor`
  - `Wire`

- **Python 3** (if collecting gesture data using `process_gesture_data.py`)

### 2. Upload Edge Impulse Model

- Train and export your gesture recognition model from Edge Impulse.
- Select **Arduino library export (Quantized int8)**.
- Place the exported `.h`, `.cpp`, and model folders in the same directory as `wand.ino`.

### 3. Flash Code to ESP32

- Open `wand.ino` in Arduino IDE.
- Select **Board**: `ESP32C3 Dev Module`
- Select correct **COM Port**
- Click **Upload**

### 4. Run the Magic Wand

- Power on the ESP32 (via battery or USB).
- Perform a gesture (O / V / Z).
- Press the button (connected to GPIO3) to trigger recognition.
- The onboard LED will flash according to the gesture:
  - O → 1 flash
  - V → 2 flashes
  - Z → 3 flashes
- Prediction and confidence score will also appear in the **Serial Monitor**.
