#pragma once

#include <ArduinoJson.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <string>

bool displayOn = true;
bool debug = true;
uint16_t debugDelay = 500;

uint8_t initialFxIndex = 0;

uint8_t dummy = 1;

// UI Elements *************************************************************************************

   using namespace ArduinoJson;

   bool rotateAnimations = false;
   
   String cPresetName;

   uint8_t cBright = 75;
   uint8_t cColOrd = 0;                  
   uint8_t cFxIndex = initialFxIndex;

   float cSpeed = 1.f;

   float cRatBase = 0.0f; 
   float cRatDiff= 1.f; 

   float cOffBase = 1.f; 
   float cOffDiff = 1.f; 

   float cScale = 1.f; 
   float cTwist = 1.f;

   float cRadius = 1.0f; 
   float cEdge = 1.0f;

   float cZoom = 1.f;

   float cAngle = 1.f; 

   float cZ = 1.f; 

   float cRed = 1.f; 
   float cGreen = 1.f; 
   float cBlue = 1.f; 

   bool Layer1 = true;
   bool Layer2 = true;
   bool Layer3 = true;
   bool Layer4 = true;
   bool Layer5 = true;

   ArduinoJson::JsonDocument sendDoc;
   ArduinoJson::JsonDocument receivedJSON;


//*******************************************************************************
//BLE CONFIGURATION *************************************************************

BLEServer* pServer = NULL;
BLECharacteristic* pButtonCharacteristic = NULL;
BLECharacteristic* pCheckboxCharacteristic = NULL;
BLECharacteristic* pNumberCharacteristic = NULL;

bool deviceConnected = false;
bool wasConnected = false;

#define SERVICE_UUID                  	"19b10000-e8f2-537e-4f6c-d104768a1214"
#define BUTTON_CHARACTERISTIC_UUID     "19b10001-e8f2-537e-4f6c-d104768a1214"
#define CHECKBOX_CHARACTERISTIC_UUID   "19b10002-e8f2-537e-4f6c-d104768a1214"
#define NUMBER_CHARACTERISTIC_UUID     "19b10003-e8f2-537e-4f6c-d104768a1214"


BLEDescriptor pButtonDescriptor(BLEUUID((uint16_t)0x2902));
BLEDescriptor pCheckboxDescriptor(BLEUUID((uint16_t)0x2902));
BLEDescriptor pNumberDescriptor(BLEUUID((uint16_t)0x2902));


//*******************************************************************************
// CONTROL FUNCTIONS ************************************************************

void updateAnimation(uint8_t newAnimation) {
   pButtonCharacteristic->setValue(String(newAnimation).c_str());
   pButtonCharacteristic->notify();
   if (debug) {
      Serial.print("Animation: ");
      Serial.println(newAnimation);
   }
}

// UI update functions ***********************************************

void sendReceiptNumber(String receivedID, float receivedValue) {
   // Prepare the JSON document to send
   sendDoc.clear();
   sendDoc["id"] = receivedID;
   sendDoc["val"] = receivedValue;

   // Convert the JSON document to a string
   String jsonString;
   serializeJson(sendDoc, jsonString);

   // Set the value of the characteristic
   pNumberCharacteristic->setValue(jsonString);
   
   // Notify connected clients
   pNumberCharacteristic->notify();
   
   if (debug) {
      Serial.print("Sent receipt for ");
      Serial.print(receivedID);
      Serial.print(": ");
      Serial.println(receivedValue);
   }
}

void sendReceiptCheckbox(String receivedID, bool receivedValue) {
  
   // Prepare the JSON document to send
   sendDoc.clear();
   sendDoc["id"] = receivedID;
   sendDoc["val"] = receivedValue;

   // Convert the JSON document to a string
   String jsonString;
   serializeJson(sendDoc, jsonString);

   // Set the value of the characteristic
   pCheckboxCharacteristic->setValue(jsonString);
   
   // Notify connected clients
   pCheckboxCharacteristic->notify();
   
   if (debug) {
      Serial.print("Sent receipt for ");
      Serial.print(receivedID);
      Serial.print(": ");
      Serial.println(receivedValue);
   }
}

// Handle UI request functions ***********************************************

