// finger + IR
#include <Adafruit_Fingerprint.h>   // thu vien Van Tay
#include <SoftwareSerial.h>         // thu vien Serial 
#include <EEPROM.h>                 // thu vien Save
#include <IRremote.h>

SoftwareSerial mySerial(2, 3);       // giao tiep Pin 2&3
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int bnn = 0;
int addnew=0;
int address = 0;
int irnew=0; 
byte value;
String s1="";
//long day = 86400000; // 86400000 milliseconds in a day
//long hour = 3600000; // 3600000 milliseconds in an hour
// long minute = 60000; // 60000 milliseconds in a minute

unsigned long timek;
uint8_t id;
const int receiverPin = 8; 
IRrecv irrecv(receiverPin); 
decode_results results;

void setup()  
{
  pinMode(4, OUTPUT); // mo cong, van tay
  pinMode(5, OUTPUT);  // dong cong
  pinMode(6, INPUT);  // nut nhan dong
  pinMode(7, OUTPUT);  // Loa
  timek = millis();
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(7,LOW);
 
  Serial.begin(9600);
  while (!Serial);  
  delay(100);

  finger.begin(57600);
  irrecv.enableIRIn();
  
  if (finger.verifyPassword()) {
    Serial.println("Tim thay van tay");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print(finger.templateCount); Serial.println(" van tay");
  Serial.println("Cho doi...");
}


uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}


void loop()                  
{
    int buttonStatus = digitalRead(6);
    if ((buttonStatus == HIGH)&&(bnn==0))
    {
      delay(500);
      digitalWrite(5,LOW);  // dong cua
      digitalWrite(7,LOW);  // tat loa
      delay(500);
      digitalWrite(5,HIGH);
      delay(500);
      Serial.println("N");
      delay(1000);  
      bnn=1; addnew=0;
    }
    if (buttonStatus == LOW)
    {
        if ((unsigned long)(millis() - timek) > 60000)
        {   digitalWrite(7,HIGH);
            delay(500);
            Serial.println("cua mo 60s"); 
        }
         if (bnn==1)
         { 
            digitalWrite(7,HIGH);
            delay(500);
            Serial.println("cua mo lech");   // dong lech
          }
        bnn=0; 
    }
  if (Serial.available())
  { char ch=Serial.read();
       
    if (ch=='4')  // doc eeprom
    { address=0;
      while (address<1024)
       { value = EEPROM.read(address);
          Serial.println(value, DEC);
          address = address + 1;
       }
          address=0;
    }
  
   if (ch=='5')  
    {   
        Serial.println("Delete ID 1..127: ");
        id = readnumber();
        if (id == 0) {
          return;
        }
        Serial.print("Delete...");
        Serial.println(id);
        deleteFingerprint(id);
    }


    if (ch=='9') 
    {   
        Serial.println("Nhap ID van tay 1..127: ");
        id = readnumber();
        addnew =0;
        if (id == 0) {
          return;
        }
        while (addnew < 10)
        { Serial.print("Kiem tra ID #");
          Serial.println(id);
          getFingerprintEnroll();
          addnew = addnew +1;
          digitalWrite(7,HIGH);
          delay(500);
          digitalWrite(7,LOW);
        }
    }
  }

if (irrecv.decode(&results)) 
  { 
  switch(results.value)
  {
    case 0x10: s1=s1+"1"; loa2();
                break;
    case 0x810: s1=s1+"2"; loa2();
                break;
    case 0x410: s1=s1+"3"; loa2();
                break;
    case 0xC10: s1=s1+"4"; loa2();
                break;      
    case 0x210: s1=s1+"5"; loa2();
                break;
    case 0xA10: s1=s1+"6"; loa2();
                break;
    case 0x610: s1=s1+"7"; loa2();
                break;
    case 0xE10: s1=s1+"8"; loa2();
                break;
    case 0x110: s1=s1+"9"; loa2();
                break;
    case 0x910: s1=s1+"0"; loa2();
                break;

    case 0x807F728D: s1=s1+"1"; loa2();
                break;
    case 0x807FB04F: s1=s1+"2"; loa2();
                break;
    case 0x807F30CF: s1=s1+"3"; loa2();
                break;
    case 0x807F52AD: s1=s1+"4"; loa2();
                break;      
    case 0x807F906F: s1=s1+"5"; loa2();
                break;
    case 0x807F10EF: s1=s1+"6"; loa2();
                break;
    case 0x807F629D: s1=s1+"7"; loa2();
                break;
    case 0x807FA05F: s1=s1+"8"; loa2();
                break;
    case 0x807F20DF: s1=s1+"9"; loa2();
                break;
    case 0x807F807F: s1=s1+"0"; loa2();
                break;
                
    
  }         
  
    Serial.println(s1); 
    //Serial.println(results.value, HEX);
    //if (irnew==1)
  //{   digitalWrite(7,HIGH);  // ID NEW
    //  delay(500);
     // digitalWrite(7,LOW); 
  //}
  
  if (s1.length()==3)
  {   s1="";
      // digitalWrite(7,HIGH);  
      // delay(500);
      // digitalWrite(7,LOW); 
  }

 if ((s1.length()==2)&&(irnew==1))
  {   id = s1.substring(0,2).toInt(); 
      s1=""; irnew =0; addnew =0;
     while (addnew < 10)
        {
        Serial.print("Kiem tra id "); Serial.println(id);
          getFingerprintEnroll() ;
          addnew = addnew +1;
          digitalWrite(7,HIGH);
          delay(500);
          digitalWrite(7,LOW);
        }
    }  
  
    if (s1.substring(0,2).toInt()==96)
    { 
        digitalWrite(7,HIGH);  // LOA bao ID New 3 giay
        delay(3000);
       digitalWrite(7,LOW);
        irnew=1; s1="";
   }
  if (s1.substring(0,2).toInt()==58)
    { 
    digitalWrite(7,LOW);  // tat loa
    digitalWrite(4, LOW);    // mo cong
    delay(500);
    digitalWrite(4, HIGH);
    Serial.println("mo cong");
    delay(2500);
    s1=""; irnew =0; addnew = 0;
  }
 
    delay(200);
    irrecv.resume(); 
  }


  
  getFingerprintIDez(); 
  delay(50);            
  
}


uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }   
}


uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    digitalWrite(7,HIGH);  // mo loa
	EEPROM.write(random(990,993), id);  // Save random eeprom
    delay(10000);    // Loa 10s 
    addnew = 10;   // Thoat vong lap
    digitalWrite(7,LOW); 
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}



uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  digitalWrite(7,HIGH);  
  digitalWrite(4, LOW);    // mo cong
  delay(500);

  digitalWrite(7,LOW);  // tat loa
  digitalWrite(4, HIGH);  // tat cong
  delay(500);
  
  Serial.println(finger.fingerID);
  delay(3000);
  
  bnn=0; 
  timek=millis();
 
       EEPROM.write(address, finger.fingerID);  // Save ID
       delay(10); address = address + 1;
       if (address == 1023) {    address = 0; }  // bo 2 end record 
  return finger.fingerID; 
}

int loa2() {
  digitalWrite(7,HIGH);    
  delay(500);
  digitalWrite(7,LOW);  
}
