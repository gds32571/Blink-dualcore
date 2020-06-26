/*
  Blink-dualcore

  taken from the blinky example

   26 Jun 2020 - gswann
   experimenting with dual core programming on the ESP32

   This is a good web site for basics:
   https://www.savjee.be/2020/01/multitasking-esp32-arduino-freertos/

*/

// int LED_BUILTIN = 2;
const int led0 = 25; // Pin of the LED for task0
const int led1 = 26; // Pin of the LED for task1


/* define these for the various conditions.
   If you allow task0 to hang (by removing the two slashes, you will always see a WDT error and reboot.
   Allowing a hang in loop or task 1 will not cause this error, unless
   you allow the systems WDT to watch core 1
*/

//#define HANGTASK0
//#define HANGTASK1
//#define HANGLOOP
//#define WATCHCORE1


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  Serial.begin(115200);

  // this task defaults to core 0, at least on my system
  xTaskCreate(
    toggleLED0,    // Function that should be called
    "Toggle LED0",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );

  //  this causes the second blink task to run on core 1
  xTaskCreatePinnedToCore(
    toggleLED1,    // Function that should be called
    "Toggle LED1",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL,            // Task handle
    1                // which core, 0 or 1
  );

#ifdef WATCHCORE1
  enableCore1WDT();
#endif

  Serial.println("File Blink-dualcore.ino running an on ESP32");

}

// the loop function runs over and over again forever
void loop() {
  // the blue LED blinks fast at first then slower and slower
  for (int ii = 10 ; ii < 560 ; ii += 50) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(ii);                       // wait awhile
    digitalWrite(LED_BUILTIN, LOW);
    delay(ii);                       // wait awhile
  }

  Serial.print("Loop task is running on: ");
  Serial.println(xPortGetCoreID());
  //vTaskDelay(100 / portTICK_PERIOD_MS);

  // kill loop() after one pass if defined
#ifdef HANGLOOP
  while (true) {
    int a = 0;
  }
#endif

}


void toggleLED0(void * parameter) {
  for (;;) { // infinite loop

    // Turn the LED on
    digitalWrite(led0, HIGH);

    // Pause the task for 500ms
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Turn the LED off
    digitalWrite(led0, LOW);

    // Pause the task again for 500ms
    vTaskDelay(500 / portTICK_PERIOD_MS);

    Serial.print("  Blink 0 task is running on: ");
    Serial.println(xPortGetCoreID());
    vTaskDelay(100 / portTICK_PERIOD_MS);

#ifdef HANGTASK0
    while (true) {
      int a = 0;
    }
#endif

  }
}

void toggleLED1(void * parameter) {
  for (;;) { // infinite loop

    // Turn the LED on
    digitalWrite(led1, HIGH);

    // Pause the task for 500ms
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Turn the LED off
    digitalWrite(led1, LOW);

    // Pause the task again for 500ms
    vTaskDelay(500 / portTICK_PERIOD_MS);

    Serial.print("  Blink 1 task is running on: ");
    Serial.println(xPortGetCoreID());
    vTaskDelay(100 / portTICK_PERIOD_MS);

#ifdef HANGTASK1
    while (true) {
      int a = 0;
    }
#endif

  }
}
