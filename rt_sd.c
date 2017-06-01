#include "defs.h"
#include <stdio.h>
#include <math.h>

float Quat_pros(double H1[4],double H2[4],double H_P[4]) // умножение кватернионов
{
    H_P[0] = H1[0] * H2[0] - H1[1] * H2[1] - H1[2] * H2[2] - H1[3] * H2[3];
    H_P[1] = H1[0] * H2[1] + H1[1] * H2[0] + H1[2] * H2[3] - H1[3] * H2[2];
    H_P[2] = H1[0] * H2[2] + H1[2] * H2[0] + H1[3] * H2[1] - H1[1] * H2[3];
    H_P[3] = H1[0] * H2[3] + H1[3] * H2[0] + H1[1] * H2[2] - H1[2] * H2[1];
    return 0;
}

// H - кватернион ориентации; Si - вектор солнца; Scck - Вектор в связанной системе координат
unsigned int Sun_Dat(double H[4],double Si[3],double Scck[3],unsigned short Sn) // модель солнечного датчика
{
    int i;
    double H_S[4]; // сопряженный кватернион
    double H_P[4]; // первое произведение кватернионов
    double H_P2[4]; // второе произведение кватернионов
    double H_Si[4]; // Вектор положения солнца в инерциальной системе отсчета
    H_Si[0]=0;
    for(i=1;i<4;i++) H_Si[i] = Si[i-1];
    H_S[0] = H[0];
    printf("H_Si = [%f,%f,%f,%f]\n",H_Si[0],H_Si[1],H_Si[2],H_Si[3]);
    for(i=1;i<4;i++) H_S[i] = -H[i];
    double H_S_len = sqrt(H_S[0]*H_S[0] + H_S[1] * H_S[1] + H_S[2]*H_S[2] + H_S[3]*H_S[3]);
    for(i=0;i<4;i++) H_S[i] = H_S[i]/H_S_len;
    printf("len = %f; H_S = [%f,%f,%f,%f]\n",H_S_len,H_S[0],H_S[1],H_S[2],H_S[3]);
    Quat_pros(H,H_Si,H_P); // умножение кватерниона ориентации на вектор положения солнца
    printf("H_P = [%f,%f,%f,%f]\n",H_P[0],H_P[1],H_P[2],H_P[3]);
    Quat_pros(H_P,H_S,H_P2); // умножение первого произведения на сопряженный кватернион
    printf("H_P2 = [%f,%f,%f,%f]\n",H_P2[0],H_P2[1],H_P2[2],H_P2[3]);
    for(i=1;i<4;i++) Scck[i-1] = H_P2[i];
  //  for(i=1;i<4;i++) Scck[i-1] = H_P[i];
    if ((Scck[2])<=0.035) {
        Sn = 0;
    }else Sn=1;
    return Sn;
}
