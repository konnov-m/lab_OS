#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <stdbool.h>


#define SECS_IN_DAY 86400
#define SECS_IN_HOUR 3600
#define SECS_IN_MINUTE 60
#define UTC_MSK 3

int* time_to_hh_mm_ss (uint32_t time) {
    uint32_t current_day_hh = time % SECS_IN_DAY;
    int* time_hh_mm_ss = malloc(3*sizeof(int));
    time_hh_mm_ss[0] = (current_day_hh / SECS_IN_HOUR);
    time_hh_mm_ss[1] = (current_day_hh - time_hh_mm_ss[0]*SECS_IN_HOUR) / SECS_IN_MINUTE;
    time_hh_mm_ss[2] = ((current_day_hh - time_hh_mm_ss[0]*SECS_IN_HOUR) - time_hh_mm_ss[1]*SECS_IN_MINUTE);
    time_hh_mm_ss[0] += UTC_MSK;
    return time_hh_mm_ss;
}

int main(int argc, char *argv[])
{   
    printf("Start program\n");
    fflush(stdout);

    const char *name_channel_sound = "../sound_data";
    const char *name_channel_light = "../light_data";
    const char *exit_string = "Exit";
    int fd_sound, fd_light;
    int count_light = 1;
    int count_sound = 1;
    char buffer[64];
    uint32_t time_light;
    uint32_t time_sound;
    bool is_light_detected = false;


    fd_sound = open(name_channel_sound, O_RDONLY);
    fd_light = open(name_channel_light, O_RDONLY);

    if (fd_light == -1 || fd_sound == -1) {
        printf("Can't open channel\n");
        exit(-1);
    } else {
        printf("Channel oppend\n");
    } 
    fflush(stdout);

    while (1) {
        if(!is_light_detected) {
            int n = read(fd_light, buffer, sizeof(buffer));
            if(memcmp(buffer, exit_string, 1) == 0) {
                printf("Завершение программы 1");
                fflush(stdout);
                exit(0);
            } 
            time_light = atoi(buffer);
            int *h_m_s = time_to_hh_mm_ss(time_light);
            printf("Время измерения вспышки № %d: %d:%d:%d\n", count_light++, h_m_s[0], h_m_s[1], h_m_s[2]);
            fflush(stdout);
            is_light_detected = true;
        } else {
            int n = read(fd_sound, buffer, sizeof(buffer));
            if(memcmp(buffer, exit_string, 1)==0) {
                printf("Завершение программы\n");
                fflush(stdout);
                exit(0);
            }
            time_sound = atoi(buffer);
            int *h_m_s = time_to_hh_mm_ss(time_sound);
            printf("Время измерения хлопка № %d: %d:%d:%d\n", count_sound, h_m_s[0], h_m_s[1], h_m_s[2]);
            is_light_detected = false;

            int meters = (time_sound-time_light)*300;
            printf("Расстояние до грозы № %d: %d\n", count_sound++, meters);
            fflush(stdout);
        }
        
    }
}
