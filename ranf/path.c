#include <stdio.h>
#include "ranf.h"

int main(void){
    int cnt = 0, te = 100000, tn = 0;
    int total=0;
    int now = 1;
    double tmp;
    while(tn < te){
        switch(now){
            case 1:
                cnt++;
                now = 2;
                break;
            
            case 2:
                cnt++;
                tmp = ranf();
                if(tmp <= 0.2) now=3;
                else now=5;
                break;

            case 3:
                cnt++;
                now = 4;
                break;
            
            case 4:
                cnt++;
                tmp = ranf();
                if(tmp <= 0.25) now = 3;
                else now =7;
                break;
            
            case 5:
                cnt++;
                tmp = ranf();
                if(tmp<=0.55) now=7;
                else now = 6;
                break;

            case 6:
                cnt++;
                now = 7;

            case 7:
                cnt++;
                now=8;
                break;
            
            case 8:
                cnt++;
                tmp = ranf();
                if(tmp<=0.3) now=1;
                else{
                    total += cnt;
                    cnt = 0;
                    tn++;
                    now = 1;
                }


        }
    }
    printf("Mean : %lf\n", (double)(total)/(double)(te));
    return 0;
}