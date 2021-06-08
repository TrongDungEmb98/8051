#include <Arduino.h>

#include "database.h"
#include "handler_mqtt.h"
#include "rtc.h"
#include "uart.h"

// /* BLYNK */
// #include <BlynkSimpleEsp8266.h>
// #define water_sensor_blynk_pin            V5
// #define power_sensor_blynk_pin                 V4
// char auth[] = "AK_mtAJMVhL3ceBBnt63QWJZ1DY-7qBi";

const char* ssid = "Dungnt98";
const char* password = "Peppe123";

// const char* ssid = "Hunonic";
// const char* password = "66668888";

void setup()
{
    uart_init();
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }

    Serial.print("INFO: IP address: ");
    Serial.println(WiFi.localIP());

    rtc_init();

    database_init();
    database_load();

    mqtt_init();
    
    memset(m_device.data[0].energy, 0 ,sizeof(m_device.data[0].energy));
    memset(m_device.data[1].energy, 0 ,sizeof(m_device.data[1].energy));
    memset(m_device.data[2].energy, 0 ,sizeof(m_device.data[2].energy));
    memset(m_device.data[3].energy, 0 ,sizeof(m_device.data[3].energy));
    memset(m_device.data[4].energy, 0 ,sizeof(m_device.data[4].energy));

    sprintf(m_device.data[0].energy, "127");
    sprintf(m_device.data[1].energy, "145");
    sprintf(m_device.data[2].energy, "114");
    sprintf(m_device.data[3].energy, "153");
    sprintf(m_device.data[4].energy, "178");

    m_device.data[0].water = 1245;
    m_device.data[1].water = 1294;
    m_device.data[2].water = 1522;
    m_device.data[3].water = 1390;
    m_device.data[4].water = 1782;
}

void loop()
{
    rtc_loop();
    mqtt_handler();

    uart_handler();
    sync_database();
}

extern uint8_t current_month;

uint32_t current_water_used = 0;
String current_energy;
void update_database(JsonDocument &doc)
{
    current_energy = doc["energy"].as<String>();
    current_water_used = doc["water"].as<int16_t>();

    if (atof(current_energy.c_str()) != 0) {
        memset(m_device.data[5].energy, 0, 100);
        sprintf(m_device.data[5].energy, "%s", current_energy.c_str());
    }

    m_device.data[5].water = current_water_used;

    sync_database_request = 1;
}
