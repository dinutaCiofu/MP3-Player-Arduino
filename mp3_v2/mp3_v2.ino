#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

bool isPlaying = true;

void setup() {
  // verifica daca programul ruleaza pe o placa esp32
#if (defined ESP32)
  FPSerial.begin(9600, SERIAL_8N1, /*rx =*/D3, /*tx =*/D2);
#else
  FPSerial.begin(9600);
  Serial.begin(115200);
#endif

  Serial.println();
  Serial.println(F("Initializing MP3 Player ... (May take 3~5 seconds)"));

  // incepe initializarea obiectului myDFPlayer care gestioneaza comunicarea cu DFPlayer mini
  // se utilizeaza FPSerial pentru a comunica cu modulul DFPlayer
  if (!myDFPlayer.begin(FPSerial, /*isACK = */ true, /*doReset = */ true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0);  // daca initializarea esueaza intra intr-o bucla infinita
    }
  }
  Serial.println(F("MP3 Player"));

  myDFPlayer.volume(10);  //seteaza volumul
  myDFPlayer.play(1);     //Play the first mp3
}

void loop() {
#if !(defined ESP32)
  if (Serial.available() > 0) {
    char command = Serial.read();
    executeCommand(command);
  }
#endif
/*
  if (FPSerial.available() > 0) {
    char command = FPSerial.read();
    executeCommand(command);
  }
*/
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
  }
}

void executeCommand(char command) {
  switch (command) {
    case 'p':
      // Serial.println(F("Play/Pause command received"));
      if (isPlaying == true) {
        isPlaying = false;
        myDFPlayer.pause();
      } else {
        isPlaying = true;
        myDFPlayer.start();
      }
      break;
    case 'n':
      //  Serial.println(F("Next command received"));
      myDFPlayer.next();
      break;
    case 'i':
      //  Serial.println(F("Previous command received"));
      myDFPlayer.previous();
      break;
    default:
      Serial.println(F("Unknown command received"));
  }
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}