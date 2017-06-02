#include "defs.h"
#include <stdio.h>

struct Mius_DUS_Status
{
    unsigned int     i_Vkl:1;
    unsigned int     i_Tep_Got:1;
    unsigned int     i_Toch_Got:1;
    unsigned int     i_zashkal0:1; //т.е. на начальном этапе нет отказа на потенциометре

    unsigned int     i_zashkal1:1; //т.е. на начальном этапе нет отказа на потенциометре
    unsigned int     i_zashkal2:1; //т.е. на начальном этапе нет отказа на потенциометре
    unsigned int     i_vklAng:1; // включение Датчика угла
    unsigned int     i_vklMOM:1; // включение Датчика момента

    unsigned int     i_vklUsil:1; // включение Усилителя

    unsigned int     i_Spare:7;

};

struct Mius_DUS_Interface
{
    unsigned short   i_Counter;  /*16-разрядный счетчик*/

    short   i_V_w0;     /*Напряжение по каналу 0 -> -10240 +10240 миллиВольт*/
    short   i_V_w1;     /*Напряжение по каналу 1*/
    short   i_V_w2;     /*Напряжение по каналу 2*/

    struct Mius_DUS_Status s_Stat; /*Состояние датчика*/
};

struct Mius_DUS_struct
{
    int    i_Initialized; // Задание начальных условий перед выполнением -?????

    int    i_Vkl;           /*Команда на включение датчика*/

    int    i_StartCounter;  /*Счетчик раскрутки датчика*/

    double o_Omg[3];

    struct Mius_DUS_Interface o_Output_Array;

    double u_wCurrent[3];
};

struct Mius_DUS_struct Mius_DUS_s; // п÷я─п╬я┌п╬я┌п╦п© п╬п©п╦я│п╟п╫п╦я▐ п©п╣я─п╣п╪п╣п╫п╫п╬п╧

//struct DUS_s_type DUS_s;

double V_w[3];    // п╫п╟п©я─я▐п╤п╣п╫п╦п╣ п©п╬ п╨п╟п╫п╟п╩п╟п╪
double temper;    // я┌п╣п╪п©п╣я─п╟я┌я┐я─п╟ п╥п╟ п╠п╬я─я┌п╬п╪
short St[16];    // п╪п╟я│я│п╦п╡ п©я─п╦п╥п╫п╟п╨п╬п╡ пЁп╬я┌п╬п╡п╫п╬я│я┌п╦
short Status;    // я│п╬я│я┌п╬я▐п╫п╦п╣ п╢п╟я┌я┤п╦п╨п╟: 1 - пЁп╬я┌п╬п╡; 0 - п╫п╣ пЁп╬я┌п╬п╡.
int i;
/*----------------------------------------------------------------------------------------------------------------------------*/

//float w0, w1, w2;                    // ОНЙЮГЮМХЪ ОНРЕМЖ. Я дся ХКХ Я ЕЦН ЛНДЕКХ (СЦК.ЯЙ. Б ПЮДХЮМЮУ Б ЯЕЙСМДС)
double W[3]; // {0,0,0};
//float Omg_out[3];
double dw0[3]={0,0,0};            // яКСВЮИМШИ ДПЕИТ
double M[3][3];                	 // лЮРПХЖЮ НЬХАНЙ СЯРЮМНБЙХ M
// float M_op;                      // нОПЕДЕКХРЕКЭ ЛЮРПХЖШ M
double dx=0,dy=0,dz=0;            // нЬХАЙХ СЯРЮМНБЙХ
double B[3][3];                   // лЮРПХЖЮ ОНЦПЕЬМНЯРЕИ ЛЮЯЬРЮАМНЦН ЙНЩТТХЖХЕМРЮ B
// float B_op;                      // нОПЕДЕКХРЕКЭ ЛЮРПХЖШ B
double bx=0,by=0,bz=0;            // оНЦПЕЬМНЯРХ ЛЮЯЬРЮАМНЦН ЙНЩТТХЖХЕМРЮ
//  float Mw[3];                    // Mw = M_ob*I_w
// int MIUS_s.k=1.0;           /* йНЩТТХЖХЕМР ВСБЯРБХРЕКЭМНЯРХ дся */
int i = 0;
//float *ptr[3] = {0};
double Mw[3], BMw[3];
int MIUS_status = 1;

void MIUS_Main(double o_Omg[3],double Omg_out[3])//float W[3], float Omg_out[3])
{
    /*  цКЮБМЮ ТСМЙЖХЪ.                                      */
    /*  оПЕНАПЮГНБЮМХЕ w Б Omg_out ОН ТНПЛЕКЕ                */
    /*  бУНДМШЕ ОЮПЮЛЕРПШ: W[i] - СЦКНБШЕ ЯЙНПНЯРХ           */
    /*  бШУНДМШЕ ОЮПЮЛЕРПШ: Omg_out[i]. Omg_out = B*M*W - dw0*/
    if (MIUS_status)
    {

      W[0] = o_Omg[0];  //  бУНДМШЕ СЦКНБШЕ ЯЙНПНЯРХ
      W[1] = o_Omg[1];
      W[2] = o_Omg[2];


      /*  бШВХЯКЕМХЕ СЦКНБНИ ЯЙНПНЯРХ.
         Omg_out = B*M*W - dw0,
         ЦДЕ W - БЕЙРНП СЦКНБНИ ЯЙНПНЯРХ Б ОПХАНПМНИ ЯХЯРЕЛЕ ЙННПДХМЮР, ОНКСВЕММЮЪ Я ДЮРВХЙЮ,
             dwo - ЯКСВЮИМШИ ДПЕИТ.
       M - ЛЮРПХЖЮ НЬХАНЙ СЯРЮМНБЙХ M.
              |  1      dy      -dz   |
       M =    | -dy     1        dx   |
              |  dz    -dx       1    |


      B - ЛЮРПХЖЮ ОНЦПЕЬМНЯРЕИ ЛЮЯЬРЮАМНЦН ЙНЩТТХЖХЕМРЮ B.
              |  1+bx    0       0   |
       B =    |   0     1+by     0   |
              |   0      0      1+bz |
      */


      M[0][0] = 1.0;	M[0][1] = dy;	M[0][2] = -dz;	/* матрица ошибок установки M  */
      M[1][0] = -dy;	M[1][1] = 1.0;	M[1][2] = dx;
      M[2][0] = dz;		M[2][1] = -dx;	M[2][2] = 1.0;

      short i,k;

      for (i = 0; i < 3; i++)
          Mw[i] = 0;

      for (i = 0; i < 3; i++)
          for (k = 0; k < 3; k++)
              Mw[i] += M[i][k]*W[k];

      B[0][0] = 1 + bx;		B[0][1] = 0;		B[0][2] = 0;	/* Матрица погрешностей масштабного коэффийиента B */
      B[1][0] = 0;			B[1][1] = 1 + by;	B[1][2] = 0;
      B[2][0] = 0;			B[2][1] = 0;		B[2][2] = 1 + bz;

      i = 0;
      k = 0;

      for (i = 0; i < 3; i++)
          BMw[i] = 0;

      for (i = 0; i < 3; i++)
          for (k = 0; k < 3; k++)
              BMw[i] += B[i][k]*Mw[k];

      i = 0;

      for (i=0; i < 3; i++)
      {
        Omg_out[i] = BMw[i] + dw0[i];
      }

    }

    return;
}
