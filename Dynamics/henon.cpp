#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double a = 1.4;
double b = 0.3;

double x = 0.5;
double y = 0.5;

void henon(FILE *out) {
    double _x = 1 - a*x*x + y;
    double _y = b * x;
    x = _x;
    y = _y;
    if (out) fprintf(out, "%f\n", x);
}

int main(int, char**)
{
    double a = 1.4;
    double b = 0.3;

    double x = 0.5;
    double y = 0.5;

    int ATT = 1000;

    for (int i=0;  i<ATT;  i++) henon(NULL);
    FILE *f = fopen("henon.dat", "w");
    for (i=0;  i<5000;  i++) henon(f);
    fclose(f);
    f = fopen("henon.cont", "w");
    for (i=0;  i<1000;  i++) henon(f);
    fclose(f);
}
