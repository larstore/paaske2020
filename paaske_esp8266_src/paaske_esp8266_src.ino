/******************************************************************************
 * Copyright 2018 Google
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
#include <CloudIoTCore.h>

#include "esp8266_mqtt.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#ifndef BTN_BUILTIN_FLASH
#define BTN_BUILTIN_FLASH 0
#endif


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupCloudIoT(); // Creates globals for MQTT
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BTN_BUILTIN_FLASH, INPUT_PULLUP);
}

unsigned long lastMillis = 0;
void loop()
{
  mqtt->loop();
  delay(10); // <- fixes some issues with WiFi stability

  if (!mqttClient->connected())
  {
    ESP.wdtDisable();
    connect();
    ESP.wdtEnable(0);
  }

  int btnValue = digitalRead(BTN_BUILTIN_FLASH);
  if (btnValue == LOW) { 
    Serial.println("Sending passphrase to cloud");
    digitalWrite(LED_BUILTIN, HIGH); // Button is active low
    mqtt->publishTelemetry("Insert passphrase here");
    delay(100); // Classic debouncing routine
  }
  if (btnValue == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
