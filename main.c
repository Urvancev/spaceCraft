#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#if defined(WIN32)
    #include <windows.h>
#elif __linux__
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif

#include "math.h"

#include <defs.h>

double Ro_Omg[3];

struct Pso_i pso;
struct Model_i model;
pthread_t t,t1;

int time_init() {
    my_time.Hour = 0;
    my_time.Minute = 0;
    my_time.Second = 0;
    return 0;
}

void clearStr() {
    printf("                 "
           "                 "
           "                 "
           "                 "
           "                 "
           "                 "
           "                 "
           "                 ");
}

int time_tik () {
    if (my_time.Mls/1000000 > 0) {
        my_time.Second = my_time.Second + my_time.Mls/1000000;
        my_time.Mls = my_time.Mls - (my_time.Mls/1000000)*1000000;
    }
    if (my_time.Second >= 60) {
        my_time.Minute++;
        my_time.Second = my_time.Second - 60;
    }
    if (my_time.Minute >= 60) {
        my_time.Hour++;
        my_time.Minute = my_time.Minute - 60;
    }
    return 0;
}

int time_print() {
    printf("I already work: %02d:%02d:%02d",my_time.Hour,my_time.Minute,my_time.Second);
    clearStr();
    printf("\033[1E");
    return 0;
}
int fd,s;

void stop() {
    pthread_mutex_lock(&mutex);
    sim_stat = 0;
    pthread_mutex_unlock(&mutex);
    int r = 0;
    r=pthread_cancel(t); // отменяем поток
    if (r!=0){
        perror("pthread_cancel");
    }
    r=pthread_join(t,NULL); //ждем завершения
    if (r!=0){
        perror("pthread_join");
    }
    r=pthread_cancel(t1); // отменяем поток
    if (r!=0){
        perror("pthread_cancel");
    }
    r=pthread_join(t1,NULL); //ждем завершения
    if (r!=0){
        perror("pthread_join");
    }

    printf("thread Stop Erorr\n");
    printf("program stop");
    close(fd);
    close(s);

}

