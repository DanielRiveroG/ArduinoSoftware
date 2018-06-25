String splitCommand[10];
String tmp = "";
byte commandSize = 0;
byte commandRead = 0;
uint16_t digitalOutputPins[] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37};
uint16_t digitalInputPins[] = {A8, A9, A10, A11, A12, A13, A14, A15};
uint16_t analogOutputPins[] ={11,12,13,44,45,46,5,8};
uint16_t analogInputPins[] = {A0, A1, A2, A3};

void outputBit(){
  digitalWrite(digitalOutputPins[splitCommand[1].toInt()], splitCommand[2].toInt());
  Serial.println("!AK");  
}

void inputBit(){
  byte tmp = digitalRead(digitalInputPins[splitCommand[1].toInt()]);
  Serial.println("!AK");
  (tmp == 0)? Serial.println("!OI 0"):Serial.println("!OI 1");
}

void outputToggle(){
  digitalWrite(digitalOutputPins[splitCommand[1].toInt()], !digitalRead(digitalOutputPins[splitCommand[1].toInt()]));
  Serial.println("!AK");  
}

void outputPulse(){
  digitalWrite(digitalOutputPins[splitCommand[1].toInt()], splitCommand[2].toInt());
  delay(splitCommand[3].toInt());  
  digitalWrite(digitalOutputPins[splitCommand[1].toInt()], !splitCommand[2].toInt());
  Serial.println("!AK");
}

void inputAnalog(){
  int result = analogRead(analogInputPins[splitCommand[1].toInt()]);
  Serial.println("!AK");
  Serial.println("!OI " + String(result));
}

void inputByte(){
  int result = PINK;
  Serial.println("!AK");
  Serial.println("!OI " + String(result));
}

void outputByte(int sel){
  uint16_t value = splitCommand[1].toInt();
  switch(sel){
    case 0:
      PORTA = value;
      break;
    case 1:
      PORTC = value;
      break;
    case 2:
      PORTA = value & 0xFF;
      PORTC = value >> 8;
  }
  Serial.println("!AK");
}

void outputAnalog(){
  analogWrite(analogOutputPins[splitCommand[1].toInt()], splitCommand[2].toInt()); 
  Serial.println("!AK"); 
}

void waitBit(){
  while(digitalRead(digitalInputPins[splitCommand[1].toInt()]) != splitCommand[2].toInt()){ 
  }
  Serial.println("!AK"); 
}

void waitAnalog(){
  if(splitCommand[3].toInt()){
    while(analogRead(analogInputPins[splitCommand[1].toInt()] > splitCommand[2].toInt())){
    }  
  }else{
    while(analogRead(analogInputPins[splitCommand[1].toInt()] < splitCommand[2].toInt())){
    }  
  }
  Serial.println("!AK");  
}

void setup() {
  Serial.begin(9600);
  DDRA = 0b11111111;
  DDRC = 0b11111111;
  DDRK = 0b00000000;
  DDRB = 0b11100000;
  DDRL = 0b00111000;
  pinMode(8, OUTPUT);
  pinMode(5, OUTPUT);
  PORTA = 0b00000000;
  PORTC = 0b00000000;
}

void executeCommand(){
  if(splitCommand[0].equals("HL")){
    Serial.println("!AK");
    return;
  }
  if(splitCommand[0].equals("OB")){
    outputBit();
    return;
  }
  if(splitCommand[0].equals("OT")){
    outputToggle();
    return;
  }
  if(splitCommand[0].equals("IB")){
    inputBit();
    return;
  }
  if(splitCommand[0].equals("AI")){
    inputAnalog();
    return;
  }
  if(splitCommand[0].equals("IL")){
    inputByte();
    return;
  }
  if(splitCommand[0].equals("OP")){
    outputPulse();
    return;
  }
  if(splitCommand[0].equals("OL")){
    outputByte(0);
    return;
  }
  if(splitCommand[0].equals("OH")){
    outputByte(1);
    return;
  }
  if(splitCommand[0].equals("OW")){
    outputByte(2);
    return;
  }
  if(splitCommand[0].equals("PW")){
    outputAnalog();
    return;
  }
  if(splitCommand[0].equals("WB")){
    waitBit();
    return;
  }
  if(splitCommand[0].equals("WA")){
    waitAnalog();
    return;
  }
  Serial.println("!NK");
}

void loop() {
  if(Serial.available()){
    char c = Serial.read();
    if(commandRead){
      if(c == '\n'){
        splitCommand[commandSize] = tmp;
        tmp = "";
        commandSize++;
        executeCommand();
        commandRead = 0;
        commandSize = 0;
      }else if(c == ' '){
        splitCommand[commandSize] = tmp;
        tmp = "";
        commandSize++;
      }else{
        tmp += c;  
      } 
    }else if(c == '$'){
      commandRead = 1;  
    }else{
      Serial.println("!NK");
    }
  }
}
