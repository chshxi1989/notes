#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
bool isTimer = false;
uint32_t g_ucsecond = 0;
bool bTImerStart = false;
uint32_t g_loop = 0;
pthread_mutex_t timermutex;

class Timer {
public:
    Timer();
    ~Timer();
    void setTimer(uint32_t ucsecond);
    bool getTimerState();
private:
    pthread_t m_tid;
    pthread_mutex_t m_timermutex;
    void* timerthread(void* args);
}

Timer::Timer()
{
    pthread_mutex_init(&m_timermutex, NULL);
    pthread_create(&m_tid, NULL, timerthread, NULL);
    pthread_detach(m_tid);
}

TImer::~Timer()
{
    pthread_mutex_destroy(&m_timermutex);
}

void Timer::setTimer(uint32_t ucsecond)
{

}

void* threadRed(void* p) {
    uint32_t timecount = 0;
    while(1) {
        pthread_mutex_lock(&timermutex);
        timecount = g_loop;
        pthread_mutex_unlock(&timermutex);
        if(timecount < 60) {
            printf("100\n");  // red ON, green OFF, yellow OFF
        }
   }
}

void* threadGreen(void* p) {
    uint32_t timecount = 0;
    while(1) {
        pthread_mutex_lock(&timermutex);
        timecount = g_loop;
        pthread_mutex_unlock(&timermutex);
        if(timecount >= 60 && timecount < 180) {
            printf("010\n");  // red OFF, green ON, yellow OFF
        }
   }
}

void* threadYellow(void* p) {
    uint32_t timecount = 0;
    while(1) {
        pthread_mutex_lock(&timermutex);
        timecount = g_loop;
        pthread_mutex_unlock(&timermutex);
        if(timecount >= 180 && timecount < 185) {
            if (timecount%2 == 0) {
                printf("001\n"); // red OFF, green OFF, yellow ON
            } else {
                printf("000\n"); // red OFF, green OFF, yellow OFF
            }
        }
   }
}

void* timercounter(void* p) {
    time_t old;
    time_t new;
    while(1) {
        if(bTImerStart) {
            pthread_mutex_lock(&timermutex);
            old = time(NULL);
            while(1) {
                new = time(NULL);
                if(new - old >= g_ucsecond) {
                    bTImerStart = false;
                    isTimer = true;
                    break;
                }
            }
            pthread_mutex_unlock(&timermutex);
        }
    }
}

bool GetTimerStatus() {
    return isTimer;
}

int settimer(uint32_t ucsecond) {
    pthread_mutex_lock(&timermutex);
    isTimer= false;
    bTImerStart = true;
    g_ucsecond = ucsecond;
    pthread_mutex_unlock(&timermutex);
    return 0;
}

int main(int argc, char** args) {
    uint32_t ucsecond = 0;
    uint32_t ucminute = 0;
    uint32_t uchour = 0;
    pthread_t tid;
    pthread_mutex_init(&timermutex,NULL);
    pthread_create(&tid, NULL, timercounter, NULL);
    pthread_detach(tid);
    while(1) {
        printf("%02d:%02d:%02d ", uchour, ucminute, ucsecond);
        settimer(1);
        while(GetTimerStatus() != true);
        ucsecond++;
        if(ucsecond == 60) {
            ucsecond = 0;
            ucminute++;
        }
        if(ucminute == 60) {
            ucminute = 0;
            uchour++;
        }
        if(uchour == 60) {
            uchour =0;
        }
        pthread_mutex_lock(&timermutex);
        g_loop = g_loop%185;
        loop++;
        pthread_mutex_unlock(&timermutex);
   }
   pthread_mutex_destroy(&timermutex);
}
