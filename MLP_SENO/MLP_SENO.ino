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
const int HiddenNodes = 10;  //neuronas en la capa oculta
const int InputNodes = 1;  //neuronas en la capa de entrada
const int OutputNodes = 1;  //neuronas en la capa de salida

// Estos valores son necesarios para la normalización de los datos cuando entran y salen de la red
// Rango de normalización
const float YMin = -1;
const float YMax = 1;

// Valores para normalizar la entrada de la red
const float XMin = 0;  //rango de la señal de entrada
const float XMax = 6.2832;  //2*pi
// Valores para normalizar la salida de la red
const float DMin = -5; //salida deseada desnormalizada
const float DMax = 5;

// Los pesos de la red fueron obtenidos en Matlab y se copiaron a este programa
// Pesos capa oculta
const float HiddenWeights[HiddenNodes][InputNodes+1]= {
{   12.1008 , -12.2479},
{   -2.0963 ,   1.9875},
{   -6.6196 ,   4.2421},
{   -6.2267 ,   2.1509},
{    2.1891 ,  -0.2349},
{    2.9252 ,   0.5600},
{    6.3450 ,   2.2821},
{   -2.9795 ,  -2.3524},
{    5.3267 ,   5.0348},
{   8.6169  ,  9.1310}  
    }; 

// Pesos capa de salida
const float OutputWeights[OutputNodes][HiddenNodes+1]  = {
  {0.0046,   -1.5960,   -0.0046,    0.0045,   -1.3084, -0.3446,   -0.0055,   -0.7463,    0.1888,    0.1186, 0.3803 }
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

}

void loop(){
  
  float Entrada;
  float Salida;
  float Tiempo;


Tiempo=millis();
Entrada=(fmod(Tiempo,6283))/1000; //un valor entre 0 y 2*pi


// Normalización de la entrada que se usará en la red neuronal
Input[0][0]=YMin+ ((Entrada-XMin)*((YMax-YMin)/(XMax-XMin)));

/******************************************************************
* Compute hidden layer activations
******************************************************************/


    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[i][InputNodes] ;//
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

Serial.println( Salida);       // print as an ASCII-encoded decimal - same as "DEC"
delay(50);  


}



