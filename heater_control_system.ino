#define dht_pin 2
#define heater_led 4
#define overheat_led 6


 typedef enum{
  idle,
  heating,
  target_heat,
  overheated,
  stabilizing
}state;

float targeted_heat = 32.0;
float overheat = 35.0;
float stablize = 2.0;
float current_temp = 00.0;

 byte data[5];
 state current_state = idle;
void dht_start_condition(void){
  pinMode(dht_pin, OUTPUT);
  digitalWrite(dht_pin,LOW);    
  delay(18);
  digitalWrite(dht_pin, HIGH);
  delayMicroseconds(30);
  pinMode(dht_pin, INPUT);
}

bool dht_data_collection(void) {
  // Wait for DHT to pull line low (response signal)
  unsigned long timeout = millis();
  while(digitalRead(dht_pin) == HIGH) {
    if(millis() - timeout > 100) return false; // Timeout
  }
                                                                    //the first high are low are for preparing the sensor to send data to mcu 
  // Wait for DHT to pull line high
  timeout = millis();
  while(digitalRead(dht_pin) == LOW) {
    if(millis() - timeout > 100) return false; // Timeout
  }
  
  // Wait for DHT to pull line low again (start of data) 
  timeout = millis();
  while(digitalRead(dht_pin) == HIGH) {
    if(millis() - timeout > 100) return false; // Timeout
  }
  
  // Read 40 bits of data
  for (int i = 0; i < 5; i++) {
    byte bits = 0;
    for (int j = 0; j < 8; j++) {
      // Wait for low period to end
      timeout = millis();
      while(digitalRead(dht_pin) == LOW) {
        if(millis() - timeout > 100) return false;
      }
      
      // Measure high period
      unsigned long highStart = micros();
      timeout = millis();
      while(digitalRead(dht_pin) == HIGH) {
        if(millis() - timeout > 100) return false;
      }
      unsigned long highDuration = micros() - highStart;
      
      // If high period > 40us, it's a '1', otherwise it's a '0'
      if (highDuration > 40) {
        bits |= (1 << (7 - j));
      }
    }
    data[i] = bits;
  }
  return true;
}

void setup() {
  
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(heater_led, OUTPUT);
pinMode(overheat_led, OUTPUT);
Serial.println("heater control system");
}

void loop() {
  // put your main code here, to run repeatedly:
dht_start_condition();
//Serial.println("start generated");
                                                      //commented prinf was for my debugging purpose
dht_data_collection();
//Serial.println("data collected");


// Checksum verification
  if (((byte)(data[0] + data[1] + data[2] + data[3]) & 0xFF) == data[4]) {
   current_temp = data[2] + (data[3] / 10.0);

  // int16_t t = ((data[2] & 0x7F) << 8) | data[3];
  // current_temp = t / 10.0;      //if using dht22 sensor
  } else {
    Serial.println("Checksum error");
    delay(2000);
    return;
  }

//heater control
 if(current_temp >= overheat) {
    // HIGHEST PRIORITY: Overheat protection
    current_state = overheated;
    digitalWrite(heater_led, LOW);
    digitalWrite(overheat_led, HIGH);
    delay(500);
    digitalWrite(overheat_led, LOW);
    delay(500);
  }
  else if(current_temp < targeted_heat - stablize) {
    current_state = heating;
    digitalWrite(heater_led, HIGH);
    digitalWrite(overheat_led, LOW);
  }
  else if(current_temp >= (targeted_heat - stablize) && current_temp < targeted_heat) {
    current_state = stabilizing;
    digitalWrite(heater_led, HIGH);
    digitalWrite(overheat_led, LOW);
  }
  else if(current_temp >= targeted_heat && current_temp < overheat) {
    current_state = target_heat;  
    digitalWrite(heater_led, LOW);
    digitalWrite(overheat_led, LOW);
  }

Serial.print("temprature:");
Serial.println(current_temp);
Serial.print("state:");
switch (current_state){
  case idle: Serial.println("idle"); break;
  case stabilizing: Serial.println("stabilizing"); break;
  case heating: Serial.println("heating"); break;
  case target_heat: Serial.println("target heat reached"); break;
  case overheated: Serial.println("over heated "); break;
}

delay(1000);

}