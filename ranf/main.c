#include <stdio.h>
#include "ranf.h"

void print_arr(int* arr, int size);
void OneToSize(int size);
void print_geometric(double p);
void print_uniform(double a, double b);
void print_exponent(double lamda);

int main(void){
    // OneToSize(45);
    // print_geometric(0.1);
    // print_uniform(0.9, 1.5);
    print_exponent(0.1);

    return 0;    
}

void print_arr(int* arr, int size){
    for(int i=0; i<size; i++){
        if(i%10==0&&i!=0) printf("\n");
        printf("%d ", arr[i]);
    }
    printf("\n\n");
}

void OneToSize(int size){
    int temp[45] = {0};
    for(int i=0; i<size; i++){
        temp[i] = random(1,size);
        for(int j=0; j<i; j++){
            if(temp[i] == temp[j]){
                i--;
                break;
            }
        }
    }
    print_arr(temp, size);
}

void print_geometric(double p){
    for(int i=0; i<10; i++){
        printf("%d\n",geometric(p));
    }
    printf("\n");
}

void print_uniform(double a, double b){
    for(int i=0; i<10; i++){
        printf("%lf\n", uniform(a, b));
    }
    printf("\n");
}

void print_exponent(double lamda){
    for(int i=0; i<10; i++){
        printf("%lf\n", exponent(lamda));
    }
    printf("\n");
}

//biomial random variable
//geometric random variable
//p=0.1