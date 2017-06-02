#include "defs.h"

struct Mdl_DOR_struct
{
    int    i_Initialized;

    double i_Moment[3];
    double o_Omg[3];
    double o_Quat[4];
    double o_y[7];
    double u_J[3][3];
    double u_invJ[3][3];
    double u_h;
};

struct Mdl_DOR_struct Mdl_DOR_s;

void Mdl_DOR_Init(void)
{
    /*  входные параметры модели */
    Mdl_DOR_s.i_Moment[0] = 0;
    Mdl_DOR_s.i_Moment[1] = 0;
    Mdl_DOR_s.i_Moment[2] = 0;
    /* выходные параметры модели */
    Mdl_DOR_s.o_Omg[0] = 0;
    Mdl_DOR_s.o_Omg[1] = 0;
    Mdl_DOR_s.o_Omg[2] = 0;
 /*   Mdl_DOR_s.o_Quat[0] = 1;
    Mdl_DOR_s.o_Quat[1] = 0;
    Mdl_DOR_s.o_Quat[2] = 0;
    Mdl_DOR_s.o_Quat[3] = 0;
  */  Mdl_DOR_s.o_y[0] = 1;
    Mdl_DOR_s.o_y[1] = 0;
    Mdl_DOR_s.o_y[2] = 0;
    Mdl_DOR_s.o_y[3] = 0;
    Mdl_DOR_s.o_y[4] = 0;
    Mdl_DOR_s.o_y[5] = 0;
    Mdl_DOR_s.o_y[6] = 0;
    /* Уставочные параметры модели */
    Mdl_DOR_s.u_J[0][0] = 600; Mdl_DOR_s.u_J[0][1] = 0;   Mdl_DOR_s.u_J[0][2] = 0;	/* тензор инерции*/
    Mdl_DOR_s.u_J[1][0] = 0;   Mdl_DOR_s.u_J[1][1] = 600; Mdl_DOR_s.u_J[1][2] = 0;
    Mdl_DOR_s.u_J[2][0] = 0;   Mdl_DOR_s.u_J[2][1] = 0;   Mdl_DOR_s.u_J[2][2] = 600;
    Mdl_DOR_s.u_invJ[0][0] = 0.00167;  Mdl_DOR_s.u_invJ[0][1] = -0.00005; Mdl_DOR_s.u_invJ[0][2] = -0.00005;	/* обратный тензор инерции*/
    Mdl_DOR_s.u_invJ[1][0] = 0.00005;  Mdl_DOR_s.u_invJ[1][1] = 0.00167;  Mdl_DOR_s.u_invJ[1][2] = -0.00005;
    Mdl_DOR_s.u_invJ[2][0] = 0.00005;  Mdl_DOR_s.u_invJ[2][1] = 0.00005;  Mdl_DOR_s.u_invJ[2][2] = 0.00167;
    Mdl_DOR_s.u_h = 0.1;	/* Величина шага сетки по x для метода интегрирования Рунге-Кутты 4-ого порядка */
}

int Mdl_DOR_f_runge(double *u, double *y, int n, int (*ptr2Fnc)(double *, double *, double *))
{
   /* метод интегрирования (Рунге-Кутты 4-го порядка) */
    /* u[3x1] - вектор управляющего воздействия */
    /* y[6x1] - вектор состояния */
    /* n      - размерность вектора состояния */
    /* ptr2Fnc - указатель на функцию расчета производной вектора состояния */

    short i;
    double k1[7], k2[7], k3[7], k4[7], y1[7];

    (*ptr2Fnc)(u,y,&k1[0]);
    for (i = 0; i < n; i++)
        y1[i] = y[i] + k1[i]*Mdl_DOR_s.u_h/2;
    (*ptr2Fnc)(u,y1,&k2[0]);
    for (i = 0; i < n; i++)
        y1[i] = y[i] + k2[i]*Mdl_DOR_s.u_h/2;
    (*ptr2Fnc)(u,y1,&k3[0]);
    for (i = 0; i < n; i++)
        y1[i] = y[i] + k3[i]*Mdl_DOR_s.u_h;
    (*ptr2Fnc)(u,y1,&k4[0]);

    for (i = 0; i < n; i++)
        y[i] += (k1[i] + 2*k2[i] + 2*k3[i] + k4[i])*Mdl_DOR_s.u_h/6;

    return 0;

}

int Mdl_DOR_f_MatrVectProd(double **M, double *V, double *O)
{
    /* Метод вычисления произведения матрицы на вектор */
    /* M[3x3] - исходная матрица */
    /* V[3x1] - исходный вектор */
    /* O[3x1] - результат произведения M*V */


    short i,k;

    for (i = 0; i < 3; i++)
        O[i] = 0;

    for (i = 0; i < 3; i++)
        for (k = 0; k < 3; k++)
            O[i] += M[i][k]*V[k];

    return 0;
}

