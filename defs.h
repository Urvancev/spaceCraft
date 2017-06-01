#ifndef DEFS
#define DEFS

#endif // DEFS

#include <pthread.h>

extern int TCP_Server_init(unsigned short port, char* host);
extern int Socket_recv(int fd, void *data, int len);
extern int Socket_send(int fd, const void *data, int len);
extern void clearStr();
extern int Socket_accept(int fd);
extern double Ro_Omg[3];
extern int set_nonblock(int fd);

struct Pso_i /* Структура для переменных режима ПСО (построения солнечной ориентации) */
{
    double o_Omg[3]; //угловый скорости
    double S_cck[3]; //направляющие косинусы на солнце в сск
    double u_wPro[3]; /* вектор программной скорости */
    unsigned short o_P[8]; /* Признаки включения двигателей на текущем шаге */
    unsigned short Sn; // признак наличия солнца
    double u_Kp;             //= 1; коэффициент пропорционального звена
    double u_Ki;             //= 0; коэффициент интегрирующего звена
    double u_Kd;             //коэффициент дифференцирующего звена

    double o_wCurrent[3]; /* текущая скорость */
    double i_sigma[3]; /* аргумент управления */
    double n_wStab[3];
    double u_wInitial[3]; /* начальная скорость */
};

struct bcSim_i
{
    double o_Omg[3]; //угловый скорости
    double S_cck[3]; //направляющие косинусы на солнце в сск
    double u_wPro[3]; /* вектор программной скорости */
    unsigned short o_P[8]; /* Признаки включения двигателей на текущем шаге */
    unsigned short Sn; // признак наличия солнца
    double u_Kp;             //= 1; коэффициент пропорционального звена
    double u_Ki;             //= 0; коэффициент интегрирующего звена
    double u_Kd;             //коэффициент дифференцирующего звена
};

extern struct Pso_i pso;

extern void DUS_Main(double o_Omg[3],double o_wCurrent[3]); /* Обработка информации БИУС (блок измерения угловых скоростей) */
extern void DOR_Main(); /* Расчет параметров стабилизации и аргумента управления */
extern void DU_Main(double i_sigma[3],unsigned short o_P[8]); /* Программа управления ДУ (двигательной установкой) */
extern int P_Sun_O(double S[3],double w_pro[3],unsigned short Sn,double omg[3]);
extern int DOR_f_CalcContrArg(double w_stab[3], double sigma[3]);
extern int DOR_f_CalcStabPar(double w_stab[3], double w_pro[3], double w_curr[3]);
extern int Disp_start();
extern int Disp_stop();
extern long mtime();
extern int Socket_init_();
extern int Socket_recv(int fd, void *data, int len);
extern int Socket_send(int fd, const void *data, int len);
extern void* bcmain();
extern int load_bc_serttings();
extern int bc_disp_wait();
extern int print_bc_stat();
extern int print_rt_stat();


int sim_stat;
unsigned int bc_takt;
int rt_takt;
pthread_mutex_t mutex;

extern double omg_old[3];
extern int sun_fail;

struct Model_i
{
    double M_o_Omg[3];
    double o_Omg[3];
    double i_Moment[3];
    double o_Quat[4];
    double S_i[3];
    double S_cck[3];
    unsigned short Sn;

    unsigned short o_P[8];

};

extern struct Model_i model;

extern int rt_disp_start();
extern int rt_disp_stop();
extern int print();
extern int Model_init();
extern int rt_disp_wait();

extern void MIUS_Main(double o_Omg[3],double Omg_out[3]);
extern void Mdl_DOR_Main(double i_Moment[3],double o_Omg[3],double o_Quat[4]);
extern void DUVS_Main(unsigned short o_P[8],double i_Moment[3]);
extern unsigned int Sun_Dat(double H[4],double Si[3],double Scck[3],unsigned short Sn);
extern void* rtmain();

struct sim_time_t {
    unsigned char Hour;
    unsigned char Minute;
    unsigned char Second;
    unsigned short Mls;
};

extern void time_tick(struct sim_time_t* time);
extern struct sim_time_t bc_time;

struct my_time_t {
    unsigned char Hour;
    unsigned char Minute;
    unsigned char Second;
    unsigned int Mls;
} my_time;
