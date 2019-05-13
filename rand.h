// Toy/::rand().h
//
// Last Modified : 2009/01/31
//

#ifndef _TOY_RAND_H_
#define _TOY_RAND_H_

#include <stdlib.h>

#ifndef __AFX_H__
#include <time.h>
#endif

#ifdef TOY
#include <Toy/message.h>
#include <Toy/mutil.h>
#include <Toy/util.h>
#else
#include "message.h"
#include "mutil.h"
#include "util.h"
#endif

BOOLEAN _bRandInit_ = FALSE;
inline int GetRand0(int min, int max) {
    return min + ::rand() % (max - min + 1);
}
inline int GetRand0(int n) { return ::rand() % n; }
inline double GetRand0() { return (double)::rand() / RAND_MAX; }
inline double GetRand0(double max) { return max * ::rand() / RAND_MAX; }
inline double GetRand0(double min, double max) {
    return min + (max - min) * ::rand() / RAND_MAX;
}
inline double GetLogRand0() { return log((::rand() + 1.) / (RAND_MAX + 1.)); }
inline void InitRand() {
    if(!_bRandInit_) {
        srand(time(NULL));
        _bRandInit_ = TRUE;
    }
}
inline int GetRand(int min, int max) {
    InitRand();
    return min + ::rand() % (max - min + 1);
}
inline int GetRand(int n) {
    InitRand();
    return ::rand() % n;
}
inline double GetRand() {
    InitRand();
    return (double)::rand() / RAND_MAX;
}
inline double GetRand(double max) {
    InitRand();
    return max * ::rand() / RAND_MAX;
}
inline double GetRand(double min, double max) {
    InitRand();
    return min + (max - min) * ::rand() / RAND_MAX;
}

// return min + (int)((max-min+1.)*::rand()/(RAND_MAX+1.));
// return min + (max-min)*::rand()/(RAND_MAX+1.);
// return sigma*sqrt(-2.*log(GetRand(TolZero, 1.)))*cos(GetRand(M_PI2));

/////////////////////////////////////////////////////////////////////////////
// Gaussian Deviates

inline double GetGRand0(double sigma = 1.) {
    InitRand();
    return sigma * sqrt(-2 * log((::rand() + 1.) / (RAND_MAX + 1.)));
}

inline double GetGRand(double sigma = 1.) {
    InitRand();
    return sigma * sqrt(-2 * log((::rand() + 1.) / (RAND_MAX + 1.)))
           * cos(M_PI2 * ::rand() / (RAND_MAX + 1.));
}
inline double GetGRand(double mean, double sigma) {
    return mean + GetGRand(sigma);
}

/////////////////////////////////////////////////////////////////////////////
//
// Gamma Deviates
//
// Originate from Numerical Recipes Software

inline double GetGamma(int ia) {
    int j;
    double am, e, s, v1, v2, x, y;

    if(ia < 1) Error("Error in routine GetGamma in rand.h");
    if(ia < 6) {
        x = 1.0;
        for(j = 1; j <= ia; j++) x *= GetRand();
        x = -log(x);
    } else {
        do {
            do {
                do {
                    v1 = 2.0 * GetRand() - 1.0;
                    v2 = 2.0 * GetRand() - 1.0;
                } while(v1 * v1 + v2 * v2 > 1.0);
                y = v2 / v1;
                am = ia - 1;
                s = sqrt(2.0 * am + 1.0);
                x = s * y + am;
            } while(x <= 0.0);
            e = (1.0 + y * y) * exp(am * log(x / am) - s * y);
        } while(GetRand() > e);
    }
    return x;
}

/////////////////////////////////////////////////////////////////////////////
//
// Poisson Deviates
//
// Originate from Numerical Recipes Software

