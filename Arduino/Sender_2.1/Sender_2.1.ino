
#include <IRremote.h>
#include <SoftwareSerial.h>

const int NEC_BIT = 32;

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

    if (str.indexOf("set") > -1) {
      int i = str.indexOf("set");
      delay(6);
      str.remove(i);
      long key = str.toInt();
      setKey(key);
    } else {
        uint32_t value = str.toInt();
        sender.sendNEC(value, NEC_BIT);
    }

    str = "";
  }

  if (Serial.available()) {
    delay(10);
    bthSerial.write(Serial.read());
  }
}

void setKey (long key) {
  Serial.println("Setting");
  str = "";
  irrecv.resume();
  decode_results results;

  bool do_loop = true;
  while (do_loop) {
    if (bthSerial.available()) {
      delay (10);
      while (bthSerial.available()) {
        str += (char)bthSerial.read();
      }
    }
    
    if (irrecv.decode(&results)) {
      Serial.println(results.value);
      Serial.println(results.decode_type);
      if (results.decode_type != NEC || results.value == 0xFFFFFFFF ) { // Must be NEC! Accept no substitute!
        irrecv.resume();
      } else {
        Serial.println(results.value, HEX);
        bthSerial.print(results.value);
        do_loop = false;
      }
    } else if (str.equalsIgnoreCase("cancel")) {
      do_loop = false;
    }
  }
}
