//Canales a Leer
#define R  0
#define L  1
 
//PINS Izquierdos
#define L1 5
#define L2 4
#define L3 3
 
//PINs Derechos
#define R1 0
#define R2 1
#define R3 2


 
//Umbrales para cada PIN
#define UMBRAL1  60
#define UMBRAL2  80
#define UMBRAL3  90
 
//Variables donde guardamos
//El valor de R y L
int r;
int l;
 
void setup() {
 
  //Ponemos todos pins del 0 a 6
  //en modo OUTPUT
  for (int i = 0; i < 6; i++)
   pinMode(i, OUTPUT);
 
}
 
void loop() {
 
  //Leemos los canales
 r = analogRead(R);
 l = analogRead(L);
 
 //Encemos los leds que correspondan
 
 if (r > UMBRAL1)
  digitalWrite(R1, HIGH);
 else
  digitalWrite(R1, LOW);
 
 if (r > UMBRAL2)
  digitalWrite(R2, HIGH);
 else
  digitalWrite(R2, LOW);
 
 if (r > UMBRAL3)
  digitalWrite(R3, HIGH);
 else
  digitalWrite(R3, LOW);
 
 if (l > UMBRAL1)
   digitalWrite(L1, HIGH);
 else
   digitalWrite(L1, LOW);
 
 if (l > UMBRAL2)
   digitalWrite(L2, HIGH);
 else
    digitalWrite(L2, LOW);
 
 if (l > UMBRAL3)
   digitalWrite(L3, HIGH);
 else
    digitalWrite(L3, LOW);
 
}
