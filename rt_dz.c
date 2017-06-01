#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>

#include "defs.h"

#define Fun(N,fun,a...) if(thread_status[N]!=3)\
            {\
            thread_time[N]=mtime();\
            thread_status[N]=1;\
            fun(a);\
            thread_status[N]=2;\
            thread_time[N] = mtime()-thread_time[N];\
            }\
            if(thread_time[N]>thread_time_max[N]) thread_time_max[N]=thread_time[N];

#define Fun_arg(N,fun,b,a...) if(thread_status[N]!=3)\
            {\
            thread_time[N]=mtime();\
            thread_status[N]=1;\
            b=fun(a);\
            thread_status[N]=2;\
            thread_time[N] = mtime()-thread_time[N];\
            }\
            if(thread_time[N]>thread_time_max[N]) thread_time_max[N]=thread_time[N];

int r;
pthread_t t;
int thread_status[32];
int thread_stop[32];
int thread_time[32];
int thread_time_max[32];

int print_rt_stat() {
    int i;
    for(i = 0; i < 5; i++) {
        printf("rt_thread_status [%d] = %d",i,thread_status[i]);
        printf("\033[1E");
    }
    return 0;
}

void* rt_thread(){
    thread_time[0] = mtime();
    thread_status[0]=1;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    pthread_mutex_lock(&mutex);
    Fun(1,DUVS_Main,model.o_P,model.i_Moment); //модель ДУ
//pthread_mutex_unlock(&mutex);
    Fun(2,Mdl_DOR_Main,model.i_Moment,model.M_o_Omg,model.o_Quat); //Модель внешней среды
//pthread_mutex_lock(&mutex);
    Fun(3,MIUS_Main,model.M_o_Omg,model.o_Omg); //модель БИУС
    Fun_arg(4,Sun_Dat,model.Sn,model.o_Quat,model.S_i,model.S_cck,model.Sn);
    pthread_mutex_unlock(&mutex);
    thread_status[0]=2;
    thread_time[0] = mtime()-thread_time[0];
    if(thread_time[0]>thread_time_max[0])
    thread_time_max[0]=thread_time[0];
    return 0;
}

int rt_disp_start(){
    int j;
    for (j=1;j<6;j++){
                if (thread_status[j]==1)
                    thread_stop[j]++;
                else
                    thread_stop[j]=0;
                if (thread_stop[j]>2){
                    thread_status[j]=3;
                    printf("\nthread_status[%d] = %d\n",j,thread_status[j]);
                }
            }
    r=pthread_create(&t,NULL,(void *(*)(void *))rt_thread,NULL);
    if (r!=0){
            perror("pthread_create");
    }
    return 0;
}

int rt_disp_wait() {
    r=pthread_join(t,NULL);
    if (r!=0){
        perror("pthread_join");
    }
    return 0;
}

int rt_disp_stop(){
    if (thread_status[0]==1){
        r=pthread_cancel(t);
            if (r!=0){
                perror("pthread_cancel");
            }
            r=pthread_join(t,NULL);
            if (r!=0){
                perror("pthread_join");
            }
            printf("thread Stop Erorr\n");
        }else {
        if (thread_status[0]!=0){
            r=pthread_join(t,NULL);
                if (r!=0){
                    perror("pthread_join");
                }
        }
        }
    return 0;
}

int load_serttings() // загружаем параметры из файла настроек settings.ini
{
    int k,i,l;
    unsigned int j;
    FILE* f;
    char str[2][100];
    char* read[10];
    read[0] = "model.M_o_Omg[0]";
    read[1] = "model.M_o_Omg[1]";
    read[2] = "model.M_o_Omg[2]";

    f = fopen("c:/settings.ini","r+");

    while(fgets(str[0],sizeof(str[0]),f))
        {
            for(i=0;i<3;i++)
                if(strstr(str[0],read[i])!=0)
                {
                    //printf("%s",str);
                    for(j = 0;j <= strlen(str[0]);j++)
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
                    printf("str[1] = %f\n",atof(str[1]));
                    switch(i)
                    {
                        case 0: model.M_o_Omg[0] = atof(str[1])*3.14/180;
                        case 1: model.M_o_Omg[1] = atof(str[1])*3.14/180;
                        case 2: model.M_o_Omg[2] = atof(str[1])*3.14/180;

                    }
                    memset(str[1],0,100);
                }
        }
    for (i=0;i<3;i++)
    {
        model.i_Moment[i]=0;
        model.o_Omg[i] = 0;
    }
    fclose(f);
    return 0;
}
int Model_init() // инициализация
{
    load_serttings();
    return 0;
}
