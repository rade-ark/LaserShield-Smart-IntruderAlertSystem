# Laser Security & GSM Alert System

An Arduino-based intrusion detection system that uses a laser-LDR pair to monitor entry points. When the beam is interrupted, the system triggers a localized audible alarm and sends a real-time SMS alert to a pre-configured mobile number.


## 📋 Features
* **Precision Detection:** Uses a 650nm laser and LDR comparator module for reliable tripwire sensing.
* **False Positive Filtering:** Firmware-level debounce logic requires 300ms of continuous blockage to trigger an alarm.
* **Dual Alert System:** Simultaneous active buzzer alarm and GSM SMS notification.
* **Robust Power Management:** Integrated buck converter to handle high-current GSM transmission bursts.
* **Safe Logic Leveling:** Voltage divider protection for SIM800L serial communication.

---

## 🛠 Hardware Components

| Component | Purpose | Key Specification |
| :--- | :--- | :--- |
| **Arduino UNO** | System Controller | ATmega328P |
| **HW-493 Laser** | Emitter | 650nm Red Diode |
| **HW-072 LDR Module** | Receiver | LM393 Comparator / Digital Out |
| **SIM800L** | GSM Module | 2G Quad-band / 3.7-4.2V |
| **LM2596S** | Buck Converter | DC-DC Step Down (tuned to 4V) |
| **BC547** | NPN Transistor | Buzzer Driver / Switch |
| **Active Buzzer** | Local Alarm | 5V DC |
| **Resistors** | Protection/Logic | 1kΩ (Base & Divider), 2kΩ (Divider) |

---

## ⚙️ Technical Logic

### 1. Detection Mechanism
The system monitors a **Digital Input** from the LDR module.
* **State:** The module is active-high for intrusion (Output is `HIGH` when the beam is broken).
* **Verification:** The Arduino polls the sensor every 100ms. A counter (`lowCount`) must reach **3 consecutive hits** (300ms) to validate an intruder, preventing triggers from insects or optical noise.

### 2. Power Regulation
The SIM800L requires up to **2A bursts** during network transmission. 
* The **LM2596S Buck Converter** steps down the main power supply (9V-12V) to **4.0V**.
* **Note:** Always ensure a common ground (GND) between the Buck Converter and the Arduino.

### 3. Logic Level Shifting
Since the Arduino operates at 5V and the SIM800L at 3.3V, a voltage divider is used on the Arduino TX (Pin 3) to prevent overvoltage:
$$V_{out} = V_{in} \cdot \frac{R_2}{R_1 + R_2} = 5V \cdot \frac{2k\Omega}{1k\Omega + 2k\Omega} \approx 3.33V$$

---

## 🔌 Pin Mapping

| Component | Arduino Pin | Description |
| :--- | :--- | :--- |
| **LDR DO** | Pin 11 | Digital Input (Sensor) |
| **Buzzer** | Pin 12 | Digital Output (via BC547 Base) |
| **SIM800L TX** | Pin 2 | SoftwareSerial RX |
| **SIM800L RX** | Pin 3 | SoftwareSerial TX (via Voltage Divider) |

---

## 🚀 Installation & Setup

1.  **Adjust the Buck Converter:** Before connecting the SIM800L, power the LM2596S and turn the potentiometer until your multimeter reads exactly **4.0V**.
2.  **Calibrate the Sensor:** Power the Laser and LDR. Adjust the LDR module's onboard potentiometer until the status LED toggles cleanly when the beam is blocked.
3.  **Configure Phone Number:** Open the provided Arduino sketch and update the recipient number:
    ```cpp
    mySerial.println("AT+CMGS=\"+91123456789\""); // Replace with your number
    ```
4.  **Upload:** Upload the code to your Arduino UNO.

---

## 🛡 System Sequence
1.  **Idle:** Laser hits LDR $\rightarrow$ Pin 11 is `LOW`.
2.  **Intrusion:** Beam broken $\rightarrow$ Pin 11 goes `HIGH`.
3.  **Confirmation:** `lowCount` reaches 3 $\rightarrow$ `alarmActive` set to `true`.
4.  **Alert:** Pin 12 goes `HIGH` (Buzzer sounds) and `sendSMS()` executes AT command sequence.
5.  **Reset:** Beam restored $\rightarrow$ Pin 11 goes `LOW` $\rightarrow$ Alarm stops and system re-arms.

---

> **Warning:** Ensure the SIM card is 2G compatible and has the PIN lock disabled before inserting it into the SIM800L module.
