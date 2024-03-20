#include <stdio.h>
#include <math.h>

#define CPU 8086

typedef double real;
#define then

#define A 16807L
#define M 2147483647L

static long In[16] = { 0L, 1973272912L, 477177549L, 20464843L, 640830765L, 1098742207L, 78126602L, 84743774L, 831312807L, 124667236L, 1172177002L, 1124933064L, 1223960546L, 1878892440L, 1449793615L, 553303732L };

static int strm = 1;

typedef enum {ARRIVAL, BEGIN, END} Event;

real ranf()
{
   short* p, * q, k; long Hi, Lo;
   p = (short*)&In[strm]; Hi = *(p + 1) * A;
   *(p + 1) = 0; Lo = In[strm] * A;
   p = (short*)&Lo; Hi += *(p + 1);
   q = (short*)&Hi;
   *(p + 1) = *q & 0X7FFF;
   k = *(q + 1) << 1;
   if (*q & 0X8000)
      k++;
   Lo -= M; Lo += k;
   if (Lo < 0)
      Lo += M;
   In[strm] = Lo;
   return((real)Lo * 4.656612875E-10);
}

int geometric(double p) {
   double k = (log(1 - ranf()) / log(1 - p)) + 1;
   return (int)k;
}

int main(void) {
   double Ta = 0.1, Ts = 0.1;
   int te = 10000000, t1, t2, time, cus,cus2, n;
   double B,X,U,L,W,C,s,tb,tn;
   n = 0; t1 = 0; t2 = te; time = 0, cus = 0;
   B = s = 0; C = 0; tn = time;

   Event event;

   while (time < te)
   {
      switch (event)
      {
        if(t1 < t2) { /* event 1: arrival */
            event = ARRIVAL;
        }
         case ARRIVAL:
            time = t1; 
            s += n * (time - tn);
            n++; 
            tn = time;
            cus++;
            printf("at time %d, customer %d arrive\n", time, cus);
            t1 = time + geometric(Ta);
            if (n == 1) {
               tb = time;
               t2 = time + geometric(Ts);
               cus2 = cus;
            }
            break;
         case BEGIN:
            time = t1; 
            s += n * (time - tn);
            n++; 
            tn = time;
            cus++;
            printf("at time %d, customer %d begin\n", time, cus);
            t1 = time + geometric(Ta);
            break;
         case END:
            time = t2;
            printf("at time %d, customer %d end\n", time, cus2);
            s += n * (time - tn);
            n--;
            tn = time;
            C++;
            if (n > 0) {   //end -> begin
               cus2++;
               t2 = time + geometric(Ts);
               event=BEGIN;
            }
            else
            {
               t2 = te;
               B += time - tb;
            }
            break;
      }
   }

   X = C / time;
   printf("throughput = %lf\n", X);
   U = B / time;
   printf("utilization = %lf\n", 1-U);
   L = (s / time);
   printf("mean no.in system = %lf\n", L);
   W = L / X;
   printf("mean residence time = %lf\n", W);

   return 0;
}
