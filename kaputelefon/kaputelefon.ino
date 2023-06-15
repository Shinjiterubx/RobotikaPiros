#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <IRremote.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9
#define SS_PIN          10

int RECV_PIN = 8;
Servo myservo;
IRrecv irrecv(RECV_PIN);
MFRC522 mfrc522(SS_PIN, RST_PIN);
int pos = 0;
LiquidCrystal_I2C lcd(0x27,16,2); 


decode_results results;

int pir = 4;
int value = 0;
int state;
String houseCode = "01";
String passCode = "2348";
String currentCode = "";
String uid = "74124154127";
unsigned long myTime;
boolean changeState = false;


void setup()
{
  Serial.begin(9600);
  
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
  pinMode(13,OUTPUT);

  pinMode(pir, INPUT);
  lcd.init();                      
  lcd.setCursor(0,0);
  lcd.print("PIN: ");
  SPI.begin();   
  mfrc522.PCD_Init();
  myTime = millis();
  
  //Állapot változó
  state = 0;
}


void loop() {
  
  checkState();
  
}



//Ellenőrző metódus, ellenőrzi, hogy a program melyik állapotban van jeleneleg.
//Az aktuális állapotot a state változóban tároljuk.
//Ha bővíteni akarjuk állapotokkal itt kell hozzáadni új case-ként.
//Az állapot dolgait egy metódusban kell kifejteni
//minek a neve stateq + állapot száma kell legyen pl.: 1-es állapot sateq1

void checkState(){
    Serial.println(state);
    switch(state){
        case 0:
          stateq0();
          break;
        case 1:
          stateq1();
          break;
        case 2:
          stateq2();
          break;
        case 3:
          stateq3();
          break;
        default:
          stateq0();
          break;
      }
  }


//Állapot metódusok

void stateq3(){
     currentCode = "";
     setLcdToDefault();

     for(int i = 0; i < 4; i++){
            currentCode = readCodeFromScreen(currentCode);
        }
        
      Serial.println(currentCode);
      if (currentCode == passCode){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("New PIN: ");
          currentCode = "";
          
          for(int i = 0; i < 4; i++){
              currentCode = readCodeFromScreen(currentCode);
          }


          if(changeState == true){
              passCode = currentCode;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("New PIN is: ");
              lcd.print(passCode);
              lcd.noBacklight();
              state = 0;
            }
        }else{
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Wrong PIN");
          delay(3000);
          lcd.noBacklight();
          state = 0;
        }

        
  }

void stateq2(){
     openDoor();
     currentCode = "";
     state = 0;
}


void stateq1(){
      currentCode = "";
      setLcdToDefault();
      
      for(int i = 0; i < 4; i++){
            currentCode = readCodeFromScreen(currentCode);
        }
        
  
      if (currentCode == passCode){
          setLcdToDefault();
          lcd.print("Correct");
          delay(2000);
          state = 2;
        }else{
          setLcdToDefault();
          lcd.print("Wrong");
          delay(2000);
          currentCode = "";
          state = 0;  
        }
  }

void stateq0(){
      backLight();
      boolean validNumber = false;
      String currentHouseNumber = "";
      state = 0;
      setLcdToDefault();
      currentCode = readCodeFromScreen(currentCode);
      Serial.println(currentCode);
      if (currentCode == "*"){
            currentHouseNumber = readCodeFromScreen(currentHouseNumber);
            currentHouseNumber = readCodeFromScreen(currentHouseNumber);
            if (currentHouseNumber == houseCode){
                lcd.print(" Correct");
                state = 1;
              }else{
                setLcdToDefault();
                lcd.print(" Wrong");
                delay(7000);
                }
            //Le kell vizsgálni azt is, hogya nem ír be kódot akkor kidobja.
            //mind a kettő ifben.
        }
        
      if(currentCode == "#"){
            currentHouseNumber = readCodeFromScreen(currentHouseNumber);
            currentHouseNumber = readCodeFromScreen(currentHouseNumber);
            if (currentHouseNumber == houseCode){
                state = 3;
                changeState = true;
              }else{
                delay(7000);
                }
      }
      currentCode = "";
}

//Állapot metódusok vége


