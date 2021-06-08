#include "uart.h"
#include "database.h"
#include "rtc.h"

#include <Arduino.h>
#include <ArduinoJson.h>

char hc06_rx_queue[512];
uint16_t p_hc06_rx_data = 0;

void handler_data(char* command);
void update_database(JsonDocument &doc);

void uart_init(void)
{
    Serial.begin(115200);
    while (!Serial);  // Wait for the serial connection to be establised.}
}

void uart_handler(void)
{
    char ch;

    p_hc06_rx_data = 0;
    memset(hc06_rx_queue, 0, sizeof(hc06_rx_queue));

    if (Serial.available() > 0) {
        while (Serial.available() > 0) {
            ch = Serial.read();
            if (p_hc06_rx_data < 512) {
                hc06_rx_queue[p_hc06_rx_data] = ch;
                p_hc06_rx_data++;
            }

            delay(1); /*1ms */
        }

        // Serial.print("recieved: ");
        // Serial.println(hc06_rx_queue);

        /* Handler data recieved */
        handler_data(hc06_rx_queue);

        p_hc06_rx_data = 0;
        memset(hc06_rx_queue, 0, sizeof(hc06_rx_queue));
    }
}

extern void update_database(JsonDocument &doc);

void handler_data(char* command)
{
    if (command == NULL) {
        return;
    }

    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, command);

    if (error) {
        // Serial.print(F("Decode fail\n"));
        // Serial.println(error.c_str());
        return;
    }

    if ((uint8_t)doc["cmd"] == CMD_POST_DATA) {
        update_database(doc);
    }
}
