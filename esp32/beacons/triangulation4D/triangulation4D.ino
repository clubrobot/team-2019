#include <MatrixMath.h>

const float x_1 = 5;
const float y_1 = -49;
const float x_2 = 1000;
const float y_2 = 3049;
const float x_3 = 1950;
const float y_3 = -49;
const float x_4 = 21.18;
const float y_4 = 1326;

const float z_tag = 484.3;
const float z_anchor = 438.3;
const float z_central = 1016.3;

float d1 = 1100;
float d2 = 2297;
float d3 = 1580;
float d4 = 900;


void setup() {
  Serial.begin(115200);
  Serial.println("starting computation");
  unsigned long startTime = micros();
  d1 = sqrt(d1*d1 - ((z_anchor - z_tag)*(z_anchor - z_tag))); // projection dans le plan des tags
  d2 = sqrt(d2*d2 - ((z_anchor - z_tag)*(z_anchor - z_tag))); // projection dans le plan des tags
  d3 = sqrt(d3*d3 - ((z_anchor - z_tag)*(z_anchor - z_tag))); // projection dans le plan des tags
  d4 = sqrt(d4*d4 - ((z_central - z_tag)*(z_central - z_tag))); // projection dans le plan des tags
  float A[3][2] ={  {-2*(x_1-x_4), -2*(y_1-y_4)},\
                    {-2*(x_2-x_4), -2*(y_2-y_4)},\
                    {-2*(x_3-x_4), -2*(y_3-y_4)}};
  
  float b[3] = {  d1*d1 - x_1*x_1 - y_1*y_1 - d4*d4 + x_4*x_4 + y_4*y_4, d2*d2 - x_2*x_2 - y_2*y_2 - d4*d4 + x_4*x_4 + y_4*y_4, d3*d3 - x_3*x_3 - y_3*y_3 - d4*d4 + x_4*x_4 + y_4*y_4};
  float Atr[2][3];
  Matrix.Transpose(&A[0][0],3,2,&Atr[0][0]);
  float Ainv[2][2];
  Matrix.Multiply(&Atr[0][0],&A[0][0],2,3,2,&Ainv[0][0]);
  //memcpy(&Ainv[0][0],&A[0][0],sizeof(float)*4);
  Matrix.Invert(&Ainv[0][0],2);
  float Atemp[2][3];
  Matrix.Multiply(&Ainv[0][0],&Atr[0][0],2,2,3,&Atemp[0][0]);
  float p[2];
  Matrix.Multiply(&Atemp[0][0],&b[0],2,3,1,&p[0]);
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
