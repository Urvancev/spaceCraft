#include "math.h"
#include "defs.h"

typedef
struct DUVS_s_type_struct  // Cтруктура - новый тип данных. При объявл-и никакая
                          // память не выделяется, реальных структур в памяти
                          // еще нет. Этот фрагмент просто скажет компилятору,
                          // что в программе могут быть элементы такого типа.
{
    int    i_Initialized;

        unsigned int Pvkl_dv[8];		// Прихнак включения i-ого двигателя
        unsigned int SostEC[8];		// Признак состояния электроклапана i-го двигателя
        double Pndv[8];			// Номинальные тяги двигателей в установившемся режиме, H
        double Tau1_dv[8];		// Времена задержки на переключение двигателей, c
        double Tau2up_dv[8];		// Постоянные времени нарастания тяги двигателей, c
        double Tau2dn_dv[8];		// Постоянные времени спада тяги двигателей, c
        double Rdv[8][3];		// Единичные вектора номинального направления тяги двигателей, -
        double Edv[8][3];		// Вектора отклонений тяги двигателей от номинальных направлений в ССК, -, 1-й индекс - номер двигателя,
                                // 2 - й индекс – ось ССК
        double Ldv[8][3];		// Вектор плеча тяги i-го двигателя относительно начала ССК, м
        double Ddv[8][3];		// Вектор отклонения точки приложения тяги i-го двигателя относительно номинального положения в ССК,м
        double Dcm[3];			// Радиус-вектор центра масс КА в ССК, -
        double Mdv[8][3];		// Управляющие моменты всех двигателей
        double Pdv[8][3];		// Вектор тяги i-го двигателя в базовой строительной системе координат (БССК)
        double Lrdv[8][3];		// Вектор тяги i-го двигателя в базовой строительной системе

        double o_Mdv[3];         //Управляющие моменты ДУ по 3 осям



} DUVS_s_type;

DUVS_s_type DUVS_s;

