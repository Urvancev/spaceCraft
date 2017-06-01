#include "defs.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#if defined(WIN32)
    #include <windows.h>
#elif __linux__
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <netinet/in.h>
#endif


pthread_t t_s;
struct sim_time_t bc_time;
int fd;

void* bcsock () {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); // разрешаем отмену потока
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    int fd = TCP_Server_init(9010,"127.0.0.1");



    while (sim_stat != 0) {
        int s = Socket_accept(fd);
        //set_nonblock(s);
        while (1) {
            char buf[2];
            memset(&buf,'\0',sizeof(buf));
            int r;
            #if defined(WIN32)
                Sleep(50);
            #elif __linux__
                usleep(50000);
            #endif
            r = Socket_recv(s,buf,1);
            if (r == -1) {
                close(s);
                break;
            }
            if (buf[0] == 'B') {
                char b[250];
                memset(&b,'\0',sizeof(b));
                pthread_mutex_lock(&mutex);
                int len = sizeof(pso);
                printf("pso size = %d \n",len);
                memcpy(&b,&pso,sizeof(pso));
                pthread_mutex_unlock(&mutex);
                r = Socket_send(s,b,len);
                if (r == -1) {
                    close(s);
                    break;
                }
                memset(&b,'\0',sizeof(b));
                pthread_mutex_lock(&mutex);
                len = sizeof(bc_time);
                memcpy(&b,&bc_time,sizeof(bc_time));
                pthread_mutex_unlock(&mutex);
                r = Socket_send(s,b,len);
                if (r == -1) {
                    close(s);
                    break;
                }
            }
            //close(s);
        }
    }
    close(fd);
    return 0;
}

void time_tick(struct sim_time_t *time) {
    time->Mls += 100;
    if (time->Mls >= 1000) {
        time->Second += 1;
        time->Mls = 0;
        if (time->Second >= 60) {
            time->Second = 0;
            time->Minute += 1;
            if (time->Minute >= 60) {
                time->Minute = 0;
                time->Hour += 1;
            }
        }
    }
}

void* bcmain() {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); // разрешаем отмену потока
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    pthread_create(&t_s, NULL, (void *(*)(void *))bcsock, NULL);

    memset(&bc_time,0,sizeof(bc_time));

    while(sim_stat != 0 ) {
        Disp_start();
        #if defined(WIN32)
            Sleep(100);
        #elif __linux__
            usleep(100000);
        #endif

        bc_disp_wait();
        pthread_mutex_lock(&mutex);

        if (sim_stat == 0) {
            break;
        }
        if (bc_takt++ >= 1000) {
            bc_takt = 0;
        }
      //  if (bc_takt >= 900) break;
        pso.Sn = model.Sn;
        int i;
        for(i = 0; i < 3; i++) {
            pso.o_Omg[i] = model.o_Omg[i];
            pso.S_cck[i] = model.S_cck[i];
        }
        //bc_time.Mls+=100;
        time_tick(&bc_time);
        printf("mls = %d\n",bc_time.Mls);
        pthread_mutex_unlock(&mutex);


    }
    close(fd);
    pthread_cancel(t_s);
    return 0;
}
