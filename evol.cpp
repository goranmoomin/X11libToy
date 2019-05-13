//#define _DEBUG
#include "canvas.h"
#include "matrix.h"
#include "rutil.h"
#include <stdio.h>
#include <stdlib.h>

const int N = 70;
const int M = 70;
const int pixel = 4;

void DrawPixel(Canvas &canvas, int i, int j, double a) {
    a = ::Modulate(a, M_PI);
    if(a < M_PI / 16 || a >= M_PI * 15 / 16)
        canvas.SetForeground(RGB(247, 66, 0));
    else if(a >= M_PI / 16 && a < M_PI * 3 / 16)
        canvas.SetForeground(RGB(255, 255, 0));
    else if(a >= M_PI * 3 / 16 && a < M_PI * 5 / 16)
        canvas.SetForeground(RGB(148, 239, 49));
    else if(a >= M_PI * 5 / 16 && a < M_PI * 7 / 16)
        canvas.SetForeground(RGB(82, 214, 132));
    else if(a >= M_PI * 7 / 16 && a < M_PI * 9 / 16)
        canvas.SetForeground(RGB(66, 231, 255));
    else if(a >= M_PI * 9 / 16 && a < M_PI * 11 / 16)
        canvas.SetForeground(RGB(33, 82, 206));
    else if(a >= M_PI * 11 / 16 && a < M_PI * 13 / 16)
        canvas.SetForeground(RGB(99, 66, 206));
    else if(a >= M_PI * 13 / 16 && a < M_PI * 15 / 16)
        canvas.SetForeground(RGB(214, 33, 140));
    int x = i * pixel, y = j * pixel;
    canvas.FillRectangle(x, y, x + pixel - 1, y + pixel - 1);

    /*
    canvas.SetForeground(BlackColor);
    double dx = pixel*cos(a)/2;
    double dy = -pixel*sin(a)/2;
    canvas.DrawLine((int)(x+pixel/2-dx), (int)(y+pixel/2-dy),
                    (int)ceil(x+pixel/2+dx), (int)ceil(y+pixel/2+dy));
    */
}

void Draw(Canvas &canvas, const Matrix &w) {
    LOOP_MATRIX_INT(w, i, j) DrawPixel(canvas, i, j, w[i][j]);
    canvas.Flush();
}

double da(double a) {
    a -= (int)(a / M_PI2) * M_PI2;
    if(a > M_PI)
        a -= M_PI2;
    else if(a < -M_PI)
        a += M_PI2;
    return (a >= 0. ? ::GetMin(M_PI - a, a) : ::GetMax(-M_PI - a, a)) / M_PI_2;
}

void WriteBinary(const Matrix &w, const char *filename, int iter) {
    FILE *fp = fopen(filename, "wb");
    w.WriteBinary(fp);
    fwrite(&iter, sizeof(int), 1, fp);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    double s2 = 6.;
    double k = 1.0;
    double eta = 0.01;
    char *fn = "opm10";

    // Canvas canvas(N*pixel, M*pixel);

    Matrix W(N, M), prevW;
    LOOP_MATRIX_INT(W, i, j) W[i][j] = ::GetRandom(-M_PI, M_PI);

    for(int t = 0; t < 100; t++) {
        // canvas.SetTitle("k = %s, (%d iterates)", ::ToStr(k), t);
        char filename[256];
        sprintf(filename, "%s.%03d", fn, t);
        WriteBinary(W, filename, t);
        // Draw(canvas, W);
        for(int n = 0; n < 100; n++) {
            prevW = W;
            LOOP_MATRIX_INT(W, x0, y0) {
                int x1 = GetMax(x0 - 9, 0), x2 = GetMin(x0 + 9, N - 1);
                int y1 = GetMax(y0 - 9, 0), y2 = GetMin(y0 + 9, M - 1);
                double F = 0.;
                for(int x = x0 - 9; x <= x0 + 9; x++)
                    for(int y = y0 - 9; y <= y0 + 9; y++) {
                        double r2 = SQR(x - x0) + SQR(y - y0);
                        double h_rs = (1 - k * r2 / s2) * exp(-r2 / (2 * s2));
                        double dW2 =
                            (prevW[x0][y0] - prevW[(x + N) % N][(y + M) % M])
                            * 2;
                        F += h_rs * sin(dW2);
                    }
                W[x0][y0] -= eta * F;
            }
        }
    }
    // puts("End --");
    // getchar();
}
