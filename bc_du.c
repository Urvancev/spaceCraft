#include "math.h"
#include <string.h>
#include <stdio.h>

#include "defs.h"

typedef
struct DU_s_type_struct
{
// входы
    double i_sigma[3];          //аргумент управления
//выходы
    unsigned short o_P[8];       //Признаки включения двигателей на текущем шаге
    double u_sigma1[3];           //Пороги включения двигателей
    double u_sigma2[3];           //Пороги отключения двигателей
} DU_s_type;

DU_s_type DU_s;

void DU_Main(double i_sigma[3],unsigned short o_P[8]) /*Вызывается на каждом такте*/ // программа управления ДУ
{
    int i;
    for(i=0;i<3;i++)
    {
    DU_s.i_sigma[i] = i_sigma[i];
    }


    for(i=0; i<8; i++)
    {
        DU_s.o_P[i]=0;
    }

    DU_s.u_sigma1[0]=0.02*M_PI/180;
    DU_s.u_sigma1[1]=0.02*M_PI/180;
    DU_s.u_sigma1[2]=0.02*M_PI/180;
    DU_s.u_sigma2[0]=0.04*M_PI/180;
    DU_s.u_sigma2[1]=0.04*M_PI/180;
    DU_s.u_sigma2[2]=0.04*M_PI/180;

    if(DU_s.i_sigma[0]>DU_s.u_sigma2[0])
    {
        DU_s.o_P[4]=1;
        DU_s.o_P[6]=1;
    }
    else if(DU_s.i_sigma[0]<DU_s.u_sigma1[0])
    {
        DU_s.o_P[4]=0;
        DU_s.o_P[6]=0;
    }

    if(DU_s.i_sigma[0]<-(DU_s.u_sigma2[0]))
    {

        DU_s.o_P[5]=1;
        DU_s.o_P[7]=1;

    }
    else if(DU_s.i_sigma[0]>-(DU_s.u_sigma1[0]))
    {
        DU_s.o_P[5]=0;
        DU_s.o_P[7]=0;
    }

    if(DU_s.i_sigma[1]>DU_s.u_sigma2[1])
    {
        DU_s.o_P[2]=1;
    }
    else if(DU_s.i_sigma[1]<DU_s.u_sigma1[1])
    {
        DU_s.o_P[2]=0;
    }

    if(DU_s.i_sigma[1]<-(DU_s.u_sigma2[1]))
    {
        DU_s.o_P[0]=1;
    }
    else if(DU_s.i_sigma[1]>-(DU_s.u_sigma1[1]))
    {
        DU_s.o_P[0]=0;
    }

    if(DU_s.i_sigma[2]>DU_s.u_sigma2[2])
    {
        DU_s.o_P[1]=1;
    }
    else if(DU_s.i_sigma[2]<DU_s.u_sigma1[2])
    {
        DU_s.o_P[1]=0;
    }

    if(DU_s.i_sigma[2]<-(DU_s.u_sigma2[2]))
    {
        DU_s.o_P[3]=1;
    }
    else if(DU_s.i_sigma[2]>-(DU_s.u_sigma1[2]))
    {
        DU_s.o_P[3]=0;
    }

    for(i=0;i<8;i++)
    {
        o_P[i] = DU_s.o_P[i];
    }
}