inline double gammln(double xx) {
    double x, y, tmp, ser;
    static double cof[6] = {76.18009172947146,     -86.50532032941677,
                            24.01409824083091,     -1.231739572450155,
                            0.1208650973866179e-2, -0.5395239384953e-5};
    int j;

    y = x = xx;
    tmp = x + 5.5;
    tmp -= (x + 0.5) * log(tmp);
    ser = 1.000000000190015;
    for(j = 0; j <= 5; j++) ser += cof[j] / ++y;
    return -tmp + log(2.5066282746310005 * ser / x);
}

inline double GetPoisson(double xm) {
    static double sq, alxm, g, oldm = (-1.0);
    double em, t, y;

    if(xm < 12.0) {
        if(xm != oldm) {
            oldm = xm;
            g = exp(-xm);
        }
        em = -1;
        t = 1.0;
        do {
            ++em;
            t *= GetRand();
        } while(t > g);
    } else {
        if(xm != oldm) {
            oldm = xm;
            sq = sqrt(2.0 * xm);
            alxm = log(xm);
            g = xm * alxm - gammln(xm + 1.0);
        }
        do {
            do {
                y = tan(M_PI * GetRand());
                em = sq * y + xm;
            } while(em < 0.0);
            em = floor(em);
            t = 0.9 * (1.0 + y * y) * exp(em * alxm - gammln(em + 1.0) - g);
        } while(GetRand() > t);
    }
    return em;
}

/////////////////////////////////////////////////////////////////////////////
//
// class Random
//

class Random {
    int m_max, m_r, m_d, m_m;

  public:
    Random() {}
    Random(int max) { Init(max); }
    void Init(int max) {
        m_max = max;
        srand(time(NULL));
        m_d = m_m = 0;
        if(m_max > 1)
            for(int d = RAND_MAX; d >= m_max; d /= m_max) m_m++;
    }
    int GetRand() {
        if(!m_d) {
            m_r = ::rand();
            m_d = m_m;
        }
        int r = m_r % m_max;
        m_r /= m_max;
        m_d--;
        return r;
    }
};

class Random2 {
    int m_bits, m_r, m_d, m_m, m_mask;

  public:
    Random2() {}
    Random2(int bits) { Init(bits); }
    void Init(int bits) {
        m_bits = bits;
        srand(time(NULL));
        m_d = 0;
        m_mask = 0;
        for(int i = 0; i < m_bits; i++) m_mask = (m_mask << 1) | 1;
        m_m = 0;
        if(m_bits)
            for(int d = RAND_MAX; d >= m_mask; d >>= bits) m_m++;
    }
    int GetRand() {
        if(!m_d) {
            m_r = ::rand();
            m_d = m_m;
        }
        int r = m_r & m_mask;
        m_r >>= m_bits;
        m_d--;
        return r;
    }
};

/////////////////////////////////////////////////////////////////////////////
//
// class Shuffle
//

class Shuffle {
    int m_min, m_max;
    int m_last, m_size;
    int *m_index;
    int m_r, m_d, m_m;

  public:
    Shuffle() {}
    Shuffle(int size) { Alloc(0, size - 1); }
    Shuffle(int min, int max) { Alloc(min, max); }
    void Alloc(int size) { Alloc(0, size - 1); }
    void Alloc(int min, int max) {
        m_min = min;
        m_max = max;
        m_size = m_last = m_max - m_min + 1;
        m_index = new int[m_size];
        for(int i = m_min; i <= m_max; i++) m_index[i - m_min] = i;
        srand(time(NULL));
        m_d = m_m = 0;
        if(m_size > 1)
            for(int d = RAND_MAX; d >= m_size; d /= m_size) m_m++;
    }
    ~Shuffle() { delete[](m_index); }
    void Init() {
        for(int i = m_min; i <= m_max; i++) m_index[i - m_min] = i;
        m_last = m_size;
    }
    int GetRand() {
        // int i = m_min+::rand()%m_last;
        if(!m_d) {
            m_r = ::rand();
            m_d = m_m;
        }
        int i = m_min + m_r % m_last;
        m_r /= m_size;
        m_d--;
        int r = m_index[i];
        m_last--;
        m_index[i] = m_index[m_last];
        return r;
    }
};

#endif
