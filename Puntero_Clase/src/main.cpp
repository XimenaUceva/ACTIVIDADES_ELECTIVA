#include <Arduino.h>
int numero =10;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(112500);
}

void duplicar(int *PunteroNumero) {
  *PunteroNumero = *PunteroNumero *2;
  Serial.print("En duplicar (): punteroNumero =");
  Serial.println (*PunteroNumero);
  
}
void loop() {
  Serial.print("En loop(): numero = "); //En loop() numero = 10

  Serial.println(numero);
  duplicar(&numero);
}

