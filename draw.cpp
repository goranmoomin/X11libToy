//#define _DEBUG
#include "canvas.h"
#include "matrix.h"
#include "rutil.h"
#include <stdio.h>
#include <stdlib.h>

int N;
int M;
const int pixel = 3;

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
    double dx = pixel*cos(a)/2;
    double dy = -pixel*sin(a)/2;
    canvas.SetForeground(BlackColor);
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

int main(int argc, char *argv[]) {
    if(argc < 2) Error("Usage : %s filename", argv[0]);
    Matrix W;
    int k = ::ToInt(argv[1]);
    char filename[256];
    // sprintf(filename, "opm%s.dat", argv[1]);
    FILE *fp = fopen(argv[1], "rb");
    W.ReadBinary(fp);
    int t;
    fread(&t, sizeof(int), 1, fp);
    fclose(fp);

    N = W.RowSize();
    M = W.ColSize();
    if(N * M == 0) Error("Size of data is 0");

    Canvas canvas(N * pixel, M * pixel, " k = %s  (%d x100 iteration)",
                  ::ToStr(strlen(argv[1]) == 3 ? k / 100. : k / 10.), t);

    Draw(canvas, W);
    getchar();
}
