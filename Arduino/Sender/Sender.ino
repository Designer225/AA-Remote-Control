
#include <IRremote.h>
#include <SoftwareSerial.h>

#define NEC_BIT 32
#define MAX_ARR_SIZE 39

// Apparently Arduino still uses C++ 98, meaning int is -32768 to 32767
// and long is -2147483648 to 2147483647
uint32_t smart_list[MAX_ARR_SIZE];
uint32_t seiki_list[MAX_ARR_SIZE];
uint32_t custom_list[MAX_ARR_SIZE];

IRsend sender;
SoftwareSerial bthSerial(7, 8);
IRrecv irrecv(11);
String str = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bthSerial.begin(9600);
  initSeiki();
  initSmart();
  irrecv.enableIRIn();
//  Serial.println("Beginning debugging");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (bthSerial.available()) {
    delay(10);
    while (bthSerial.available()) {
      str += (char)bthSerial.read();
    }
//    str = Serial.readString();
    Serial.println(str);
//    delay(10);

    if (str.indexOf("seiki") > -1) {
      int i = str.indexOf("seiki");
      delay(6);
      str.remove(i);
//      Serial.println(str);
      long key = str.toInt();
      sendSeiki(key);
    }

    if (str.indexOf("smart") > -1) {
      int i = str.indexOf("smart");
      delay(6);
      str.remove(i);
//      Serial.println(str);
      long key = str.toInt();
      sendSmart(key);
    }

    if (str.indexOf("set,") > -1) {
      int i = str.indexOf("set");
      delay(6);
      uint32_t val = str.substring(i+4).toInt();
      str.remove(i);
      long key = str.toInt();
      setKey(key, val);
    }

    if (str.indexOf("set") > -1) {
      int i = str.indexOf("set");
      delay(6);
      str.remove(i);
      long key = str.toInt();
      setKey(key);
      upload(key);
    }

    if (str.indexOf("custom") > -1) {
      int i = str.indexOf("custom");
      delay(6);
      str.remove(i);
      long key = str.toInt();
      sendCustom(key);
    }
    
    str = "";
//    Serial.println("loop complete");
    delay(10);
  }

  if (Serial.available()) {
    delay(10);
    bthSerial.write(Serial.read());
  }
}

void upload (long key) {
  String str = String(custom_list[key]);
  delay(50);
  bthSerial.write(str.c_str());
}

// store commands?
void setKey (long key) {
//  bthSerial.write("setting");
  irrecv.resume();
  decode_results results;
  
  while (true) {
    if (irrecv.decode(&results)) {
      Serial.print(results.value);
      Serial.println(results.decode_type);
      if (results.decode_type != NEC && results.value != 0xFFFFFFFF ) { // Must be NEC! Accept no substitute!
        irrecv.resume();
      } else {
        Serial.println(results.value, HEX);
        Serial.println(results.value);
        custom_list[key] = results.value;
        break;
      }
    } else if (bthSerial.readString().equalsIgnoreCase("cancel")) {
      break;
    }
  }
}

void setKey (long key, long value) {
  custom_list[key] = value;
}

void sendCustom (long key) {
  
  long code = custom_list[key];
  if (code != 0) {
//    Serial.print("Check passed! ");
    Serial.println(code);
    sender.sendNEC(code, NEC_BIT);
  }
}

void initSeiki () {
  seiki_list[0] = 0x40be629d;
  seiki_list[1] = 0x40be32cd;
  seiki_list[2] = 0x40be807f;
  seiki_list[3] = 0x40be40bf;
  seiki_list[4] = 0x40bec03f;
  seiki_list[5] = 0x40be20df;
  seiki_list[6] = 0x40bea05f;
  seiki_list[7] = 0x40be690f;
  seiki_list[8] = 0x40bee01f;
  seiki_list[9] = 0x40be10ef;
  seiki_list[10] = 0x40be906f;
  seiki_list[11] = 0x40be42bd;
  seiki_list[12] = 0x40be00ff;
  seiki_list[13] = 0x40be827d;
  seiki_list[14] = 0x40bee21d;
  seiki_list[15] = 0x40bec23d;
  seiki_list[16] = 0x40be0af5;
  seiki_list[17] = 0x40be8877;
  seiki_list[18] = 0x40bed22d;
  seiki_list[19] = 0x40bef00f;
  seiki_list[20] = 0x40be5aa5;
  seiki_list[21] = 0x40be30cf;
  seiki_list[22] = 0x40be9867;
  seiki_list[23] = 0x40bef20d;
  seiki_list[24] = 0x40be02fd;
  seiki_list[25] = 0x40be22dd;
  seiki_list[26] = 0x40be1ae5;
  seiki_list[27] = 0x40be4ab5;
  seiki_list[28] = 0x40be728d;
  seiki_list[29] = 0x40be12ed;
  seiki_list[30] = 0x40be926d;
  seiki_list[31] = 0x40beb24d;
  seiki_list[32] = 0x40be52ad;
  seiki_list[33] = 0x40bea25d;
  seiki_list[34] = 0x40beb04f;
  seiki_list[35] = 0x40beda25;
  seiki_list[36] = 0x40be58a7;
  seiki_list[37] = 0x40be9a65;
  seiki_list[38] = 0x40be18e7;
}

void sendSeiki (long key) {
  
  long code = seiki_list[key];
  if (code != 0) {
//    Serial.print("Check passed! ");
    Serial.println(code);
    sender.sendNEC(code, NEC_BIT);
  }
  
}

void initSmart () {
  smart_list[0] = 0xd15348b7;
  smart_list[1] = 0xd1538877;
  smart_list[33] = 0xd153d827;
  smart_list[21] = 0xd15322dd;
  smart_list[22] = 0xd153629d;
  smart_list[28] = 0xd15342bd;
  smart_list[29] = 0xd15302fd;
  smart_list[30] = 0xd153c23d;
  smart_list[31] = 0xd153827d;
  smart_list[32] = 0xd153c837;
}

void sendSmart (long key) {
  long code = smart_list[key];
  if (code != 0) {
//    Serial.print("Check passed! ");
    Serial.println(code);
    sender.sendNEC(code, NEC_BIT);
  }
}

