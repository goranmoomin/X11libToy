// Toy/sort.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_SORT_H_
#define _TOY_SORT_H_

#ifdef TOY
#include <Toy/list.h>
#include <Toy/message.h>
#include <Toy/util.h>
#include <Toy/vector.h>
#else
#include "list.h"
#include "message.h"
#include "util.h"
#include "vector.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// BubbleSort

template <class Type> void BubbleSort(Type *a, int n) {
    BOOLEAN bAgain;
    do {
        bAgain = FALSE;
        for(int i = 1; i < n; i++)
            if(a[i - 1] > a[i]) {
                ::Swap(a[i - 1], a[i]);
                bAgain = TRUE;
            }
    } while(bAgain);
}
template <class Type> void BubbleSort(Array<Type> &a) {
    BubbleSort(a() - a.From(), a.Size());
}

template <class TypeA, class TypeB> void BubbleSort(TypeA *a, TypeB *b, int n) {
    BOOLEAN bAgain;
    do {
        bAgain = FALSE;
        for(int i = 1; i < n; i++)
            if(a[i - 1] > a[i]) {
                ::Swap(a[i - 1], a[i]);
                ::Swap(b[i - 1], b[i]);
                bAgain = TRUE;
            }
    } while(bAgain);
}
template <class TypeA, class TypeB>
void BubbleSort(Array<TypeA> &a, Array<TypeB> &b) {
    CHECK_MATCH(a, b);
    BubbleSort(a() + a.From, b() + b.From, a.Size);
}

template <class TypeA, class TypeB>
void BubbleSort(Array<TypeA> &a, LinkedList<TypeB> &list) {
    if(a.Size != list.GetLength()) return;

    BOOLEAN bAgain;
    do {
        bAgain = FALSE;
        for(int i = a.From + 1; i <= a.To; i++)
            if(a[i - 1] > a[i]) {
                ::Swap(a[i - 1], a[i]);
                list.Swap(i - 1, i);
                bAgain = TRUE;
            }
    } while(bAgain);
}

/////////////////////////////////////////////////////////////////////////////
// BubbleSort

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

// 1-base
template <class Type> void NRC_sort(unsigned long n, Type arr[]) {
    const int M = 7;
    const int NSTACK = 50;
    unsigned long i, ir = n, j, k, l = 1;
    int jstack = 0;
    Type a;

    Index istack(1, NSTACK);
    for(;;) {
        if(ir - l < M) {
            for(j = l + 1; j <= ir; j++) {
                a = arr[j];
                for(i = j - 1; i >= 1; i--) {
                    if(arr[i] <= a) break;
                    arr[i + 1] = arr[i];
                }
                arr[i + 1] = a;
            }
            if(jstack == 0) return;
            ir = istack[jstack--];
            l = istack[jstack--];
        } else {
            k = (l + ir) >> 1;
            ::Swap(arr[k], arr[l + 1]);
            if(arr[l + 1] > arr[ir]) ::Swap(arr[l + 1], arr[ir]);
            if(arr[l] > arr[ir]) ::Swap(arr[l], arr[ir]);
            if(arr[l + 1] > arr[l]) ::Swap(arr[l + 1], arr[l]);
            i = l + 1;
            j = ir;
            a = arr[l];
            for(;;) {
                do
                    i++;
                while(arr[i] < a);
                do
                    j--;
                while(arr[j] > a);
                if(j < i) break;
                ::Swap(arr[i], arr[j]);
            }
            arr[l] = arr[j];
            arr[j] = a;
            jstack += 2;
#ifdef _DEBUG
            if(jstack > NSTACK) Error("QuickSort:: too small stack.");
#endif
            if(ir - i + 1 >= j - l) {
                istack[jstack] = ir;
                istack[jstack - 1] = i;
                ir = j - 1;
            } else {
                istack[jstack] = j - 1;
                istack[jstack - 1] = l;
                l = i;
            }
        }
    }
}

