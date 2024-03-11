#include <math.h>

#define CPU 8086
#define A 16807L
#define M 2147483647L

typedef double real;
static int strm=1;

static long In[16] = {0L, 1973272912L, 747177549L, 20464843L, 640830765L, 1098742207L, 78126602L, 84743774L, 831312807L, 124667236L, 1172177002L, 1124933064L, 1223960546L, 1878892440L, 1449793615L, 553303732L};

real ranf(void){
    short *p, *q, k;
    long Hi, Lo;
    p = (short *)&In[strm];
    Hi=*(p+1)*A;
    *(p+1)=0;

    Lo=In[strm]*A;
    p=(short *)&Lo;
    Hi+=*(p+1);
    q=(short *)&Hi;

    *(p+1)=*q&0X7FFF;
    k=*(q+1)<<1;
    if(*q&0X8000) k++;
    Lo-=M;
    Lo+=k;
    if(Lo<0) Lo+=M;

    In[strm]=Lo;
    return((real)Lo*4.656612875E-10);
}

int random(int a, int b){
    return (int)(a+(b-a+1)*ranf());
}

int geometric(real p){
    return (int)(log(1-ranf())/log(1-p))+1;
}

// int stream(int n){
//     if(n<0 || n>15){
//         return -1;
//     }
//     strm=n;
//     return strm;
// }

// long seed(long Ik, int n){
//     if(n<0 || n>15){
//         return -1;
//     }
//     In[n]=Ik;
//     return In[n];
// }

// real uniform(real a, real b){
//     if(a>b) return -1;
//     return a+(b-a)*ranf();
// }