void DUVS_Init(void)
{
    DUVS_s.Pndv[0] = 0.5;
    DUVS_s.Pndv[1] = 0.5;
    DUVS_s.Pndv[2] = 0.5;
    DUVS_s.Pndv[3] = 0.5;
    DUVS_s.Pndv[4] = 0.5;
    DUVS_s.Pndv[5] = 0.5;
    DUVS_s.Pndv[6] = 0.5;
    DUVS_s.Pndv[7] = 0.5;

    DUVS_s.Tau1_dv[0] = 0.0;
    DUVS_s.Tau1_dv[1] = 0.0;
    DUVS_s.Tau1_dv[2] = 0.0;
    DUVS_s.Tau1_dv[3] = 0.0;
    DUVS_s.Tau1_dv[4] = 0.0;
    DUVS_s.Tau1_dv[5] = 0.0;
    DUVS_s.Tau1_dv[6] = 0.0;
    DUVS_s.Tau1_dv[7] = 0.0;

    DUVS_s.Tau2up_dv[0] = 0.3;
    DUVS_s.Tau2up_dv[1] = 0.3;
    DUVS_s.Tau2up_dv[2] = 0.3;
    DUVS_s.Tau2up_dv[3] = 0.3;
    DUVS_s.Tau2up_dv[4] = 0.3;
    DUVS_s.Tau2up_dv[5] = 0.3;
    DUVS_s.Tau2up_dv[6] = 0.3;
    DUVS_s.Tau2up_dv[7] = 0.3;

    DUVS_s.Tau2dn_dv[0] = 0.7;
    DUVS_s.Tau2dn_dv[1] = 0.7;
    DUVS_s.Tau2dn_dv[2] = 0.7;
    DUVS_s.Tau2dn_dv[3] = 0.7;
    DUVS_s.Tau2dn_dv[4] = 0.7;
    DUVS_s.Tau2dn_dv[5] = 0.7;
    DUVS_s.Tau2dn_dv[6] = 0.7;
    DUVS_s.Tau2dn_dv[7] = 0.7;

    DUVS_s.Rdv[0][0] = 1.0;		DUVS_s.Rdv[0][1] = 0.0;		DUVS_s.Rdv[0][2] = 0.0;
    DUVS_s.Rdv[1][0] = 1.0;		DUVS_s.Rdv[1][1] = 0.0;		DUVS_s.Rdv[1][2] = 0.0;
    DUVS_s.Rdv[2][0] = 1.0;		DUVS_s.Rdv[2][1] = 0.0;		DUVS_s.Rdv[2][2] = 0.0;
    DUVS_s.Rdv[3][0] = 1.0;		DUVS_s.Rdv[3][1] = 0.0;		DUVS_s.Rdv[3][2] = 0.0;
    DUVS_s.Rdv[4][0] = 0.0;		DUVS_s.Rdv[4][1] = 0.0;		DUVS_s.Rdv[4][2] = 1.0;
    DUVS_s.Rdv[5][0] = 0.0;		DUVS_s.Rdv[5][1] = 0.0;		DUVS_s.Rdv[5][2] = -1.0;
    DUVS_s.Rdv[6][0] = 0.0;		DUVS_s.Rdv[6][1] = 0.0;		DUVS_s.Rdv[6][2] = -1.0;
    DUVS_s.Rdv[7][0] = 0.0;		DUVS_s.Rdv[7][1] = 0.0;		DUVS_s.Rdv[7][2] = 1.0;

    DUVS_s.Edv[0][0] = 0.00;		DUVS_s.Edv[0][1] = 0.00;		DUVS_s.Edv[0][2] = 0.00;
    DUVS_s.Edv[1][0] = 0.00;		DUVS_s.Edv[1][1] = 0.00;		DUVS_s.Edv[1][2] = 0.00;
    DUVS_s.Edv[2][0] = 0.00;		DUVS_s.Edv[2][1] = 0.00;		DUVS_s.Edv[2][2] = 0.00;
    DUVS_s.Edv[3][0] = 0.00;		DUVS_s.Edv[3][1] = 0.00;		DUVS_s.Edv[3][2] = 0.00;
    DUVS_s.Edv[4][0] = 0.00;		DUVS_s.Edv[4][1] = 0.00;		DUVS_s.Edv[4][2] = 0.00;
    DUVS_s.Edv[5][0] = 0.00;		DUVS_s.Edv[5][1] = 0.00;		DUVS_s.Edv[5][2] = 0.00;
    DUVS_s.Edv[6][0] = 0.00;		DUVS_s.Edv[6][1] = 0.00;		DUVS_s.Edv[6][2] = 0.00;
    DUVS_s.Edv[7][0] = 0.00;		DUVS_s.Edv[7][1] = 0.00;		DUVS_s.Edv[7][2] = 0.00;

    DUVS_s.Ldv[0][0] = 0.05;		DUVS_s.Ldv[0][1] = 0.5;			DUVS_s.Ldv[0][2] = 0.0;
    DUVS_s.Ldv[1][0] = 0.05;		DUVS_s.Ldv[1][1] = 0.0;			DUVS_s.Ldv[1][2] = 0.5;
    DUVS_s.Ldv[2][0] = 0.05;		DUVS_s.Ldv[2][1] = -0.5;		DUVS_s.Ldv[2][2] = 0.0;
    DUVS_s.Ldv[3][0] = 0.05;		DUVS_s.Ldv[3][1] = 0.0;			DUVS_s.Ldv[3][2] = -0.5;
    DUVS_s.Ldv[4][0] = 0.2;			DUVS_s.Ldv[4][1] = 0.5;			DUVS_s.Ldv[4][2] = 0.0;
    DUVS_s.Ldv[5][0] = 0.2;			DUVS_s.Ldv[5][1] = 0.5;			DUVS_s.Ldv[0][2] = 0.0;
    DUVS_s.Ldv[6][0] = 0.2;			DUVS_s.Ldv[6][1] = 0.0;			DUVS_s.Ldv[6][2] = -0.5;
    DUVS_s.Ldv[7][0] = 0.2;			DUVS_s.Ldv[7][1] = 0.0;			DUVS_s.Ldv[7][2] = -0.5;

    DUVS_s.Ddv[0][0] = 0.00;		DUVS_s.Ddv[0][1] = 0.00;		DUVS_s.Ddv[0][2] = 0.00;
    DUVS_s.Ddv[1][0] = 0.00;		DUVS_s.Ddv[1][1] = 0.00;		DUVS_s.Ddv[1][2] = 0.00;
    DUVS_s.Ddv[2][0] = 0.00;		DUVS_s.Ddv[2][1] = 0.00;		DUVS_s.Ddv[2][2] = 0.00;
    DUVS_s.Ddv[3][0] = 0.00;		DUVS_s.Ddv[3][1] = 0.00;		DUVS_s.Ddv[3][2] = 0.00;
    DUVS_s.Ddv[4][0] = 0.00;		DUVS_s.Ddv[4][1] = 0.00;		DUVS_s.Ddv[4][2] = 0.00;
    DUVS_s.Ddv[5][0] = 0.00;		DUVS_s.Ddv[5][1] = 0.00;		DUVS_s.Ddv[5][2] = 0.00;
    DUVS_s.Ddv[6][0] = 0.00;		DUVS_s.Ddv[6][1] = 0.00;		DUVS_s.Ddv[6][2] = 0.00;
    DUVS_s.Ddv[7][0] = 0.00;		DUVS_s.Ddv[7][1] = 0.00;		DUVS_s.Ddv[7][2] = 0.00;

    DUVS_s.Dcm[0] = 0.50;
    DUVS_s.Dcm[1] = 0.05;
    DUVS_s.Dcm[2] = -0.05;

}


