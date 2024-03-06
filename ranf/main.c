#include <stdio.h>
#include "ranf.h"

int main(void){
    int w=0, d=0, l=0;
    int epoch = 1000000;
    double check = 1.0/3.0;
    for(int i=0; i<epoch; i++){
        real temp = ranf();
        if(temp<check) w++;
        else if(temp>2*check) l++;
        else d++;
    }

    printf("wins: %d\ndraws: %d\nlosses: %d\n\n", w, d, l);
    printf("win rate: %lf\ndraw rate: %lf\nloss rate: %lf\n", (double)w/epoch, (double)d/epoch, (double)l/epoch);
    return 0;
}

// #include <stdio.h>
// #include "ranf.h"

// int main(void){
//     for(int i=0;i<9;i++){
//         printf("%lf\n", ranf());
//     }

//     return 0;
// }