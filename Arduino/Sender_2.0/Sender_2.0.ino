
#include <IRremote.h>
#include <SoftwareSerial.h>

const int NEC_BIT = 32;
const int MAX_ARR_SIZE = 39;

uint32_t custom_list[MAX_ARR_SIZE];
IRsend sender;
SoftwareSerial bthSerial(7,8);
IRrecv irrecv(11);
String str = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bthSerial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (bthSerial.available()) {
    delay(10);
    while (bthSerial.available()) {
      str += (char)bthSerial.read();
    }

    Serial.println(str);

    if (str.indexOf("set,") > -1) {
      int i = str.indexOf("set");
      delay(6);
      uint32_t val = str.substring(i+4).toInt();
      str.remove(i);
      long key = str.toInt();
      setKey(key, val);
    } else if (str.indexOf("set") > -1) {
      int i = str.indexOf("set");
      delay(6);
      str.remove(i);
      long key = str.toInt();
      str = "";
      setKey(key);
    } else {
      if (str.charAt(1) > 47 && str.charAt(1) < 58) {
        str = str.substring(0, 2);
      } else {
        str = str.substring(0, 1);
      }
      Serial.println(str);
      long key = str.toInt();
      sendKey(key);
    }

    str = "";
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

void setKey (long key) {
//  bthSerial.write("setting");
  irrecv.resume();
  decode_results results;

  if (bthSerial.available()) {
    delay (10);
    while (bthSerial.available()) {
      str += (char)bthSerial.read();
    }
  }

  bool do_loop = true;
  while (do_loop) {
    if (irrecv.decode(&results)) {
      Serial.println(results.value);
      Serial.println(results.decode_type);
      if (results.decode_type != NEC || results.value == 0xFFFFFFFF ) { // Must be NEC! Accept no substitute!
        irrecv.resume();
      } else {
        Serial.println(results.value, HEX);
//        Serial.println(results.value);
        bthSerial.print(results.value);
        do_loop = false;
      }
    } else if (str.equalsIgnoreCase("cancel")) {
      do_loop = false;
    }
  }
}

void setKey (long key, long value) {
  custom_list[key] = value;
}

void sendKey (long key) {
  uint32_t code = custom_list[key];
  if (code != 0) {
    Serial.println(code);
    sender.sendNEC(code, NEC_BIT);
  }
}