void processNumber(String receivedID, float receivedValue ) {

   if (receivedID == "inBright") {cBright = receivedValue;};
   if (receivedID == "inSpeed") {cSpeed = receivedValue;};
   if (receivedID == "inColOrd") {cColOrd = receivedValue;};
   if (receivedID == "inRatBase") {cRatBase = receivedValue;};
   if (receivedID == "inRatDiff") {cRatDiff = receivedValue;};
   if (receivedID == "inOffBase") {cOffBase = receivedValue;};
   if (receivedID == "inOffDiff") {cOffDiff = receivedValue;};
   if (receivedID == "inScale") {cScale = receivedValue;};	
   if (receivedID == "inAngle") {cAngle = receivedValue;};	
   if (receivedID == "inZoom") {cZoom = receivedValue;};
   if (receivedID == "inRadius") {cRadius = receivedValue;};
   if (receivedID == "inEdge") {cEdge = receivedValue;};	
   if (receivedID == "inZ") {cZ = receivedValue;};	
   if (receivedID == "inRed") {cRed = receivedValue;};	
   if (receivedID == "inGreen") {cGreen = receivedValue;};	
   if (receivedID == "inBlue") {cBlue = receivedValue;};
   if (receivedID == "inTwist") {cTwist = receivedValue;};
   
   sendReceiptNumber(receivedID, receivedValue);

}

void processCheckbox(String receivedID, bool receivedValue ) {

   if (receivedID == "cxRotateAnim") {rotateAnimations = receivedValue;};
   if (receivedID == "cxLayer1") {Layer1 = receivedValue;};
   if (receivedID == "cxLayer2") {Layer2 = receivedValue;};
   if (receivedID == "cxLayer3") {Layer3 = receivedValue;};
   if (receivedID == "cxLayer4") {Layer4 = receivedValue;};
   if (receivedID == "cxLayer5") {Layer5 = receivedValue;};
    
   sendReceiptCheckbox(receivedID, receivedValue);
}

//*******************************************************************************
// PRESETS **********************************************************************

struct Preset {
   String pPresetName;
   uint8_t pFxIndex;
   uint8_t pBright;
   uint8_t pColOrd;
   float pSpeed;
   float pRatBase;
   float pRatDiff;
   float pOffBase;
   float pOffDiff;
   float pScale;	
   float pAngle;
   float pZoom;
   float pTwist;
   float pRadius;
   float pEdge;	
   float pZ;	
   float pRed;
   float pGreen;	
   float pBlue;   
};

Preset preset1 = {.pPresetName ="preset1"};
Preset preset2 = {.pPresetName ="preset2"};


