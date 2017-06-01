#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include <time.h>

#define FunNumbers 5 //колличество функций диспетчера

#define Fun(N,fun,a...) if(thread_status[N]!=3)\
            {\
            thread_time[N]=mtime();\
            thread_status[N]=1;\
            fun(a);\
            thread_status[N]=2;\
            thread_time[N] = mtime()-thread_time[N];\
            }\
            if(thread_time[N]>thread_time_max[N]) thread_time_max[N]=thread_time[N];


int r;
pthread_t t_bc; //поток
int thread_status[FunNumbers+1]; // массив статуса функций и потока
int thread_stop[FunNumbers+1];
int thread_time[FunNumbers+1]; // время функции
int thread_time_max[FunNumbers+1]; //максимальное время функции

int print_bc_stat() {
    int i;
    for(i = 0; i < FunNumbers+1; i++) {
        printf("bc_thread_status [%d] = %d",i,thread_status[i]);
        printf("\033[1E");
    }
    return 0;
}

long mtime() // функция счета времени, возвращает время в микросекундах
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME,&t);
    return (long)t.tv_sec*1000000 + t.tv_nsec/1000;
}

void* bc_thread(){ // поток диспетчера
    thread_time[0] = mtime();// засекаем время потока
    thread_status[0]=1; // статус - поток начался
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); // разрешаем отмену потока
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
pthread_mutex_lock(&mutex);
    Fun(1,DUS_Main,pso.o_Omg,pso.o_wCurrent);				// функция обработки угловых скоростей
    Fun(2,DOR_f_CalcStabPar,pso.n_wStab, pso.u_wPro, pso.o_wCurrent);	// Функция расчета параметров стабилизации
    Fun(3,DOR_f_CalcContrArg,pso.n_wStab, pso.i_sigma);			// Функция расчета аргумента управления
    Fun(4,DU_Main,pso.i_sigma,pso.o_P);					// функция управления двигателями
    Fun(5,P_Sun_O,pso.S_cck,pso.u_wPro,pso.Sn,pso.o_Omg);
pthread_mutex_unlock(&mutex);
    thread_status[0]=2; // статус - поток завершился
    thread_time[0] = mtime()-thread_time[0]; // считываем время выполнения потока
    if(thread_time[0]>thread_time_max[0]) thread_time_max[0]=thread_time[0]; // ищем максимальное время выполнения
    return 0;
}

int Disp_start(){ //запус потока диспетчера
    int j;
    for (j=1;j<6;j++){ //проверка статуса функций и потока
                if (thread_status[j]==1)
                    thread_stop[j]++; //изменение флага отмены функции если статус некорректный
                else
                    thread_stop[j]=0; // обнуление флага если все в порядке
                if (thread_stop[j]>2){ // если 3 ошибки подряд отмена функции
                    thread_status[j]=3;
                    printf("\nthread_status[%d] = %d\n",j,thread_status[j]);
                }
            }
    r=pthread_create(&t_bc,NULL,(void *(*)(void *))bc_thread,NULL); // создаем поток
    if (r!=0){
            perror("pthread_create");
    }
    return 0;
}

int bc_disp_wait() {
    r=pthread_join(t_bc,NULL); //ждем завершения
    if (r!=0){
        perror("pthread_join");
    }
    return 0;
}

int Disp_stop(){ //проверка завершения потока
    if (thread_status[0]==1){ // если поток не завершен
        r=pthread_cancel(t_bc); // отменяем поток
            if (r!=0){
                perror("pthread_cancel");
            }
            r=pthread_join(t_bc,NULL); //ждем завершения
            if (r!=0){
                perror("pthread_join");
            }
            printf("thread Stop Erorr\n");
        }else {
            r=pthread_join(t_bc,NULL);
            if (r!=0){
                perror("pthread_join");
            }
        }
    return 0;
}

int load_bc_serttings() // загружаем параметры из файла настроек settings.ini
{
    int k,i,l;
    unsigned int j;
    FILE* f;
    char str[2][100];
    char* read[10];
    read[0] = "pso.u_wInitial[0]";
    read[1] = "pso.u_wInitial[1]";
    read[2] = "pso.u_wInitial[2]";
    read[3] = "pso.u_wPro[0]";
    read[4] = "pso.u_wPro[1]";
    read[5] = "pso.u_wPro[2]";
    read[6] = "pso.u_Kp";
    read[7] = "pso.u_Ki";
    read[8] = "pso.u_Kd";

    f = fopen("C:/bcsettings.ini","r+");

    while(fgets(str[0],sizeof(str[0]),f))
        {
            for(i=0;i<9;i++)
                if(strstr(str[0],read[i])!=0)
                {
                    //printf("%s",str);
                    for(j=0;j<=strlen(str[0]);j++)
                        if (str[0][j]=='=')
                            k=j+1;
                    l=0;
                    //printf("k = %d \n",k);
                    while(str[0][k]!=';'){
                        if(str[0][k]!= ' ')
                        {
                            str[1][l]=str[0][k];
                            l++;
                        }
                        k++;
                    }
                    //printf("str[1] = %f\n",atof(str[1]));
                    switch(i)
                    {
                        case 0: pso.u_wInitial[0] = atof(str[1]);
                        case 1: pso.u_wInitial[1] = atof(str[1]);
                        case 2: pso.u_wInitial[2] = atof(str[1]);
                        case 3: pso.u_wPro[0] = atof(str[1])*3.14/180;
                        case 4: pso.u_wPro[1] = atof(str[1])*3.14/180;
                        case 5: pso.u_wPro[2] = atof(str[1])*3.14/180;
                        case 6: pso.u_Kp = atof(str[1]);
                        case 7: pso.u_Ki = atof(str[1]);
                        case 8: pso.u_Kd = atof(str[1]);
                    }
                    memset(str[1],0,100);
                }
        }
    fclose(f);
    return 0;
}
