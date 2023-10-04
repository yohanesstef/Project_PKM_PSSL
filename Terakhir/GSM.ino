void GSM_routine(){
    if (Serial.available() > 0)
    switch (Serial.read())
    {
      case 's':
        SendMessage();
        break;
      case 'r':
        RecieveMessage();
        break;
      case 'c':
        callNumber();
        break;
    }
  if (SIM.available() > 0)
    Serial.write(SIM.read());
}

void GSM_init(){
  _buffer.reserve(50);
  Serial.println("Sistem Started...");
  SIM.begin(SIM_BAUDRATE, SERIAL_8N1, SIM_TX, SIM_RX);
  delay(1000);
  Serial.println("Type s to send an SMS, r to receive an SMS, and c to make a call");
}

void SendMessage()
{
  //Serial.println ("Sending Message");
  SIM.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  SIM.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "Hello, how are you? greetings from miliohm.com admin";
  SIM.println(SMS);
  delay(100);
  SIM.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
}

void RecieveMessage()
{
  Serial.println ("SIM800L Read an SMS");
  SIM.println("AT+CMGF=1");
  delay (200);
  SIM.println("AT+CNMI=1,2,0,0,0"); // AT Command to receive a live SMS
  delay(200);
  Serial.write ("Unread Message done");
}

String _readSerial() {
  _timeout = 0;
  while  (!SIM.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (SIM.available()) {
    return SIM.readString();
  }
}

void callNumber() {
  SIM.print (F("ATD"));
  SIM.print (number);
  SIM.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}