#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"        // <-- this one contains the declaration of yield()
#include "defs.h"

#include "petersonlock.h"



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
            __sync_lock_test_and_set(&lk->active, 1);
            __sync_lock_test_and_set(&lk->id, identifier++);
            __sync_synchronize();
            return lk->id;
        }
    }
    return -1;
}

int 
peterson_acquire(int lock_id, int role){
    petersonlock * lk;
    for (lk = locks; lk < &locks[NLOCKS]; lk++)
    {
        __sync_synchronize();
        if(lk->id == lock_id){
            lk->flag[role] = 1;
            lk->turn = role;
            __sync_synchronize();
            while(lk->flag[1-role] && lk->turn == role) {yield(); __sync_synchronize();}    
            return 0;        
        }
    }    
    return -1;
}

int 
peterson_release(int lock_id, int role){
    petersonlock * lk;
    for (lk = locks; lk < &locks[NLOCKS]; lk++)
    {
        if(lk->id == lock_id){
            lk->flag[role] = 0;
            __sync_synchronize();
            return 0;             
        }
    }
    return -1;
}


int
peterson_destroy(int lock_id) {
    petersonlock *lk;
    
    for (lk = locks; lk < &locks[NLOCKS]; lk++) {
        if (lk->id == lock_id) {
            __sync_lock_test_and_set(&lk->id, -1);
            __sync_lock_release(&lk->active);
            __sync_synchronize();
            return 0;
        }
    }
    return -1;
}