void capturePreset(Preset &preset) {
   
   delay(debugDelay);
   Serial.println(cFxIndex);
   delay(debugDelay);
   Serial.println(cFxIndex);
   delay(debugDelay);
   preset.pFxIndex = cFxIndex;
   delay(debugDelay);
   Serial.println(preset.pFxIndex);
   delay(debugDelay);
   Serial.println(preset.pFxIndex);
   
   delay(debugDelay);
   Serial.println(cBright);
   delay(debugDelay);
   Serial.println(cBright);
   delay(debugDelay);
   preset.pBright = cBright;
   delay(debugDelay);
   Serial.println(preset.pBright);
   delay(debugDelay);
   Serial.println(preset.pBright);
   
   delay(debugDelay);
   Serial.println(cColOrd);
   delay(debugDelay);
   Serial.println(cColOrd);
   delay(debugDelay);
   preset.pColOrd = cColOrd;
   delay(debugDelay);
   Serial.println(preset.pColOrd);
   delay(debugDelay);
   Serial.println(preset.pColOrd);
   
   delay(debugDelay);
   Serial.println(cSpeed);
      delay(debugDelay);
   Serial.println(cSpeed);
   delay(debugDelay);
   preset.pSpeed = cSpeed;
   delay(debugDelay);
   Serial.println(preset.pSpeed);
   delay(debugDelay);
   Serial.println(preset.pSpeed);
   
   
   delay(debugDelay);
   Serial.println(cZoom);
   delay(debugDelay);
   Serial.println(cZoom);
   delay(debugDelay);
   preset.pZoom = cZoom;
   delay(debugDelay);
   Serial.println(preset.pZoom);
      delay(debugDelay);
   Serial.println(preset.pZoom);

      delay(debugDelay);
   Serial.println(cScale);
   delay(debugDelay);
   Serial.println(cScale);
   delay(debugDelay);
   preset.pScale = cScale;
   delay(debugDelay);
   Serial.println(preset.pScale);
      preset.pScale = cScale;
   delay(debugDelay);
   Serial.println(preset.pScale);

      delay(debugDelay);
   Serial.println(cAngle);
   delay(debugDelay);
   Serial.println(cAngle);
   delay(debugDelay);
   preset.pAngle = cAngle;
   delay(debugDelay);
   Serial.println(preset.pAngle);
 delay(debugDelay);
   Serial.println(preset.pAngle);
   
   
   
   delay(debugDelay);
   Serial.println(cTwist);
   delay(debugDelay);
   Serial.println(cTwist);
   delay(debugDelay);
   preset.pTwist = cTwist;
   delay(debugDelay);
   Serial.println(preset.pTwist);
      delay(debugDelay);
   Serial.println(preset.pTwist);
   
   
   delay(debugDelay);
   Serial.println(cRadius);
      delay(debugDelay);
   Serial.println(cRadius);
   delay(debugDelay);
   preset.pRadius = cRadius;
   delay(debugDelay);
   Serial.println(preset.pRadius);
      delay(debugDelay);
   Serial.println(preset.pRadius);

   delay(debugDelay);
   Serial.println(cEdge);
   delay(debugDelay);
   Serial.println(cEdge);
   delay(debugDelay);
   preset.pEdge = cEdge;
   delay(debugDelay);
   Serial.println(preset.pEdge);
      delay(debugDelay);
   Serial.println(preset.pEdge);

      delay(debugDelay);
   Serial.println(cZ);
   delay(debugDelay);
   Serial.println(cZ);
   delay(debugDelay);
   preset.pZ = cZ;
   delay(debugDelay);
   Serial.println(preset.pZ);
      delay(debugDelay);
   Serial.println(preset.pZ);
   
   
   delay(debugDelay);
   Serial.println(cRatBase);
   delay(debugDelay);
   Serial.println(cRatBase);
   delay(debugDelay);
   preset.pRatBase = cRatBase;
   delay(debugDelay);
   Serial.println(preset.pRatBase);
    delay(debugDelay);
   Serial.println(preset.pRatBase);
   
   delay(debugDelay);
   Serial.println(cRatDiff);   
   delay(debugDelay);
   Serial.println(cRatDiff);
   delay(debugDelay);
   preset.pRatDiff = cRatDiff;
   delay(debugDelay);
   Serial.println(preset.pRatDiff);
      delay(debugDelay);
   Serial.println(preset.pRatDiff);
   
  
   delay(debugDelay);
   Serial.println(cOffBase);  
   delay(debugDelay);
   Serial.println(cOffBase);
   delay(debugDelay);
   preset.pOffBase = cOffBase;
   delay(debugDelay);
   Serial.println(preset.pOffBase);
      delay(debugDelay);
   Serial.println(preset.pOffBase);
   

   delay(debugDelay);
   Serial.println(cOffDiff);
    delay(debugDelay);
   Serial.println(cOffDiff);
   delay(debugDelay);
   preset.pOffDiff = cOffDiff;
   delay(debugDelay);
   Serial.println(preset.pOffDiff);
    delay(debugDelay);
   Serial.println(preset.pOffDiff);
   

   delay(debugDelay);
   Serial.println(cRed);
   delay(debugDelay);
   Serial.println(cRed);
   delay(debugDelay);
   preset.pRed = cRed;
   delay(debugDelay);
   Serial.println(preset.pRed);
    delay(debugDelay);
   Serial.println(preset.pRed);
   
   delay(debugDelay);
   Serial.println(cGreen);
   delay(debugDelay);
   Serial.println(cGreen);
      delay(debugDelay);
   preset.pGreen = cGreen;
   delay(debugDelay);
   Serial.println(preset.pGreen);
      delay(debugDelay);
   Serial.println(preset.pGreen);
   
   delay(debugDelay);
   Serial.println(cBlue);
    delay(debugDelay);
   Serial.println(cBlue);
   delay(debugDelay);
   preset.pBlue = cBlue;
   delay(debugDelay);
   Serial.println(preset.pBlue);
    delay(debugDelay);
   Serial.println(preset.pBlue);

}


//*******************************************************************************
// CALLBACKS ********************************************************************

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    wasConnected = true;
    if (debug) {Serial.println("Device Connected");}
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    wasConnected = true;
  }
};

class ButtonCharacteristicCallbacks : public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *characteristic) {

