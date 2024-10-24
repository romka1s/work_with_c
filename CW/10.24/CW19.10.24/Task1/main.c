#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handler(int s){
    printf("Signal ID %d\n", s);
}
void handler_sig_term(int s){
    printf("Signal_TERM %d\n", s);
    exit(0);
}
int main(){
    // signal(SIGINT, handler); - catch Cntrl-C
    signal(SIGTERM, handler_sig_term);
    while (1) {
        printf("Running... Press Ctrl-C to trigger\n");
        sleep(1);
    }
    return 0;
}