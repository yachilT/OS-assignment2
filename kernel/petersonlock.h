typedef struct petersonlock
{
     // maybe need to be volatile
    int flag[2];
    int turn;
    int active;
    int id;
} petersonlock;