      String value = characteristic->getValue();
      if (value.length() > 0) {
         
         uint8_t receivedValue = value[0];
         
         if (debug) {
            Serial.print("Animation: ");
            Serial.println(receivedValue);
         }
       
         if (receivedValue != 99) {
         
            if (receivedValue < 20) {

               if (receivedValue == 1) { //polar waves
                  cFxIndex = 0;
               }
               if (receivedValue == 2) { // spiralus
                  cFxIndex = 1;
               }
               if (receivedValue == 3) { // caleido1
                  cFxIndex = 2;
               }
               if (receivedValue == 4) { // waves
                  cFxIndex = 3;
               }
               if (receivedValue == 5) { // chasing spirals
                  cFxIndex = 4;
               }
               if (receivedValue == 6) { // complex kaleido 6 
                  cFxIndex = 5;
               }
               if (receivedValue == 7) { // water
                  cFxIndex = 6;
               }
               if (receivedValue == 8) { // experiment 10
                  cFxIndex = 7;
               }
               if (receivedValue == 9) { // experiment sm1
                  cFxIndex = 8;
               }
               if (receivedValue == 10) { // test
                  cFxIndex = 9;
               }

               displayOn = true;
               updateAnimation(cFxIndex);
            }
         
         if (receivedValue >= 20 && receivedValue < 30) {

            if (receivedValue == 20) { capturePreset(preset1); }

             if (receivedValue == 22) { capturePreset(preset2); }

         }

       }

       if (receivedValue == 99) { //off
          displayOn = false;
       }
	
      }
   }
};

class CheckboxCharacteristicCallbacks : public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *characteristic) {
  
      String receivedBuffer = characteristic->getValue();
  
      if (receivedBuffer.length() > 0) {
                  
         if (debug) {
            Serial.print("Received buffer: ");
            Serial.println(receivedBuffer);
         }
      
         ArduinoJson::deserializeJson(receivedJSON, receivedBuffer);
         String receivedID = receivedJSON["id"] ;
         bool receivedValue = receivedJSON["val"];
      
         if (debug) {
            Serial.print(receivedID);
            Serial.print(": ");
            Serial.println(receivedValue);
         }
      
         processCheckbox(receivedID, receivedValue);
      
      }
   }
};

class NumberCharacteristicCallbacks : public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *characteristic) {
      
      String receivedBuffer = characteristic->getValue();
      
      if (receivedBuffer.length() > 0) {
      
         if (debug) {
            Serial.print("Received buffer: ");
            Serial.println(receivedBuffer);
         }
      
         ArduinoJson::deserializeJson(receivedJSON, receivedBuffer);
         String receivedID = receivedJSON["id"] ;
         float receivedValue = receivedJSON["val"];
      
         if (debug) {
            Serial.print(receivedID);
            Serial.print(": ");
            Serial.println(receivedValue);
         }
      
         processNumber(receivedID, receivedValue);
      }
   }
};



//*******************************************************************************
// BLE SETUP FUNCTION ***********************************************************

void bleSetup() {

   BLEDevice::init("AnimARTrix Playground");

   pServer = BLEDevice::createServer();
   pServer->setCallbacks(new MyServerCallbacks());

   BLEService *pService = pServer->createService(SERVICE_UUID);

   pButtonCharacteristic = pService->createCharacteristic(
                     BUTTON_CHARACTERISTIC_UUID,
                     BLECharacteristic::PROPERTY_WRITE |
                     BLECharacteristic::PROPERTY_READ |
                     BLECharacteristic::PROPERTY_NOTIFY
                  );
   pButtonCharacteristic->setCallbacks(new ButtonCharacteristicCallbacks());
   pButtonCharacteristic->setValue(String(cFxIndex).c_str());
   pButtonCharacteristic->addDescriptor(new BLE2902());

   pCheckboxCharacteristic = pService->createCharacteristic(
                     CHECKBOX_CHARACTERISTIC_UUID,
                     BLECharacteristic::PROPERTY_WRITE |
                     BLECharacteristic::PROPERTY_READ |
                     BLECharacteristic::PROPERTY_NOTIFY
                  );
   pCheckboxCharacteristic->setCallbacks(new CheckboxCharacteristicCallbacks());
   pCheckboxCharacteristic->setValue(String(dummy).c_str());
   pCheckboxCharacteristic->addDescriptor(new BLE2902());
   
   pNumberCharacteristic = pService->createCharacteristic(
                     NUMBER_CHARACTERISTIC_UUID,
                     BLECharacteristic::PROPERTY_WRITE |
                     BLECharacteristic::PROPERTY_READ |
                     BLECharacteristic::PROPERTY_NOTIFY
                  );
   pNumberCharacteristic->setCallbacks(new NumberCharacteristicCallbacks());
   pNumberCharacteristic->setValue(String(dummy).c_str());
   pNumberCharacteristic->addDescriptor(new BLE2902());

      
   //**********************************************************

   pService->start();

   BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
   pAdvertising->addServiceUUID(SERVICE_UUID);
   pAdvertising->setScanResponse(false);
   pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
   BLEDevice::startAdvertising();
   if (debug) {Serial.println("Waiting a client connection to notify...");}

}
