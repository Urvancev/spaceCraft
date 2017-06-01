#include "defs.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#define MAX_SPEED 2*M_PI/180 //максимальная скорость вращения по оси 3 градуса в секунду
#define K1 (2*(M_PI/180))
#define D (2*(M_PI/180))
#define K3 (0.04*(M_PI/180))
#define I (2*(M_PI/180))
#define RD *M_PI/180
//float K1=1;
int step=0;
double deg = 180;
double S_old[3];
int st0 = -1;
int st1 = -1;
double E_error[3];

double omg_old[3];
int seq = 0;
int seq0,seq1;
int sun_fail;

int P_Sun_O(double S[3],double w_pro[3],unsigned short Sun,double omg[3])
{
    //printf("step = %d \n ",step);
    int i;
    for(i = 0; i < 3; i++) {
       // S_old[i] = S[i];
        E_error[i] += S[i];
    }
    switch(step){
        case 3:
            if (seq >= 5) {
                seq = 0;
                if (( fabs(S[0]) -fabs(S_old[0]) > 0) && (fabs(omg_old[1]) < fabs(omg[1])) && omg_old[1]*omg[1] > 0) {
                    printf("%7.3f < %7.3f\n",fabs(omg_old[1]*(180/M_PI)),fabs(omg[1]*(180/M_PI)));

                    if (st1 == 1 && seq1 >= 5) {
                        st1 = -1;
                        seq1 = 0;
                    }else if (seq1 >= 5){
                        st1 = 1;
                        seq1 = 0;
                    }
                    seq1++;
                }
                if (( fabs(S[1]) -fabs(S_old[1]) > 0) && (fabs(omg_old[0]) < fabs(omg[0]))&& omg_old[1]*omg[1] > 0) {
                    printf("0:s %7.3f < %7.3f || w %7.3f < %7.3f\n",fabs(S_old[1]),fabs(S[1]),fabs(omg_old[0]*(180/M_PI)),fabs(omg[0]*(180/M_PI)));

                    if (st0 == 1 && seq0 >= 5){
                        seq0 = 0;
                        st0 = -1;
                    }
                    else if (seq0 >= 5) {
                        seq0 = 0;
                        st0 = 1;
                    }
                    seq0++;
                }

                for (i = 0; i < 3; i++){
                    omg_old[i] = omg[i];
                    S_old[i] = S[i];
                    //printf("omg_old[%d] = %7.3f S_old[%d] = %7.3f\n",i,omg[i]*(180/M_PI),i,S_old[i]);
                }
            }
            seq++;

            w_pro[0] = 0;
            w_pro[1] = 0;
            w_pro[2] = 0;
            if(-(K1*S[1] ) < (-MAX_SPEED)) {
                w_pro[0] = st0*MAX_SPEED;
            }
            if(fabs(K1*S[1])<=MAX_SPEED) {
                w_pro[0] = st0*(-K1*S[1]);
            }
            if(-(K1*S[1]) > MAX_SPEED) {
                w_pro[0] = st0*(-MAX_SPEED);
            }

            if(K1*S[0] < (-MAX_SPEED)) {
                w_pro[1] = st1*(-MAX_SPEED);
            }
            if(fabs(K1*S[0])<=MAX_SPEED) {
                w_pro[1] = st1*(K1*S[0]);
            }
            if(K1*S[0] > MAX_SPEED) {
                w_pro[1] = st1*MAX_SPEED;
            }

            if(Sun == 0) {
                step=1;
                sun_fail++;
            }
            break;
        case 2:
            w_pro[0] = MAX_SPEED;
            w_pro[1] = 0;
            w_pro[2] = 0;
            if (Sun == 1)
            {
                step=3;
            }
            break;
        case 1:
            w_pro[0] = 0;
            w_pro[1] = 0;
            w_pro[2] = 0;

            if(6*(deg/90)*(M_PI/180)>MAX_SPEED)	 	w_pro[1] = MAX_SPEED;
            if(6*(deg/90)*(M_PI/180)<=MAX_SPEED)	w_pro[1] = 6*(deg/90)*(M_PI/180);

            deg = deg - fabs(omg[1])*(180/M_PI)*0.1;
            //printf("deg = %f\n",deg);
            if (deg <= 1)
            {
                step=2;
                //deg = 180;
            }
            if (Sun == 1)
            {
                step=3;
            }
            break;
        case 0:
            w_pro[0] = 0;
            w_pro[1] = 0;
            w_pro[2] = 0;
            if (fabs(omg[0])<K3 && fabs(omg[1])<K3 && fabs(omg[2])<K3)
            {
                step=1;
            }
            break;
        default:
            w_pro[0] = 0;
            w_pro[1] = 0;
            w_pro[2] = 0;
            break;
    }
    return Sun;
}