int Mdl_DOR_f_CrossProd(double *V1, double *V2, double *O)
{
    /* Метод вычисления векторного произведения векторов */
    /* V1[3x1] - ОЕПБШИ ХЯУНДМШИ БЕЙРНП */
    /* V2[3x1] - БРНПНИ ХЯУНДМШИ БЕЙРНП */
    /* O[3x1] - ПЕГСКЭРЮР ОПНХГБЕДЕМХЪ V1*V2 */


    O[0] = V1[1]*V2[2] - V2[1]*V1[2];
    O[1] = - V1[0]*V2[2] + V2[0]*V1[2];
    O[2] = V1[0]*V2[1] - V2[0]*V1[1];

    return 0;
}

int Mdl_DOR_f_QuatProd(double *Q1, double *Q2, double *O)
{
    /* Метод вычисления произведения кватернионов */
    /* Q1[4x1] - ОЕПБШИ ХЯУНДМШИ ЙБЮРЕПМХНМ */
    /* Q2[4x1] - БРНПНИ ХЯУНДМШИ ЙБЮРЕПМХНМ */
    /* O[4x1] - ПЕГСКЭРЮР ОПНХГБЕДЕМХЪ Q1*Q2 */


    O[0] = Q1[0]*Q2[0] - Q1[1]*Q2[1] - Q1[2]*Q2[2] - Q1[3]*Q2[3];
    O[1] = Q1[0]*Q2[1] + Q1[1]*Q2[0] + Q1[2]*Q2[3] - Q1[3]*Q2[2];
    O[2] = Q1[0]*Q2[2] + Q1[2]*Q2[0] + Q1[3]*Q2[1] - Q1[1]*Q2[3];
    O[3] = Q1[0]*Q2[3] + Q1[3]*Q2[0] + Q1[1]*Q2[2] - Q1[2]*Q2[1];

    return 0;
}

int Mdl_DOR_f_AngMotionEquatBq(double *u, double *y, double *dy)
{
   /* метод расчета диффиринциального уравнения кинематики */
    /* u = [OmgX, OmgY, OmgZ] - вектор угловой скорости */
    /* y = [Lmbd0, Lmbd1, Lmbd2, Lmbd3] - кватернион ориентации */
    /* dy = dy/dt - производная кватерниона ориентации */


    short i;
    double V[4], Qw[4] = {0}, eps = 0;

    for (i = 1; i < 4; i++)
        Qw[i] = u[i-1];
    Mdl_DOR_f_QuatProd(&y[0], &Qw[0], &V[0]); /* V = Lmbd*Qw */

    for (i = 0; i < 4; i++)
        eps += y[i]*y[i];
    eps += -1;                               /* отклонение нормы от 1 */

    for (i = 0; i < 4; i++)
        dy[i] = 0.5*V[i] - eps*y[i]; /* dy = 0.5*Lmbd*Qw - eps*Lmbd (с автокоррекцией нормы) */

    return 0;
}

int Mdl_DOR_f_AngMotionEquatBw(double *u, double *y, double *dy)
{
   /* метод расчета диффиренциального уравнения динамики */
    /* u = [MuprX, MuprY, MuprZ] - вектор управляющих моментов */
    /* y = [OmgX, OmgY, OmgZ] - вектор угловых скоростей */
    /* dy = dy/dt - производная вектора угловых скоростей */


    short i;
    double V[3], Cr[3], M[3];
    double *ptr[3] = {0};

    ptr[0] = Mdl_DOR_s.u_J[0];
    ptr[1] = Mdl_DOR_s.u_J[1];
    ptr[2] = Mdl_DOR_s.u_J[2];
    Mdl_DOR_f_MatrVectProd(&ptr[0],&y[0],&V[0]);  /* V = J*Omg */
    Mdl_DOR_f_CrossProd(&y[0],&V[0],&Cr[0]);      /* Cr = cross(Omg,J*Omg) */
    for (i = 0; i < 3; i++)
        M[i] = u[i] - Cr[i];                      /* M = u - cross(Omg,J*Omg); */
    ptr[0] = Mdl_DOR_s.u_invJ[0];
    ptr[1] = Mdl_DOR_s.u_invJ[1];
    ptr[2] = Mdl_DOR_s.u_invJ[2];
    Mdl_DOR_f_MatrVectProd(&ptr[0],&M[0],&dy[0]); /* dy = inv(J)*(u - cross(Omg,J*Omg)) */

    return 0;
}

void Mdl_DOR_Main(double i_Moment[3],double o_Omg[3],double o_Quat[4])
{
    if (Mdl_DOR_s.i_Initialized == 0) {
        Mdl_DOR_Init();
        Mdl_DOR_s.i_Initialized++;
    }
    int (*ptr2Fnc)(double *, double *, double *);
    ptr2Fnc = &Mdl_DOR_f_AngMotionEquatBw;
    Mdl_DOR_f_runge(&i_Moment[0], &o_Omg[0], 3, ptr2Fnc);	/* dw/wt = f(M) */
    ptr2Fnc = &Mdl_DOR_f_AngMotionEquatBq;
    Mdl_DOR_f_runge(&o_Omg[0], &o_Quat[0], 4, ptr2Fnc);		/* dLmbd/dt = f(w) */
}
