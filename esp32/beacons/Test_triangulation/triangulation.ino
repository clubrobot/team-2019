#include "MatrixMath.h"

const float x_1 = 5;
const float y_1 = -49;
const float x_2 = 1000;
const float y_2 = 3049;
const float x_3 = 1950;
const float y_3 = -49;

float d1 = 1500;
float d2 = 2097;
float d3 = 1380;

void setup() {
  Serial.begin(115200);
  Serial.println("starting computation");
  unsigned long startTime = micros();
  float A[2][2] ={  {-2*(x_1-x_3), -2*(y_1-y_3)},\
                    {-2*(x_2-x_3), -2*(y_2-y_3)}};
  
  float b[2] = {  d1*d1 - x_1*x_1 - y_1*y_1 - d3*d3 + x_3*x_3 + y_3*y_3, d2*d2 - x_2*x_2 - y_2*y_2 - d3*d3 + x_3*x_3 + y_3*y_3};
  float Ainv[2][2];
  memcpy(&Ainv[0][0],&A[0][0],sizeof(float)*4);
  Matrix.Invert(&Ainv[0][0],2);
  float p[2];
  Matrix.Multiply(&Ainv[0][0],&b[0],2,2,1,&p[0]);
  unsigned long totalTime =  micros() - startTime;
  Serial.print("Computation finished in ");
  Serial.print(totalTime);
  Serial.println("us");
  Serial.println("Point found : ");
  Matrix.Print(&p[0],1,2,"p");
  Serial.println("other matrix : ");
  Matrix.Print(&A[0][0],2,2,"A");
  Matrix.Print(&Ainv[0][0],2,2,"Ainv");
  Matrix.Print(&b[0],2,1,"b");
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
