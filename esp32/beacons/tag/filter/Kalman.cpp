#include "Kalman.h"

void Kalman::init(float xMes, float yMes, float realX, float realY){
    Matrix.Transpose(&A[0][0], SYSTEM_STATE_NUMBER, SYSTEM_STATE_NUMBER, &ATranspose[0][0]);
    Matrix.Transpose(&H[0][0], INPUT_DATA_NUMBER, SYSTEM_STATE_NUMBER, &HTranspose[0][0]);

    X[0][0] = realX; X[1][0] = realY; X[2][0] = xMes - realX; X[3][0] = yMes - realY; X[4][0] = 0; X[5][0] = 0; X[6][0] = 0; X[7][0] = 0;
    Y[0][0] = realX; Y[1][0] = realY; Y[2][0] = 0; Y[3][0] = 0;


}


void Kalman::update(float xRange, float yRange, float meanAccX, float meanAccY, float elapsedTime){
    Y[0][0] = xRange; Y[1][0] = yRange; Y[2][0] = meanAccX; Y[3][0] = meanAccY;
    A[0][4] = elapsedTime; A[0][6] = 1/2 * (elapsedTime * elapsedTime);
    A[1][5] = elapsedTime; A[0][7] = 1/2 * (elapsedTime * elapsedTime);
    A[4][6] = elapsedTime;
    A[5][7] = elapsedTime;

    prediction();
    estimation();
}

void Kalman::prediction(){
    //Xpred = A * X
    Matrix.Multiply(&A[0][0],&X[0][0], SYSTEM_STATE_NUMBER, SYSTEM_STATE_NUMBER, 1, &Xpred[0][0]);
    
    //P = A * P * A' + Q
        //tmp = P * A'
    float tmp[SYSTEM_STATE_NUMBER][SYSTEM_STATE_NUMBER];
    Matrix.Multiply(&P[0][0],&ATranspose[0][0], SYSTEM_STATE_NUMBER, SYSTEM_STATE_NUMBER, SYSTEM_STATE_NUMBER, &tmp[0][0]);
    delete [] ATranspose;
        //res = A*tmp
    float res[SYSTEM_STATE_NUMBER][SYSTEM_STATE_NUMBER];
    Matrix.Multiply(&A[0][0], &tmp[0][0], SYSTEM_STATE_NUMBER, SYSTEM_STATE_NUMBER, SYSTEM_STATE_NUMBER, &res[0][0]);
    delete [] tmp;
        //P = Res + Q
    Matrix.Add(&res[0][0], &Q[0][0], SYSTEM_STATE_NUMBER, SYSTEM_STATE_NUMBER, &P[0][0]);
}
void Kalman::estimation(){
    // K = P * H' * inv(R + H* P * H')
    float P_HT[SYSTEM_STATE_NUMBER][INPUT_DATA_NUMBER];
    float H_P_HT[INPUT_DATA_NUMBER][INPUT_DATA_NUMBER];
    float K[SYSTEM_STATE_NUMBER][INPUT_DATA_NUMBER];
    Matrix.Multiply(&P[0][0], &HTranspose[0][0], SYSTEM_STATE_NUMBER, SYSTEM_STATE_NUMBER, INPUT_DATA_NUMBER, &P_HT[0][0]);
    Matrix.Multiply(&H[0][0], &P_HT[0][0], INPUT_DATA_NUMBER, SYSTEM_STATE_NUMBER, INPUT_DATA_NUMBER, &H_P_HT[0][0]);
    Matrix.Add(&H_P_HT[0][0], &R[0][0], INPUT_DATA_NUMBER, INPUT_DATA_NUMBER, &H_P_HT[0][0]);
    Matrix.Invert(&H_P_HT[0][0],INPUT_DATA_NUMBER);
    Matrix.Multiply(&P_HT[0][0], &H_P_HT[0][0], SYSTEM_STATE_NUMBER, INPUT_DATA_NUMBER, INPUT_DATA_NUMBER, &K[0][0]);
    delete [] P_HT;
    delete [] H_P_HT;
    //

    //P = P - K * H * P
    float H_P[INPUT_DATA_NUMBER][SYSTEM_STATE_NUMBER];
    float K_H_P[SYSTEM_STATE_NUMBER][SYSTEM_STATE_NUMBER];
    Matrix.Multiply(&H[0][0], &P[0][0], INPUT_DATA_NUMBER, SYSTEM_STATE_NUMBER, SYSTEM_STATE_NUMBER, &H_P[0][0]);
    Matrix.Multiply(&K[0][0], &H_P[0][0], SYSTEM_STATE_NUMBER, INPUT_DATA_NUMBER, SYSTEM_STATE_NUMBER, &K_H_P[0][0]);
    delete [] H_P;
    Matrix.Add(&P[0][0], &K_H_P[0][0], SYSTEM_STATE_NUMBER, SYSTEM_STATE_NUMBER, &P[0][0]);
    delete [] K_H_P;
    //

    //X = Xpred + K *(Y - H * XPred)
    float H_Xpred[INPUT_DATA_NUMBER][1];
    Matrix.Multiply(&H[0][0], &Xpred[0][0], INPUT_DATA_NUMBER, SYSTEM_STATE_NUMBER, 1, &H_Xpred[0][0]);
    Matrix.Subtract(&Y[0][0], &H_Xpred[0][0], INPUT_DATA_NUMBER, 1, &H_Xpred[0][0]);
    Matrix.Multiply(&K[0][0], &H_Xpred[0][0], SYSTEM_STATE_NUMBER, INPUT_DATA_NUMBER, 1, &X[0][0]);
    Matrix.Add(&Xpred[0][0], &X[0][0], SYSTEM_STATE_NUMBER, 1, &X[0][0]);

}