template <class TypeA, class TypeB>
void NRC_sort2(unsigned long n, TypeA arr[], TypeB brr[]) {
    const int M = 7;
    const int NSTACK = 50;
    unsigned long i, ir = n, j, k, l = 1;
    int jstack = 0;
    TypeA a;
    TypeB b;

    Index istack(1, NSTACK);
    for(;;) {
        if(ir - l < M) {
            for(j = l + 1; j <= ir; j++) {
                a = arr[j];
                b = brr[j];
                for(i = j - 1; i >= 1; i--) {
                    if(arr[i] <= a) break;
                    arr[i + 1] = arr[i];
                    brr[i + 1] = brr[i];
                }
                arr[i + 1] = a;
                brr[i + 1] = b;
            }
            if(jstack == 0) return;
            ir = istack[jstack];
            l = istack[jstack - 1];
            jstack -= 2;
        } else {
            k = (l + ir) >> 1;
            ::Swap(arr[k], arr[l + 1]);
            ::Swap(brr[k], brr[l + 1]);
            if(arr[l + 1] > arr[ir]) {
                ::Swap(arr[l + 1], arr[ir]);
                ::Swap(brr[l + 1], brr[ir]);
            }
            if(arr[l] > arr[ir]) {
                ::Swap(arr[l], arr[ir]);
                ::Swap(brr[l], brr[ir]);
            }
            if(arr[l + 1] > arr[l]) {
                ::Swap(arr[l + 1], arr[l]);
                ::Swap(brr[l + 1], brr[l]);
            }
            i = l + 1;
            j = ir;
            a = arr[l];
            b = brr[l];
            for(;;) {
                do
                    i++;
                while(arr[i] < a);
                do
                    j--;
                while(arr[j] > a);
                if(j < i) break;
                ::Swap(arr[i], arr[j]);
                ::Swap(brr[i], brr[j]);
            }
            arr[l] = arr[j];
            arr[j] = a;
            brr[l] = brr[j];
            brr[j] = b;
            jstack += 2;
#ifdef _DEBUG
            if(jstack > NSTACK) Error("QuickSort:: too small stack.");
#endif
            if(ir - i + 1 >= j - l) {
                istack[jstack] = ir;
                istack[jstack - 1] = i;
                ir = j - 1;
            } else {
                istack[jstack] = j - 1;
                istack[jstack - 1] = l;
                l = i;
            }
        }
    }
}

// 0-base
template <class Type> void QuickSort(Type arr[], int n) {
    NRC_sort(n, arr - 1);
}

template <class Type> void QuickSort(Array<Type> &a, BOOLEAN bTopDown = FALSE) {
    NRC_sort(a.Size(), a.Offset(1));
    if(bTopDown) a.Reverse();
}

// 0-base
template <class TypeA, class TypeB>
void QuickSort(TypeA arr[], TypeB brr[], int n) {
    NRC_sort2(n, arr - 1, brr - 1);
}
template <class TypeA, class TypeB>
void QuickSort(Array<TypeA> &a, Array<TypeB> &b) {
    CHECK_MATCH(a, b);
    NRC_sort2(a.Size(), a() + a.From() - 1, b() + b.From() - 1);
}

template <class Type> void Sort(Array<Type> &data, Index &index) {
    index.Alloc(data.From(), data.To());
    LOOP_ARRAY_INT(index, i) index[i] = i;
    NRC_sort2(data.Size(), data() + data.From() - 1,
              index() + index.From() - 1);
}

template <class Type> int *GetOrder(const Array<Type> &aD, Index &aI) {
    int f = aD.From(), t = aD.To();
    double *D = aD();
    int *I = aI.Alloc(f, t);
    for(int i = f; i <= t; i++) I[i] = i;
    for(int i = f; i <= t; i++) {
        int max = i;
        for(int j = i + 1; j <= t; j++)
            if(D[I[max]] < D[I[j]]) max = j;
        ::Swap(&I[i], &I[max]);
    }
    return I;
}

#endif
