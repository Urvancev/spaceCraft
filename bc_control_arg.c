#include <stdio.h>
#include "defs.h"

typedef
struct DOR_s_type_struct
{
//установочный переменные
    double u_Isum_max;       //максимальное число интегральной суммы
//внутренние переменные
    double n_wStab[3];       //параметры стабилизации скорости
    double n_predwStab[3];   //предыдущие параметры стабилизации
    double n_Pgain[3];       //пропорциональная составляющая
    double n_Igain[3];       //интегральная составляющая
    double n_Dgain[3];       //дифференциальная составляющая
    double n_Isum[3];        //интегральная сумма
    double n_Tdiscr;   //период дискретизации
} DOR_s_type;

DOR_s_type DOR_s;

//Функция расчета параметров стабилизации
int DOR_f_CalcStabPar(double w_stab[3], double w_pro[3], double w_curr[3])
{
    int i;
    for ( i = 0; i <= 2; i++)
    {
        w_stab[i] = w_pro[i] - w_curr[i];

    }
    return 0;
}

//Функция расчета аргумента управления
int DOR_f_CalcContrArg(double w_stab[3], double sigma[3])
    {
    int i;
    DOR_s.n_Tdiscr = 0.2;
//Вычисление интегральной составляющей
    for ( i = 0; i <= 2; i++)
        {
        //добавить ограничение суммы
         DOR_s.n_Isum[i] += w_stab[i];

         DOR_s.n_Igain[i] = DOR_s.n_Isum[i] * pso.u_Ki * DOR_s.n_Tdiscr;
        }

//Вычисление пропорциональной составляющей
    for ( i = 0; i <= 2; i++)
        {
        DOR_s.n_Pgain[i] = w_stab[i] * pso.u_Kp;
        }

//Вычисление пропорциональной составляющей
     for ( i = 0; i <= 2; i++)
         {
         DOR_s.n_Dgain[i] = ((w_stab[i] - DOR_s.n_predwStab[i]) / DOR_s.n_Tdiscr) * pso.u_Kd;
         }

//Вычисление аргумента управления
    for ( i = 0; i<= 2; i++)
    {
        sigma[i] = DOR_s.n_Pgain[i] + DOR_s.n_Igain[i] + DOR_s.n_Dgain[i];

    }
    return 0;
}
