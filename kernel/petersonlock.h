
typedef struct petersonlock
{
     // maybe need to be volatile
    int flag[2];
    int turn;
    int active;
    int id;
} petersonlock;


void petersonlocksinit(void);
int peterson_create(void);
int peterson_acquire(int lock_id, int role);
int peterson_release(int lock_id, int role);
int peterson_destroy(int lock_id);