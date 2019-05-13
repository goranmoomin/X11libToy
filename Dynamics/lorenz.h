#ifndef _LORENZ_H_
#define _LORENZ_H_

#include <NRC/rk4.h>

// const double lorenz_sigma = 10.;
// const double lorenz_b = 8./3.;
// const double lorenz_r = 28.;

class Lorenz: public RK4<double> {
  public:
    double Sigma, B, R;

    Lorenz() : RK4<double>(3) {
        Set(10., 8. / 3., 28.);
        Init();
    }
    Lorenz(double sigma, double b, double r) : RK4<double>(3) {
        Set(sigma, b, r);
        Init();
    }
    void Init(int seed = 0) {
        switch(seed) {
        case 0: y[1] = y[2] = y[3] = 1.; break;
        }
    }
    double X() { return y[1]; }
    double Y() { return y[2]; }
    double Z() { return y[3]; }
    void Set(double sigma, double b, double r) {
        Sigma = sigma;
        B = b;
        R = r;
    }
    void Derivs(double t, double y[], double dydt[]) {
        dydt[1] = Sigma * (y[2] - y[1]);
        dydt[2] = R * y[1] - y[2] - y[1] * y[3];
        dydt[3] = y[1] * y[2] - B * y[3];
    }
};

#endif
