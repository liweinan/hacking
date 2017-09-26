#include <stdio.h>
#include <time.h>

int main() {
    long int sec_since_epoch;
    struct tm curr_time, *time_ptr;
    int hour, minute, second, day, month, year;

    sec_since_epoch = time(0);
    printf("time() - seconds since epoch: %ld\n", sec_since_epoch);

    time_ptr = &curr_time;

    localtime_r(&sec_since_epoch, time_ptr);

    hour = curr_time.tm_hour;
    minute = time_ptr->tm_min;
    second = *((int *) time_ptr);

    printf("Current time is: %02d:%02d:%02d\n", hour, minute, second);
}
