#include <stdio.h>
#include <limits.h>
#include <stddef.h>
#include <time.h>

int main() {
    time_t now;

    struct tm some_time;

    now = time(NULL);
    printf("Какое сейчас время %s", ctime(&now));
    printf("Размер структуры  %zu\n", sizeof(time_t));
    printf("Если будем рассматривать как лонг  %ld\n", (long)ctime(&now));

    printf("Local: %s", asctime(localtime(&now)));
    printf("UTC: %s", asctime(gmtime(&now)));

    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    printf("%ld s %ld ns", ts.tv_sec, ts.tv_nsec);

    return 0;
}