String rfidRead(){
    MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  byte block;
  byte len;
  MFRC522::StatusCode status;

  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return "";
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return "";
  }

  Serial.println(F("**Card Detected:**"));

  //-------------------------------------------
  
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));
  String currentuid;
  
  for (int i = 0; i < 4; i++) {
      currentuid = currentuid + mfrc522.uid.uidByte[i];
      Serial.println(currentuid);
    }


  Serial.println(F("\n**End Reading**\n"));

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  return currentuid;

  delay(1000); //change value if you want to read cards faster
  }


//Kód beolvasása a billentyűzetről és hozzáírja az előző kódhoz;

String readCodeFromScreen(String code){
    char key;
    unsigned long currentTime = myTime;
    Serial.println(currentTime);
    while(irrecv.decode(&results) == 0){
      myTime = millis();
      Serial.println(myTime);
      if(myTime - currentTime >= 5000){
          state = 0;
          setLcdToDefault();
          changeState = false;
          currentCode="";
          break;
        }
      
      if (state == 0 && rfidRead() == uid){
          state = 2;
               setLcdToDefault();
               lcd.clear();
               lcd.print("RFID DETECTED");
               currentCode="";
          break;
        }
    }
    
    if (irrecv.decode(&results)) {
      key = readKeyFromIR();
      irrecv.resume(); // Receive thxt value

      switch(key){
        case '0': 
          code = code + 0;
          setLcdToDefault();
          lcd.print(code);
          return code;
        case '1': 
          code = code + 1;
          setLcdToDefault();
          lcd.print(code);
          return code;
        case '2': 
          code = code + 2;
          setLcdToDefault();
          lcd.print(code);
          return code;
        case '3': 
          code = code + 3;
          setLcdToDefault();
          lcd.print(code);
          return code;
        case '4': 
          code = code + 4;
          setLcdToDefault();
          lcd.print(code);
          return code;
        case '5': 
          code = code + 5;
          setLcdToDefault();
          lcd.print(code);
          return code;
        case '6': 
          code = code + 6;
          setLcdToDefault();
          lcd.print(code);
          return code;
        case '7': 
          code = code + 7;
          setLcdToDefault();
          lcd.print(code);
          return code;
        case '8': 
          code = code + 8;
          setLcdToDefault();
          lcd.print(code);
          return code;
        case '9': 
          code = code + 9;
          setLcdToDefault();
          lcd.print(code);
          return code;
        case '*': 
          code = code + "*";
          setLcdToDefault();
          lcd.print(code);
          return code;
        case '#': 
          code = code + "#";
          setLcdToDefault();
          lcd.print(code);
          return code;
      }
    }
    return code;
  }

void setLcdToDefault(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PIN: ");
  }

char readKeyFromIR(){
    char key = ' '; 
    
    
      switch(results.value)
        {
          case 16738455: key = '1';
             delay(500);
             irrecv.resume();
             return key;
          case 16750695: key = '2';
             delay(500);
             irrecv.resume();
             return key;
          case 16756815: key = '3';
             delay(500);
             irrecv.resume();
             return key;
          case 16724175: key = '4';
             delay(500);
             irrecv.resume();
             return key;
          case 16718055: key = '5';
             delay(500);
             irrecv.resume();
             return key;
          case 16743045: key = '6';
             delay(500);
             irrecv.resume();
             return key;
          case 16716015: key = '7';
             delay(500);
             irrecv.resume();
             return key;
          case 16726215: key = '8';
             delay(500);
             irrecv.resume();
             return key;
          case 16734885: key = '9';
             delay(500);
             irrecv.resume();
             return key;
          case 16730805: key = '0';
             delay(500);
             irrecv.resume();
             return key;
          case 16728765: key = '*';
             delay(500);
             irrecv.resume();
             return key;;
          case 16732845: key = '#';
             delay(500);
             irrecv.resume();
             return key;
          default:
            break;
        }
}

//Háttérvilágítás

void backLight(){
    value = digitalRead(4);
    if (value == 1){
      lcd.backlight();
    }else{
      lcd.noBacklight();
      }
    
  }

//Ajtónyitás

void openDoor(){
  myservo.attach(5);
  if(pos != 181){
    for (pos = 0; pos <= 180; pos += 1) { 
      myservo.write(pos);
      delay(70);
    }
    
    pos = 0;
    myservo.write(pos);
    delay(30);
    myservo.detach();
  }
}