int main() {
    signal(SIGINT,stop);
    signal(SIGHUP,stop);
    signal(SIGTERM,stop);
    signal(SIGPIPE,stop);
    sim_stat = 1;
    #if defined(WIN32)
        Sleep(1000);
    #elif __linux__
        usleep(1000);
    #endif

    time_init();

    pso.u_wInitial[0] = 0*M_PI/180;
    pso.u_wInitial[1] = 0;
    pso.u_wInitial[2] = 0;

    pso.u_wPro[0] = 0*M_PI/180;
    pso.u_wPro[1] = 0*M_PI/180;
    pso.u_wPro[2] = 0*M_PI/180;

    pso.u_Kp = 1;
    pso.u_Ki = 0;
    pso.u_Kd = 0;

    model.M_o_Omg[0] = 0*M_PI/180;
    model.M_o_Omg[1] = 0*M_PI/180;
    model.M_o_Omg[2] = 0*M_PI/180;

    model.S_i[0] = 1;
    model.S_i[1] = 0;
    model.S_i[2] = 0;

    double Mod;
    Mod = sqrt(model.S_i[0] * model.S_i[0] + model.S_i[1] * model.S_i[1] + model.S_i[2] * model.S_i[2]);

    model.S_i[0] = model.S_i[0]/Mod;
    model.S_i[1] = model.S_i[1]/Mod;
    model.S_i[2] = model.S_i[2]/Mod;

    model.S_cck[0] = 0;
    model.S_cck[1] = 0;
    model.S_cck[2] = 0;

    model.o_Quat[0] = 0.7071;
    model.o_Quat[1] = 0;
    model.o_Quat[2] = 0;
    model.o_Quat[3] = 0.7071;

    printf("start\n");
  //  fd = TCP_Server_init(9012,"127.0.0.1");

   // int r;
    //s = Socket_accept(fd);
    //char b[100];
  //  r = recv(s,b,sizeof(b),0);
   // if (r == -1) {
    //    perror("recv");
    //}
   /* char b1[2];
    memset(&b1,'\0',sizeof(b));
    b1[0] = 'h';
    b1[1] = 'i';
    r = send(s,&b1,2,MSG_NOSIGNAL);
    if (r == -1) {
        perror("send");
    }
   */ usleep(2000000);
    #if defined(WIN32)
        Sleep(1000);
    #elif __linux__
        usleep(1000);
    #endif
    pthread_mutex_init(&mutex, NULL);
    #if defined(WIN32)
        Sleep(1000);
    #elif __linux__
        usleep(1000);
    #endif
    pthread_create(&t, NULL, (void *(*)(void *))rtmain, NULL);
    pthread_create(&t1, NULL, (void *(*)(void *))bcmain, NULL);

  //  int i;
 //   for(i = 0; i < 400000; i++) {
    while(sim_stat != 0)  {
        long t;
        t = mtime();
    #if defined(WIN32)
        Sleep(200);
        system("cls");
    #elif __linux__
        usleep(150000);
        system("clear");
    #endif
        pthread_mutex_lock(&mutex);
       /* for(i = 0; i < 8; i++)  {
            model.o_P[i] = pso.o_P[i];
        }
        pso.Sn = model.Sn;
        for(i = 0; i < 3; i++) {
            pso.o_Omg[i] = model.o_Omg[i];
            pso.S_cck[i] = model.S_cck[i];
        }*/
        char buf[250];
        memset(&buf,'\0',sizeof(buf));
        memcpy(&buf,&pso,sizeof(pso));
      //  Socket_send(s, buf,sizeof(buf));
        int k;
        printf("bc_takt = %4d    rt_takt = %4d     Sn = %1d      sun_fail = %4d",bc_takt,rt_takt,pso.Sn,sun_fail);
        printf("\033[1E");
        printf("угловая скорость      Вектор солнца           програмная скорость       ");
        printf("\033[1E");
        printf("o_Omg[0] = %7.3f     S_cck[0] = %7.3f    w_pro[0] = %7.3f         ",
               model.o_Omg[0]*180/M_PI,model.S_cck[0],pso.u_wPro[0]*180/M_PI);
        printf("\033[1E");
        printf("o_Omg[1] = %7.3f     S_cck[1] = %7.3f    w_pro[1] = %7.3f         ",
               model.o_Omg[1]*180/M_PI,model.S_cck[1],pso.u_wPro[1]*180/M_PI);
        printf("\033[1E");
        printf("o_Omg[2] = %7.3f     S_cck[2] = %7.3f    w_pro[2] = %7.3f         ",
               model.o_Omg[2]*180/M_PI,model.S_cck[2],pso.u_wPro[2]*180/M_PI);
        printf("\033[1E");
        for (k = 0; k < 3; k++) {
        printf("omg_old[%d] = %7.3f    model.m_omg[%d] = %7.3f\n",k,omg_old[k]*180/M_PI,k,model.M_o_Omg[k]*180/M_PI);
        }
        printf("Моменты по осям                     Кватернионы");
        printf("\033[1E");
        printf("model.i_Moment[0] = %7.3f     model.o_Quat[0] = %7.3f",model.i_Moment[0],model.o_Quat[0]);
        printf("\033[1E");
        printf("model.i_Moment[1] = %7.3f     model.o_Quat[1] = %7.3f",model.i_Moment[1],model.o_Quat[1]);
        printf("\033[1E");
        printf("model.i_Moment[2] = %7.3f     model.o_Quat[2] = %7.3f",model.i_Moment[2],model.o_Quat[2]);
        printf("\033[1E");
        printf("                                model.o_Quat[3] = %7.3f",model.o_Quat[3]);
        printf("\033[1E");
        printf("\033[1E");
        printf("состояние двигателей:");
        printf("\033[1E");
        for(k = 0; k < 8; k++) {
            printf("o_P[%d] = %d",k,model.o_P[k]);
            printf("\033[1E");
        }
        printf("статус систем Борта");
        printf("\033[1E");
        print_bc_stat();
        printf("статус систем модели");
        printf("\033[1E");
        print_rt_stat();
        pthread_mutex_unlock(&mutex);
        my_time.Mls = my_time.Mls + (mtime() - t);
        time_tik ();
        printf("\033[1E");
        time_print();
        printf("\033[1E");
        fflush(stdout);
    }
  //  close(fd);
   // close(s);
    pthread_mutex_destroy(&mutex);
    return 0;
}

