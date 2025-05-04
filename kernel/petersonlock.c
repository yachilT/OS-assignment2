#include "petersonlock.h"
#include "defs.h"

#define NLOCKS 15
petersonlock locks[NLOCKS];
int identifier = 0;

void
petersonlocksinit(void) {
    petersonlock * lk;
    for (lk = locks; lk < &locks[NLOCKS]; lk++) {
        lk->active = 0;
        lk->flag[0] = 0;
        lk->flag[1] = 0;
        lk->turn = 0;
        lk->id = -1;
    }

}

int
peterson_create(void)
{
    petersonlock * lk;
    for (lk = locks; lk < &locks[NLOCKS]; lk++)
    {
        if(!lk->active){
            lk->active = 1;
            lk->id = identifier++;
        }
    }
    return 0;
}