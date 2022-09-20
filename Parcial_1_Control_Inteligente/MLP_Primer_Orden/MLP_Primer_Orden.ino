/******************************************************************
 * ArduinoANN - An artificial neural network for the Arduino
 * All basic settings can be controlled via the Network Configuration
 * section.
 * See robotics.hobbizine.com/arduinoann.html for details.
 ******************************************************************/

#include <math.h>

/******************************************************************
 * Network Configuration - customized per network 
 ******************************************************************/

const int PatternCount = 1;
const int HiddenNodes = 9;
const int InputNodes = 4;
const int OutputNodes = 1;

float Samples=0;
float randNumber;
float Y_K_1=0;
float U_K_1=0;
float Y_K_2=0;
float U_K_2=0;
// Estos valores son necesarios para la normalización de los datos cuando entran y salen de la red
// Rango de normalización
const float YMin = -1;
const float YMax = 1;


// Valores para normalizar la entrada de la red
const float X1Min = 0.00032304212091327549;
const float X1Max = 4.6187842811411501;
const float X2Min = 0;
const float X2Max = 4.6187842811411501;
const float X3Min = 0.076981862111474314;
const float X3Max = 9.9103738553404686;
const float X4Min = 0.076981862111474314;
const float X4Max = 9.9103738553404686;

// Valores para normalizar la salida de la red
     
const float DMin = 0.0012711867345181875 ;
const float DMax =4.6187842811411501;

// Los pesos de la red fueron obtenidos en Matlab y se copiaron a este programa
// Pesos capa oculta
const float HiddenWeights[HiddenNodes][InputNodes+1]= {

{-0.7305 ,  -0.8324,   -2.0720,   -2.1163,    2.1166},
 {   0.3818,   -1.8658,    1.4517,   -0.5655,   -2.2528},
 {   1.6968,    0.5995,    1.6341,    0.9519,   -0.8604},
  {  0.5688,    2.5680,   -1.5220,   -1.1625,   -0.9794},
   { 2.0823,    0.7667,   -0.8743,   -1.2462,    0.6382},
    {0.6890,  -1.8390,    0.4511,   0.9859,    0.0597},
   {-2.3795,   -0.8004,   -0.2394,   -0.6917,   -0.8131},
   {-1.4300,   -0.3087,   -1.0460,    1.9807,   -1.6577},
    {1.4262,    1.5786,   -0.1307,    0.5894,    2.4426},

};/*

  
{   -0.9528,   -1.1472,   -1.3797,   -0.9796,    2.5284},
{    0.6680,    0.5791,   -1.1542,    0.5212,   -1.2736},
{    0.2725,   -1.5563,    1.7304,   -0.7860,   -1.4456},
{   -1.1353,    0.8805,   -0.2168,   -0.1032,    0.2004},
{   -1.6944,    0.6565,    0.3874,    0.4097,   -0.0453},
{    0.0479,    2.2218,   -1.3246,    0.5376,   -1.8048},
{    0.6759,   -0.1038,    0.5975,   -0.3810,    0.9914},
{   -0.9312,    0.4354,   -0.7827,    2.2220,   -1.8086},
{    0.6127,   -1.3700,   -1.3288,   -1.1448,    2.0807},
 
};
  */
// Pesos capa de salida
const float OutputWeights[OutputNodes][HiddenNodes+1]  = {

 {-0.5053,    0.4755,   -0.2549,   -0.4273,   -0.6980,   -0.3616, -0.4094,   -1.3334,    0.6924,   -0.8730}

//{0.0221,    0.5639,   -0.2212,   -1.0816,   -0.3082,   -0.0821, 0.6670,   -0.2493,   -0.1076,   -0.2534}
};

/******************************************************************
 * End Network Configuration
 ******************************************************************/


int i, j, p, q, r;
float Accum;
float Hidden[HiddenNodes];
float Output[OutputNodes];
float Input[InputNodes][PatternCount];
 

void setup(){
  //start serial connection
  Serial.begin(9600);
  randomSeed(analogRead(0));

}

void loop(){
  
  float Entrada;
  float Salida;
  float Tiempo;

//Tiempo=millis();
//Entrada=(fmod(Tiempo,6283))/1000; 

Samples=Samples+1;

if (Samples==1)
{
  randNumber = random(100);
  Samples=Samples+1;
}

if (Samples>100)
{
  Samples=0;
}

Entrada=(randNumber/10);

// Normalización de la entrada que se usará en la red neuronal
Input[0][0]=YMin+ ((Y_K_1-X1Min)*((YMax-YMin)/(X1Max-X1Min)));
Input[1][0]=YMin+ ((Y_K_2-X2Min)*((YMax-YMin)/(X2Max-X2Min)));
Input[2][0]=YMin+ ((U_K_1-X3Min)*((YMax-YMin)/(X3Max-X3Min)));
Input[3][0]=YMin+ ((U_K_2-X4Min)*((YMax-YMin)/(X4Max-X4Min)));

/******************************************************************
* Compute hidden layer activations
******************************************************************/


    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[i][InputNodes] ;
      for( j = 0 ; j < InputNodes ; j++ ) {
        Accum += HiddenWeights[i][j]*Input[j][0];
      }
      Hidden[i] = (2.0/(1.0 + exp(-2*Accum)))-1.0; 
    }



/******************************************************************
* Compute output layer activations
******************************************************************/
 //   Serial.print("\n");    // prints a tab 
    for( i = 0 ; i < OutputNodes ; i++ ) {    
      Accum = OutputWeights[i][HiddenNodes] ;
      for( j = 0 ; j < HiddenNodes ; j++ ) {
        Accum +=  OutputWeights[i][j]*Hidden[j];
      }

   Output[i] = Accum; 

    }

// La salida da la red esta nromalizada, para que quede en el rango original hay que desnormalizar
// Desnormalización de la salida de la red neuronal
Salida=DMin+ ((Output[0]-YMin)*((DMax-DMin)/(YMax-YMin)));


Y_K_2=Y_K_1;
Y_K_1=Salida;
U_K_2=U_K_1;
U_K_1=Entrada;




Serial.print("Salida: "); 
Serial.print( Salida);       // print as an ASCII-encoded decimal - same as "DEC"
Serial.print("  Entrada: "); 
Serial.println( Entrada);

delay(50);  


}



