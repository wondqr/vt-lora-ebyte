#include <Arduino.h>
#include "vt_lora"

using namespace vt;
using lora_t = lora_e22;

// HardwareSerial Serial1(PA10, PA9);
Uart SerialLora(PA3, PA2);  // RX, TX
lora_t lora_tx(SerialLora, PA5, PA6);
// lora_t lora_rx(Serial2, PA5, PA6);

void setup() {
    delay(5000);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin();

    // Begin configuration (sleep) mode.
    lora_t *loras[] = {&lora_tx};
    for (auto &lora: loras) {
        Serial.println("Beginning configuration mode...");
        lora->config();
        Serial.println("Entered configuration mode!");
        lora->set_param(0xffff,
                        0,
                        115200,
                        LoRaParity::PARITY_8N1,
                        2400,
                        63,
                        240,
                        true);

        lora->query_param();

        Serial.println("Done config!");
    }

    lora_tx.begin(115200);
}

void loop() {
    // Serial.println("Sent!");

    // Test tx
    // lora_tx.hw_serial().println("Message");
    static uint32_t last = 0;
    if (millis() - last >= 1000) {
        last = millis();
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        lora_tx.hw_serial().println("Message");
        Serial.println("Sent!");
    }

    // Test rx
    // while (lora_rx.hw_serial().available()) Serial.write(lora_rx.hw_serial().read());
    while (lora_tx.hw_serial().available()) Serial.write(lora_tx.hw_serial().read());

}
