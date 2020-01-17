void setup()
{
   pinMode(LEDG, OUTPUT);
}

void loop()
{
   digitalWrite(LEDG, HIGH);
   delay(250);
   digitalWrite(LEDG, LOW);
   delay(250);
}
