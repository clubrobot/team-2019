#ifndef KALMAN_H
#define KALMAN_H

#include "MatrixMath.h"
#include "KalmanConstants.h"

class Kalman
{

    private:
        
        float Y[INPUT_DATA_NUMBER][1] = {   {0},
                                            {0},
                                            {0},
                                            {0}};

        float X[SYSTEM_STATE_NUMBER][1] ={  {1000},
                                            {1500},
                                            {0},
                                            {0},
                                            {0},
                                            {0},
                                            {0},
                                            {0}};

        float H[INPUT_DATA_NUMBER][SYSTEM_STATE_NUMBER] =  {{1,0,1,0,0,0,0,0},
                                                            {0,1,0,1,0,0,0,0},
                                                            {0,0,0,0,0,0,1,0},
                                                            {0,0,0,0,0,0,0,1}}; 
        float t = 1/8;
        float A[SYSTEM_STATE_NUMBER][SYSTEM_STATE_NUMBER] ={{1,0,0,0,t,0,(t*t/2),0},
                                                            {0,1,0,0,0,t,0,(t*t/2)},
                                                            {0,0,1,0,0,0,0,0},
                                                            {0,0,0,1,0,0,0,0},
                                                            {0,0,0,0,1,0,t,0},
                                                            {0,0,0,0,0,1,0,t},
                                                            {0,0,0,0,0,0,1,0},
                                                            {0,0,0,0,0,0,0,1}}; 

        float R[INPUT_DATA_NUMBER][INPUT_DATA_NUMBER] =    {{BEACONS_NOISE_X, 0 ,0 ,0},
                                                            {0, BEACONS_NOISE_Y ,0 ,0},
                                                            {0, 0 ,ACC_NOISE ,0},  
                                                            {0, 0 ,0 ,ACC_NOISE}};
        
        float Q[SYSTEM_STATE_NUMBER][SYSTEM_STATE_NUMBER] ={{ESTIMATION_ERROR_X,0,0,0,0,0,0,0},
                                                            {0,ESTIMATION_ERROR_Y,0,0,0,0,0,0},
                                                            {0,0,ESTIMATION_ERROR_BIAS_X,0,0,0,0,0},
                                                            {0,0,0,ESTIMATION_ERROR_BIAS_Y,0,0,0,0},
                                                            {0,0,0,0,ESTIMATION_ERROR_VX,0,0,0},
                                                            {0,0,0,0,0,ESTIMATION_ERROR_VY,0,0},
                                                            {0,0,0,0,0,0,ESTIMATION_ERROR_ACCX,0},
                                                            {0,0,0,0,0,0,0,ESTIMATION_ERROR_ACCY}};

        float Xpred[SYSTEM_STATE_NUMBER][1];
        float P[SYSTEM_STATE_NUMBER][SYSTEM_STATE_NUMBER] = {{INIT_VALUE,0,0,0,0,0,0,0},
                                                            {0,INIT_VALUE,0,0,0,0,0,0},
                                                            {0,0,INIT_VALUE,0,0,0,0,0},
                                                            {0,0,0,INIT_VALUE,0,0,0,0},
                                                            {0,0,0,0,INIT_VALUE,0,0,0},
                                                            {0,0,0,0,0,INIT_VALUE,0,0},
                                                            {0,0,0,0,0,0,INIT_VALUE,0},
                                                            {0,0,0,0,0,0,0,INIT_VALUE}};

        float ATranspose[SYSTEM_STATE_NUMBER][SYSTEM_STATE_NUMBER];
        float HTranspose[SYSTEM_STATE_NUMBER][INPUT_DATA_NUMBER]; 

        void estimation();
        void prediction();

    public: 
    
        void init(float xMes, float yMes, float realX, float realY);
        void update(float xRange, float yRange, float meanAccX, float meanAccY, float time);
        
        float getEstX(){return X[0][0];}
        float getEstY(){return X[1][0];}
        

};



#endif