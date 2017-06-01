#include "defs.h"
#include <stdio.h>
#include <string.h>
#if defined(WIN32)
    #include <windows.h>
#elif __linux__
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <netinet/in.h>

#endif

pthread_t t_sM;
struct sim_time_t rt_time;
int fd;

void* rtsock () {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); // разрешаем отмену потока
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    int fd = TCP_Server_init(9000,"127.0.0.1");


    while (sim_stat != 0) {
        int s = Socket_accept(fd);
        //set_nonblock(s);
        while (1) {
            char buf[2];
            memset(&buf,'0',sizeof(buf));
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
            if (buf[0] == 'M') {
                char b[250];
                memset(&b,'0',sizeof(b));
                pthread_mutex_lock(&mutex);
                int len = sizeof(model);
                memcpy(&b,&model,sizeof(model));
                pthread_mutex_unlock(&mutex);
                r = Socket_send(s,b,len);
                if (r == -1) {
                    close(s);
                    break;
                }
                memset(&b,'0',sizeof(b));
                pthread_mutex_lock(&mutex);
                len = sizeof(rt_time);
                memcpy(&b,&rt_time,sizeof(rt_time));
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

void* rtmain() {
    int i=0;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); // разрешаем отмену потока
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    pthread_create(&t_sM, NULL, (void *(*)(void *))rtsock, NULL);



    while(sim_stat != 0 ) {
        rt_disp_start();
        #if defined(WIN32)
            Sleep(100);
        #elif __linux__
            usleep(100000);
        #endif

        rt_disp_wait();
        pthread_mutex_lock(&mutex);
        if (sim_stat == 0) {
            break;
        }
        if (rt_takt++ >= 1000) {
            rt_takt = 0;
        }

        for(i = 0; i < 8; i++)  {
            model.o_P[i] = pso.o_P[i];
        }
        time_tick(&rt_time);
        pthread_mutex_unlock(&mutex);


    }
    close(fd);
    pthread_cancel(t_sM);
    return 0;
}
