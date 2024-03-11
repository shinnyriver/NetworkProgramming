#include <stdio.h>
#include "ranf.h"

void print_arr(int* arr, int size);

// int main(void){
//     int w=0, d=0, l=0;
//     int epoch = 1000000;
//     double check = 1.0/3.0;
//     for(int i=0; i<epoch; i++){
//         real temp = ranf();
//         if(temp<check) w++;
//         else if(temp>2*check) l++;
//         else d++;
//     }

//     printf("wins: %d\ndraws: %d\nlosses: %d\n\n", w, d, l);
//     printf("win rate: %lf\ndraw rate: %lf\nloss rate: %lf\n", (double)w/epoch, (double)d/epoch, (double)l/epoch);
//     return 0;
// }

// #include <stdio.h>
// #include "ranf.h"

int main(void){
    int lotto[6] = {0};
    int temp[45] = {0};

    // for (int i=0; i<6; i++){
    //     lotto[i] = random(1, 45);
    //     for (int j=0; j<i; j++){
    //         if(lotto[i] == lotto[j]){
    //             i--;
    //             break;
    //         }
    //     }
    // }
    // printf("Lotto numbers\n");
    // print_arr(lotto, 6);

    // printf("1~45\n\n");
    // for(int i=0; i<45; i++){
    //     temp[i] = random(1,45);
    //     for(int j=0; j<i; j++){
    //         if(temp[i] == temp[j]){
    //             i--;
    //             break;
    //         }
    //     }
    // }

    // print_arr(temp, 45);
    for(int i=0; i<10; i++){
    printf("%d\n",geometric(0.1));
    }
    return 0;

    // for (int i=0; i<100; i++){
    //     printf("%d\n", random(1, 45));
    // }
}

void print_arr(int* arr, int size){
    for(int i=0; i<size; i++){
        if(i%10==0&&i!=0) printf("\n");
        printf("%d ", arr[i]);
    }
    printf("\n\n");
}

//biomial random variable
//geometric random variable
//p=0.1