/*
**  Programmer : Chung, Yun-Chan
**  Date : 9/15/1994
*/

#ifndef WIN32
#include <stdio.h>
#include <stdlib.h>
#endif
#include "message.h"
#include "mutil.h"
#include "util.h"
#include "zmap.h"

ZMap::ZMap(int nx, int ny) {
    ni = nx;
    nj = ny;
    x0 = (float)0;
    x1 = (float)(ni - 1);
    y0 = (float)0;
    y1 = (float)(nj - 1);
    dx = dy = (float)1;
    Z = alloc_2r(ni, nj);
}

ZMap::~ZMap() {
    if(Z) free_2r(Z, ni);
}

void ZMap::ResetZ() {
    max_z = min_z = Z[0][0];
    for(int i = 0; i < ni; i++)
        for(int j = 0; j < nj; j++) {
            max_z = GetMax(max_z, Z[i][j]);
            min_z = GetMin(min_z, Z[i][j]);
        }

    Zd = min_z;
    Zu = max_z;
    Zb = min_z;
}

Box3DD ZMap::GetEstimateBox() const {
    Box3DD box;
    box.SetX(x0, x1);
    box.SetY(y0, y1);
    box.SetZ(GetMin(Zd, min_z), GetMax(Zu, max_z));
    return box;
}

/*
BOOLEAN	CopyZofZmap(ZMAP_TYPE *A, ZMAP_TYPE *B)
{
        int	i, j ;

        if (!A || !B) return (FALSE) ;
        if (A->ni != B->ni || A->nj != B->nj) return (FALSE) ;

        for (i = 0; i < A->ni ; i++)
            for (j = 0; j < A->nj ; j++)
                B->Z[i][j] = A->Z[i][j] ;

        return (TRUE) ;
}
*/

/**********************************************************************
                        Miscellaneous
***********************************************************************/

//  --  'allocates memory for 2-dimensional array'
float **alloc_2r(int ni, int nj) {
    float **array;
    int i;

    if(ni * nj == 0) return NULL;

    array = (float **)calloc(ni, sizeof(float *));
    if(!array) {
        Warning("Memory error at array in alloc_2r");
        return (NULL);
    }
    for(i = 0; i < ni; i++) array[i] = NULL;

    for(i = 0; i < ni; i++) {
        array[i] = (float *)calloc(nj, sizeof(float));
        if(!array[i]) {
            Warning("Memory error at array[%d] in alloc_2r", i);
            free_2r(array, ni);
            return (NULL);
        }
    }
    return (array);
}

//  -- 'free 2-dimensional array of float'
void free_2r(float **array, int n) {
    int i;

    if(!array) return;

    for(i = 0; i < n; i++) {
        if(array[i] != NULL) delete array[i];
    }
    delete array;
}
