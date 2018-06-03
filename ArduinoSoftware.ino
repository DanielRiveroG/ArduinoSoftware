String splitCommand[10];
String tmp = "";
byte commandSize = 0;
byte commandRead = 0;
uint16_t digitalOutputPins[] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37};
uint16_t digitalInputPins[] = {42,43,44,45,46,47,48,49};

void outputBit(){
  digitalWrite(digitalOutputPins[splitCommand[1].toInt()], splitCommand[2].toInt());  
}

byte inputBit(){
  return digitalRead(digitalInputPins[splitCommand[1].toInt()]);
}

void outputToggle(){
  digitalWrite(digitalOutputPins[splitCommand[1].toInt()], !digitalRead(digitalOutputPins[splitCommand[1].toInt()]));  
}

void outputPulse(){
  digitalWrite(digitalOutputPins[splitCommand[1].toInt()], splitCommand[2].toInt());
  delay(splitCommand[3].toInt());  
  digitalWrite(digitalOutputPins[splitCommand[1].toInt()], !splitCommand[2].toInt());
}

void setup() {
  Serial.begin(9600);
  DDRA = 0b11111111;
  DDRC = 0b11111111;
  DDRL = 0b00000000;
  PORTA = 0b00000000;
  PORTC = 0b00000000;
}

void executeCommand(){
  if(splitCommand[0].equals("HL")){
    Serial.println("!AK");
  }
  if(splitCommand[0].equals("OB")){
    outputBit();
    Serial.println("!AK");
  }
  if(splitCommand[0].equals("OT")){
    outputToggle();
    Serial.println("!AK");
  }
  if(splitCommand[0].equals("IB")){
    byte tmp = inputBit();
    Serial.println("!AK");
    (tmp == 0)? Serial.println("!OI 0"):Serial.println("!OI 1");
  }
  if(splitCommand[0].equals("OP")){
    outputPulse();
    Serial.println("!AK");
  }
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
    }
    if(c == '$'){
      commandRead = 1;  
    }
  }
}