void DUVS_Main(unsigned short o_P[8],double i_Moment[3]) /*Вызывается на каждом такте*/
{
    int i;
    for (i = 0; i<8; i++)
            DUVS_s.SostEC[i] = o_P[i];
        /* Расчет плеч тяги двигателей относительно центра масс КА */
        /* Lrdv[i][j] - вектор плеча тяги i-го двигателя относительно начала БССК */
        /* Ldv[i][j] - радиус-вектор номинальной точки приложения тяги i-го двигателя относительно начала БССК */
        /* Ddv[i][j] - вектор отклонения точки приложения тяги i-го двигателя относительно номинального положения в БССК */
        /* Dcm[i] - радиус-вектор центра масс КА в БССК */
        /* Mdv[i][j] - управляющие моменты */
        /* Pdv[i][j] - вектор тяги i-го двигателя в базовой стоительной системе координат (БССК) */

    DUVS_s.Pndv[0] = 0.5;
    DUVS_s.Pndv[1] = 0.5;
    DUVS_s.Pndv[2] = 0.5;
    DUVS_s.Pndv[3] = 0.5;
    DUVS_s.Pndv[4] = 0.5;
    DUVS_s.Pndv[5] = 0.5;
    DUVS_s.Pndv[6] = 0.5;
    DUVS_s.Pndv[7] = 0.5;

    DUVS_s.Tau1_dv[0] = 0.0;
    DUVS_s.Tau1_dv[1] = 0.0;
    DUVS_s.Tau1_dv[2] = 0.0;
    DUVS_s.Tau1_dv[3] = 0.0;
    DUVS_s.Tau1_dv[4] = 0.0;
    DUVS_s.Tau1_dv[5] = 0.0;
    DUVS_s.Tau1_dv[6] = 0.0;
    DUVS_s.Tau1_dv[7] = 0.0;

    DUVS_s.Tau2up_dv[0] = 0.3;
    DUVS_s.Tau2up_dv[1] = 0.3;
    DUVS_s.Tau2up_dv[2] = 0.3;
    DUVS_s.Tau2up_dv[3] = 0.3;
    DUVS_s.Tau2up_dv[4] = 0.3;
    DUVS_s.Tau2up_dv[5] = 0.3;
    DUVS_s.Tau2up_dv[6] = 0.3;
    DUVS_s.Tau2up_dv[7] = 0.3;

    DUVS_s.Tau2dn_dv[0] = 0.7;
    DUVS_s.Tau2dn_dv[1] = 0.7;
    DUVS_s.Tau2dn_dv[2] = 0.7;
    DUVS_s.Tau2dn_dv[3] = 0.7;
    DUVS_s.Tau2dn_dv[4] = 0.7;
    DUVS_s.Tau2dn_dv[5] = 0.7;
    DUVS_s.Tau2dn_dv[6] = 0.7;
    DUVS_s.Tau2dn_dv[7] = 0.7;

    DUVS_s.Rdv[0][0] = 1.0;		DUVS_s.Rdv[0][1] = 0.0;		DUVS_s.Rdv[0][2] = 0.0;
    DUVS_s.Rdv[1][0] = 1.0;		DUVS_s.Rdv[1][1] = 0.0;		DUVS_s.Rdv[1][2] = 0.0;
    DUVS_s.Rdv[2][0] = 1.0;		DUVS_s.Rdv[2][1] = 0.0;		DUVS_s.Rdv[2][2] = 0.0;
    DUVS_s.Rdv[3][0] = 1.0;		DUVS_s.Rdv[3][1] = 0.0;		DUVS_s.Rdv[3][2] = 0.0;
    DUVS_s.Rdv[4][0] = 0.0;		DUVS_s.Rdv[4][1] = 1.0;		DUVS_s.Rdv[4][2] = 0.0;
    DUVS_s.Rdv[5][0] = 0.0;		DUVS_s.Rdv[5][1] = -1.0;	DUVS_s.Rdv[5][2] = 0.0;
    DUVS_s.Rdv[6][0] = 0.0;		DUVS_s.Rdv[6][1] = -1.0;	DUVS_s.Rdv[6][2] = 0.0;
    DUVS_s.Rdv[7][0] = 0.0;		DUVS_s.Rdv[7][1] = 1.0;		DUVS_s.Rdv[7][2] = 0.0;

    DUVS_s.Edv[0][0] = 0.00;		DUVS_s.Edv[0][1] = 0.00;		DUVS_s.Edv[0][2] = 0.00;
    DUVS_s.Edv[1][0] = 0.00;		DUVS_s.Edv[1][1] = 0.00;		DUVS_s.Edv[1][2] = 0.00;
    DUVS_s.Edv[2][0] = 0.00;		DUVS_s.Edv[2][1] = 0.00;		DUVS_s.Edv[2][2] = 0.00;
    DUVS_s.Edv[3][0] = 0.00;		DUVS_s.Edv[3][1] = 0.00;		DUVS_s.Edv[3][2] = 0.00;
    DUVS_s.Edv[4][0] = 0.00;		DUVS_s.Edv[4][1] = 0.00;		DUVS_s.Edv[4][2] = 0.00;
    DUVS_s.Edv[5][0] = 0.00;		DUVS_s.Edv[5][1] = 0.00;		DUVS_s.Edv[5][2] = 0.00;
    DUVS_s.Edv[6][0] = 0.00;		DUVS_s.Edv[6][1] = 0.00;		DUVS_s.Edv[6][2] = 0.00;
    DUVS_s.Edv[7][0] = 0.00;		DUVS_s.Edv[7][1] = 0.00;		DUVS_s.Edv[7][2] = 0.00;

    DUVS_s.Ldv[0][0] = 0.05;		DUVS_s.Ldv[0][1] = 0.0;			DUVS_s.Ldv[0][2] = 0.5;
    DUVS_s.Ldv[1][0] = 0.05;		DUVS_s.Ldv[1][1] = 0.5;			DUVS_s.Ldv[1][2] = 0.0;
    DUVS_s.Ldv[2][0] = 0.05;		DUVS_s.Ldv[2][1] = 0.0;		    DUVS_s.Ldv[2][2] = -0.5;
    DUVS_s.Ldv[3][0] = 0.05;		DUVS_s.Ldv[3][1] = -0.5;		DUVS_s.Ldv[3][2] = 0.0;
    DUVS_s.Ldv[4][0] = 0.2;			DUVS_s.Ldv[4][1] = 0.0;			DUVS_s.Ldv[4][2] = 0.5;
    DUVS_s.Ldv[5][0] = 0.2;			DUVS_s.Ldv[5][1] = 0.0;			DUVS_s.Ldv[0][2] = 0.5;
    DUVS_s.Ldv[6][0] = 0.2;			DUVS_s.Ldv[6][1] = 0.0;			DUVS_s.Ldv[6][2] = -0.5;
    DUVS_s.Ldv[7][0] = 0.2;			DUVS_s.Ldv[7][1] = 0.0;			DUVS_s.Ldv[7][2] = -0.5;

    DUVS_s.Ddv[0][0] = 0.00;		DUVS_s.Ddv[0][1] = 0.00;		DUVS_s.Ddv[0][2] = 0.00;
    DUVS_s.Ddv[1][0] = 0.00;		DUVS_s.Ddv[1][1] = 0.00;		DUVS_s.Ddv[1][2] = 0.00;
    DUVS_s.Ddv[2][0] = 0.00;		DUVS_s.Ddv[2][1] = 0.00;		DUVS_s.Ddv[2][2] = 0.00;
    DUVS_s.Ddv[3][0] = 0.00;		DUVS_s.Ddv[3][1] = 0.00;		DUVS_s.Ddv[3][2] = 0.00;
    DUVS_s.Ddv[4][0] = 0.00;		DUVS_s.Ddv[4][1] = 0.00;		DUVS_s.Ddv[4][2] = 0.00;
    DUVS_s.Ddv[5][0] = 0.00;		DUVS_s.Ddv[5][1] = 0.00;		DUVS_s.Ddv[5][2] = 0.00;
    DUVS_s.Ddv[6][0] = 0.00;		DUVS_s.Ddv[6][1] = 0.00;		DUVS_s.Ddv[6][2] = 0.00;
    DUVS_s.Ddv[7][0] = 0.00;		DUVS_s.Ddv[7][1] = 0.00;		DUVS_s.Ddv[7][2] = 0.00;

    DUVS_s.Dcm[0] = 0.50;
    DUVS_s.Dcm[1] = 0.05;
    DUVS_s.Dcm[2] = -0.05;



        int j;
        for (i = 0; i<8; i++)
        {
            for (j = 0; j<3; j++)
            {
                DUVS_s.Lrdv[i][j]=DUVS_s.Ldv[i][j]+DUVS_s.Ddv[i][j]-DUVS_s.Dcm[j];
            }
        }

        /* Метод вычисления тяги каждого двигателя */
        /* Pvkl_dv[i] - признак включения i-го двигателя */
        /* SostEC[i] - признак состояния ЭК i-го двигателя */
        /* Pmdv[i] - расчетная тяга i-го двигателя в установившемся режиме, Н */
        /* Pndv[i] - номинальная тяга i-го двигателя в установившемся режиме, Н*/
        /* DPdv[i] – погрешность тяги i-го двигателя в установившемся режиме, Н */
        /* Pt1dv[i] - расчетная тяга i-го двигателя с учетом задержки на переключение двигателя, Н */
        /* Cdv[i] – счетчик тактов задержки на переключение i-го двигателя */
        /* Tau1_dv[i] - время задержки на переключение i-го двигателя, с */
        /* T0 – такт вызова ММ ДУ (0,1 с). */
        /* Ptdv[i] - расчетная тяга i-го двигателя с учетом времени нарастания (спада) тяги, Н */
        /* Tau2up_dv[i] – постоянная времени нарастания тяги i-го двигателя, с */
        /* Tau2dn_dv[i] – постоянная времени спада тяги i-го двигателя, с */
        /* Pdv[i] - вектор тяги i-го двигателя в базовой стоительной системе координат (БССК) */
        /* Rdv[i] - единичный вектор номинального направления тяги i-го двигателя в БССК */
        /* Edv[i] - вектор отклонения тяги i-го двигателя от номинального направления в БССК */

        double DPdv[8] = {0};
        double Pmdv[8];
       // float Dpdv[8] = {0};
        static double Ptdv[8] = {0};
        static unsigned Cdv[8] = {0};
        static double Pt1dv[8] = {0};
        double T0 = 0.1;
        i=0;
        j=0;

        /*DUVS_s.SostEC[0] = 1;
        for (i = 1; i<8; i++) DUVS_s.SostEC[i] = 0;*/ /* отладка */

        for (i = 0; i<8; i++)
        {
            DUVS_s.Pvkl_dv[i] = DUVS_s.SostEC[i];							/* вычисление признака включения двигателя */
            Pmdv[i] = (DUVS_s.Pndv[i] + DPdv[i])*DUVS_s.Pvkl_dv[i]; 	/* вычисление расчетной тяги двигателя в установившемся режиме */

            if (Pmdv[i] != Pt1dv[i])								/* учет задержки на переключение двигателей */
            {
                if (Cdv[i]>=DUVS_s.Tau1_dv[i]/T0)
                {
                    Pt1dv[i] = Pmdv[i];
                    Cdv[i] = 0;
                } else
                    Cdv[i] = Cdv[i] + 1;
            }

            if ((Pt1dv[i] != 0)&(DUVS_s.Tau2up_dv[i] >= T0))				/* учет времени нарастания (спада) тяги */
                Ptdv[i] = Ptdv[i] + T0/DUVS_s.Tau2up_dv[i]*(Pt1dv[i] - Ptdv[i]);
            else
                if ((Pt1dv[i] == 0)&(DUVS_s.Tau2dn_dv[i] >= T0))
                    Ptdv[i] = Ptdv[i] + T0/DUVS_s.Tau2dn_dv[i]*(Pt1dv[i] - Ptdv[i]);
                else
                    Ptdv[i] = Pt1dv[i];

            for (j = 0; j<3; j++)
                DUVS_s.Pdv[i][j]=Ptdv[i]*(DUVS_s.Rdv[i][j]+DUVS_s.Edv[i][j]);		/* преобразование тяги в векторную форму */

           // print (Pdv);
        }

        //DUVS_f_pr(&DUVS_s.Lrdv[], &DUVS_s.Pdv[], &DUVS_s.Mdv[]);
        i=0;
        DUVS_s.o_Mdv[0] = 0;
        DUVS_s.o_Mdv[1] = 0;
        DUVS_s.o_Mdv[2] = 0;

        for (i=0; i<8; i++)
        {

            DUVS_s.Mdv[i][0] = - DUVS_s.Lrdv[i][1]*DUVS_s.Pdv[i][2] + DUVS_s.Lrdv[i][2]*DUVS_s.Pdv[i][1];
            DUVS_s.Mdv[i][1] = - DUVS_s.Lrdv[i][2]*DUVS_s.Pdv[i][0] + DUVS_s.Lrdv[i][0]*DUVS_s.Pdv[i][2];
            DUVS_s.Mdv[i][2] = - DUVS_s.Lrdv[i][0]*DUVS_s.Pdv[i][1] + DUVS_s.Lrdv[i][1]*DUVS_s.Pdv[i][0];

            DUVS_s.o_Mdv[0] = DUVS_s.o_Mdv[0] + DUVS_s.Mdv[i][0];
            DUVS_s.o_Mdv[1] = DUVS_s.o_Mdv[1] + DUVS_s.Mdv[i][1];
            DUVS_s.o_Mdv[2] = DUVS_s.o_Mdv[2] + DUVS_s.Mdv[i][2];
        }

    for (i=0; i<3; i++)
    i_Moment[i] = DUVS_s.o_Mdv[i];

}
