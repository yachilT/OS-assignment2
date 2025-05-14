#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("incorrect usage! torunament <number of proccesses>\n");
        exit(-1);
    }
    int n = atoi(argv[1]);
    int tournamentID = tournament_create(n);

    if (tournamentID == -1){
        printf("Failed to create tournament.\n");
        exit(-1);
    }
    
    tournament_acquire();
    printf("Process PID: %d, Process tournamentID: %d\n", getpid(), tournamentID);
    tournament_release();

    if (tournamentID == n-1) {
        while(wait(0) != -1);
    }
    exit(0);

}