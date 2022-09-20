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
const int HiddenNodes = 15;
const int InputNodes = 6;
const int OutputNodes = 1;

float Samples=0;
float randNumber;
float Y_K=0;
float Y_K_1=0;
float Y_K_2=0;
float Y_K_mas_1=0;
float U_K_1=0;
float U_K_2=0;
float U_K_3=0;

// Estos valores son necesarios para la normalización de los datos cuando entran y salen de la red
// Rango de normalización
const float YMin = -1;
const float YMax = 1;


// Valores para normalizar la entrada de la red
const float X1Min = 0.28431962530652399;
const float X1Max = 3.0562973703704386;
const float X2Min = 0.076981862111474314;
const float X2Max = 9.3469289594082756;
const float X3Min = 0.076981862111474314;
const float X3Max = 9.3469289594082756;
const float X4Min = 0.28431962530652399;
const float X4Max = 3.0562973703704386;
const float X5Min = 0.24640785447005287;
const float X5Max = 3.0562973703704386;
const float X6Min = 0;
const float X6Max = 3.0562973703704386;

// Valores para normalizar la salida de la red
     
const float DMin = 0.076981862111474314;
const float DMax =9.3469289594082756;

// Los pesos de la red fueron obtenidos en Matlab y se copiaron a este programa
// Pesos capa oculta
const float HiddenWeights[HiddenNodes][InputNodes+1]= {

  { 0.6080,    0.3238,   0.2420,   -1.7198,    0.0152,    2.3470,  1.3168},
 {   2.0720,    0.3110,   -0.3909,   -9.8956,    3.8725,    1.7116,  2.1916},
 {  -1.5358,    0.8378,   -0.1711,   -0.7617,   -1.1035,   -1.1439,  1.8818},
 {   0.3696,    0.6012,   -0.2011,    1.9842,    1.4653,   -1.5175,  0.8772},
 {  -1.9964,   -0.3181,   -1.9242,   -8.8893,  -1.7106 ,   2.9641,   9.7509},
 {  -1.1743,   -1.3841,    1.7292,   -0.9309,   -0.6111,   -0.3387,  -0.3904},
 {   3.5706,   -0.0818,    0.0971,   -1.1861,   -3.5109,   -2.3622,  1.7618},
 {   1.7755,    0.5263,   -1.4113,   -0.1263,   -1.3076,   -0.9497,  -0.6528},
 {   4.6486,   -0.8115,   -0.7837,   -2.9225,   -2.4606,   -0.6499,  -2.3094},
 {   9.1120,    0.4666,   -0.8404,   -7.3314,   -1.1732,    0.3685,  0.1327},
 {  -6.3590,   -0.8811,    1.3307,   -0.7898,    5.6928,   -0.1019,  -0.2039},
 {  -2.5779,    0.0903,   -0.1794,    4.4970,   -4.8707,    0.4054,  2.4801},
 {  -0.8089,   -1.7017,    1.5841,    0.5324,   -1.2813,   -0.0087,  -0.2767},
 {  -6.9006,   -0.4559,   -1.9522,    2.9065,   -3.2447,   -2.1354,   9.6631},
 {  -1.4970,   -0.1422,   -0.7371,   -0.7362,    0.0156,    1.2757,   -1.5816},



};

    

// Pesos capa de salida
const float OutputWeights[OutputNodes][HiddenNodes+1]  = {
{-4.6841,   -8.0553,   -0.3848,    1.3066,    7.5971,    0.9126,  0.8724,   -1.6443,   -2.9394,    6.9524,    5.4180,    7.4389, -2.1569,   -7.6296,    1.4870,    1.0104}
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
Input[0][0]=YMin+ ((Y_K_mas_1-X1Min)*((YMax-YMin)/(X1Max-X1Min)));
Input[1][0]=YMin+ ((U_K_2-X2Min)*((YMax-YMin)/(X2Max-X2Min)));
Input[2][0]=YMin+ ((U_K_3-X3Min)*((YMax-YMin)/(X3Max-X3Min)));
Input[3][0]=YMin+ ((Y_K-X4Min)*((YMax-YMin)/(X4Max-X4Min)));
Input[4][0]=YMin+ ((Y_K_1-X5Min)*((YMax-YMin)/(X5Max-X5Min)));
Input[5][0]=YMin+ ((Y_K_2-X6Min)*((YMax-YMin)/(X6Max-X6Min)));


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
Y_K_1=Y_K;
Y_K=Salida;
U_K_3=U_K_2;
U_K_2=U_K_1;
U_K_1=Y_K_mas_1;
Y_K_mas_1=Entrada;

Serial.print("Salida: "); 
Serial.print( Salida);       // print as an ASCII-encoded decimal - same as "DEC"
Serial.print("  Entrada: "); 
Serial.println( Entrada);

delay(50);  


}



