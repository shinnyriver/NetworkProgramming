#include <stdio.h>
#include <math.h>

#define CPU 8086

typedef double real;

#define A 16807L
#define M 2147483647L

static long In[16] = { 0L, 1973272912L, 477177549L, 20464843L, 640830765L, 1098742207L, 78126602L, 84743774L, 831312807L, 124667236L, 1172177002L, 1124933064L, 1223960546L, 1878892440L, 1449793615L, 553303732L };

static int strm = 1;

typedef enum {ARRIVAL, END} event;

real ranf()
{
    short* p, * q, k; long Hi, Lo;
    p = (short*)&In[strm]; Hi = *(p + 1) * A;
    *(p + 1) = 0; Lo = In[strm] * A;
    p = (short*)&Lo; Hi += *(p + 1);
    q = (short*)&Hi;
    *(p + 1) = *q & 0X7FFF;
    k = *(q + 1) << 1;
    if (*q & 0X8000) k++;
    Lo -= M; Lo += k;
    if (Lo < 0) Lo += M;
    In[strm] = Lo;
    return((real)Lo * 4.656612875E-10);
}

int geometric(double p) {
    double k = (log(1 - ranf()) / log(1 - p)) + 1;
    return (int)k;
}

int main(void) {
    double Ta = 0.1, Ts = 0.1;
    int te = 10000000, t1, t2, time, cus, cus2, n;
    double B, X, U, L, W, C, s, tb, tn;
    n = 0; t1 = 0; t2 = te; time = 0, cus = 0;
    B = s = 0; C = 0; tn = time;
    event e;

    while (time < te) {
        if (n == 0 || t1 < t2) e = ARRIVAL;
        else e = END;

        switch(e) {
            case ARRIVAL: /* arrival event */
                time = t1;
                s += n * (time - tn);
                n++;
                tn = time;
                cus++;
                t1 = time + geometric(Ta);
               //  printf("at time %d, customer %d arrive\n", time, cus);
                if (n == 1) {
                    tb = time; // Begin service
                    t2 = time + geometric(Ts);
                    cus2 = cus;
                  //   printf("at time %d, customer %d begin\n", time, cus);
                }
                break;
            
            case END: /* completion event */
                time = t2;
               //  printf("at time %d, customer %d end\n", time, cus2);
                s += n * (time - tn);
                n--;
                tn = time;
                C++;
                if (n > 0) {
                    t2 = time + geometric(Ts);
                    cus2++;
                  //   printf("at time %d, customer %d begin\n", time, cus2);
                } else {
                    B += time - tb; // Update busy time
                    t2 = te; // No more customers
                }
                break;
        }
    }
    if(n>0) {
      B+=time - tb;
      t2 = te;
   }

    X = C / (double)time;
    U = B / (double)time;
    L = s / (double)time;
    W = L / X;

    printf("Throughput = %lf\n", X);
    printf("Utilization = %lf\n", U);
    printf("Mean number in system = %lf\n", L);
    printf("Mean residence time = %lf\n", W);

    return 0;
}
