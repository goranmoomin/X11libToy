/*
**	      File : clip.c
**	Programmer : Chung, Yun-Chan
**	      Date : 4/5/1990
**
**	1) two-dimensional clipping algorithm to remove hidden line.
**	2) Test a point with Box2DI; Interactive picking.
**	3) Test a point with Zmap.
**
**
**	CLIP_CENTER = 0 	 0000 	    1010 |1000 | 1001
**	CLIP_RIGHT  = 1		 0001	   <-----+-----+----->
**	CLIP_LEFT   = 2 	 0010  	         |     |
**	CLIP_DOWN   = 4 	 0100  	    0010 |0000 | 0001
**	CLIP_UP	    = 8		 1000  	   <-----+-----+----->
**	CLIP_ANY    = 15	 1111  	    0110 |0100 | 0101
*/

#ifndef WIN32
#include <stdio.h>
#include <stdlib.h>
#endif
#include "canvas.h"
#include "clip.h"
#include "mutil.h"

/*
void Intersection(Point *pt1, Point pt2, ClipCode endcode)
{
    int d;

    if (endcode & CLIP_UP ) {
        if ((d = pt2.y - pt1->y) != 0)
            pt1->x += (pt2.x - pt1->x) *(g_pCanvas->m_view.MaxY() - pt1->y) / d;
            pt1->y = g_pCanvas->m_view.MaxY();
        } else if( endcode & CLIP_DOWN ) {
            pt1->x += (pt2.x - pt1->x)*(g_pCanvas->m_view.MinY() - pt1->y)
                                /(pt2.y - pt1->y);
            pt1->y = g_pCanvas->m_view.MinY();
        } else if( endcode & CLIP_RIGHT ) {
            pt1->y += (pt2.y - pt1->y)*(g_pCanvas->m_view.MaxX() - pt1->x)
                                /(pt2.x - pt1->x);
            pt1->x = g_pCanvas->m_view.MaxX();
        } else if( endcode & CLIP_LEFT ) {
            pt1->y += (pt2.y - pt1->y)*(g_pCanvas->m_view.MinX() - pt1->x)
                                /(pt2.x - pt1->x);
            pt1->x = g_pCanvas->m_view.MinX();
        }
}

int Clip_HiddenLine(Point *P1, Point *P2)
{
    ClipCode P1code = OutCode(*P1);
    ClipCode P2code = OutCode(*P2);

    if ( (P1code == 0) && (P2code == 0) )  return(CLIP_IN);
    if ( P1code & P2code ) return(CLIP_OUT);

    do {
        if ( P1code ) {
            Intersection(P1,*P2,P1code);
            P1code = OutCode(*P1);
        } else{
            Intersection(P2,*P1,P2code);
            P2code = OutCode(*P2);
        }
        if (!(P1code | P2code)) return(CLIP_IN);
        if ( P1code & P2code ) return(CLIP_OUT);
    } while ( P1code | P2code );

    return (CLIP_IN);
}


void InsPoint(ClipCode c1, ClipCode c2, int *n, Point *OutPt)
{
    ClipCode code;

    if ( (c1==0) || (c2==0) || (c1==c2) ) return;

    (*n)++;
    code = (c1|c2);
    if (code & CLIP_LEFT) OutPt[*n-1].x = g_pCanvas->m_view.MinX();
                     else OutPt[*n-1].x = g_pCanvas->m_view.MaxX();
    if (code & CLIP_DOWN) OutPt[*n-1].y = g_pCanvas->m_view.MinY();
                     else OutPt[*n-1].y = g_pCanvas->m_view.MaxY();
}



void	Clip_Line(Point pt1, Point pt2, ClipCode startCode,
                ClipCode endCode, int *n, Point *OutPt, ClipCode *code)
{
        ClipCode	tc1,tc2;
        int		accept,reject;

        if ( startCode == 0 ) {
           tc1 = 0;
           while( endCode != 0 ) {
                tc1 = SubDivide(&pt2,pt1,endCode);
                endCode = OutCode(pt2);
           }
           InsPoint(code[*n],tc1,n,OutPt);
           (*n)++;
           OutPt[*n-1] = pt2;
           code[*n] = tc1;
        } else {
           accept = FALSE;	reject = FALSE;
           tc1 = 0;	tc2 = 0;
           while ( (accept==FALSE) && (reject==FALSE) ) {
                if( (startCode|endCode) == 0 )	accept = TRUE;
                else if( startCode & endCode )	reject = TRUE;
                else if( startCode ) {
                   tc1 = SubDivide(&pt1,pt2,startCode);
                   startCode = OutCode(pt1);
                } else if( endCode ) {
                   tc2 = SubDivide(&pt2,pt1,endCode);
                   endCode = OutCode(pt2);
                }
           }
           if( accept == TRUE ) {
                InsPoint( code[*n],tc1,n,OutPt );
                (*n)++;
                OutPt[*n-1] = pt1;	code[*n] = tc1;
                (*n)++;
                OutPt[*n-1] = pt2;	code[*n] = tc2;
           }
        }
}



void Clip_Shading(Point *InPt, int *n, Point *OutPt)
{
        ClipCode c1,c2,c3,c[8];


        c1 = OutCode(InPt[0]);
        c2 = OutCode(InPt[1]);
        c3 = OutCode(InPt[2]);

        if( (c1|c2|c3) == 0 ) {
           *n = 3;
           OutPt[0] = InPt[0];
           OutPt[1] = InPt[1];
           OutPt[2] = InPt[2];
        } else if ( c1 & c2 & c3 ) {
           *n = 0;
        } else {
           *n  = 0; 	c[0] = 0;
           Clip_Line ( InPt[0],InPt[1],c1,c2,n,OutPt,c );
           Clip_Line ( InPt[1],InPt[2],c2,c3,n,OutPt,c );
           Clip_Line ( InPt[2],InPt[0],c3,c1,n,OutPt,c );
           InsPoint (c[*n],c[1],n,OutPt);
        }
}

ClipCod SubDivide(Point *pt1, Point pt2, ClipCode code)
{
    if (code & CLIP_UP) {
        pt1->x = pt1->x + (pt2.x - pt1->x)*(g_pCanvas->m_view.MaxY() - pt1->y)
                                /(pt2.y - pt1->y);
        pt1->y = g_pCanvas->m_view.MaxY();
        return CLIP_UP;
    } else if (code & CLIP_DOWN) {
        pt1->x = pt1->x + (pt2.x - pt1->x)*(g_pCanvas->m_view.MinY() - pt1->y)
                                /(pt2.y - pt1->y);

        pt1->y = g_pCanvas->m_view.MinY();
        return CLIP_DOWN ;
    } else if (code & CLIP_RIGHT) {
        pt1->y = pt1->y + (pt2.y - pt1->y)*(g_pCanvas->m_view.MaxX() - pt1->x)
                                /(pt2.x - pt1->x);
        pt1->x = g_pCanvas->m_view.MaxX();
        return CLIP_RIGHT;
    } else if (code & CLIP_LEFT) {
        pt1->y = pt1->y + (pt2.y - pt1->y)*(g_pCanvas->m_view.MinX() - pt1->x)
                                /(pt2.x - pt1->x);
        pt1->x = g_pCanvas->m_view.MinX();
        return(CLIP_LEFT);
    }
    return CLIP_CENTER;
}

ClipCode OutCode(Point pt)
{
    ClipCode code = 0;

    if ( pt.x > g_pCanvas->m_view.MaxX() ) code = CLIP_RIGHT;
    else if( pt.x < g_pCanvas->m_view.MinX() ) code = CLIP_LEFT;

    if ( pt.y > g_pCanvas->m_view.MaxY() ) code =  code|CLIP_UP;
    else if ( pt.y < g_pCanvas->m_view.MinY() ) code =  code|CLIP_DOWN;

    return(code);
}
*/

ClipCode OutCodeBox2DI(const Box2DI &box, int x, int y) {
    ClipCode code = 0;

    if(x < box.MinX())
        code = CLIP_LEFT;
    else if(x > box.MaxX())
        code = CLIP_RIGHT;

    if(y < box.MinY())
        code = code | CLIP_DOWN;
    else if(y > box.MaxY())
        code = code | CLIP_UP;

    return (code);
}

ClipCode OutCodeBox2DI(const Box2DI &box, Index2D p) {
    return OutCodeBox2DI(box, p.x, p.y);
}

ClipCode OutCodeBox2DR(Box2DR *mb, double x, double y) {
    ClipCode code = 0;

    if(x < mb->MinX())
        code = CLIP_LEFT;
    else if(x > mb->MaxX())
        code = CLIP_RIGHT;

    if(y < mb->MinY())
        code = code | CLIP_DOWN;
    else if(y > mb->MaxY())
        code = code | CLIP_UP;

    return code;
}

ClipCode OutCodeZ(double testz, double stdz, double tol) {
    ClipCode code = 0;

    if(testz < stdz - tol)
        code = CLIP_DOWN;
    else if(testz > stdz + tol)
        code = CLIP_UP;

    return (code);
}

BOOLEAN CheckClipBox2DI(const Box2DI &box, int sx, int sy, int ex, int ey,
                        ClipCode scode, ClipCode ecode) {
    ClipCode code = scode | ecode;

    if(scode == CLIP_CENTER || ecode == CLIP_CENTER) return TRUE;
    if(code == CLIP_DU || code == CLIP_LR) return TRUE;
    if(scode & ecode) return FALSE;

    double a = (double)(ey - sy) / (ex - sx);
    double b = sy - a * sx;
    if((scode & CLIP_LEFT) || (ecode & CLIP_LEFT)) {
        double y = a * box.MinX() + b;
        if(y > box.MinY() && y < box.MaxY()) return TRUE;
    }
    if((scode & CLIP_RIGHT) || (ecode & CLIP_RIGHT)) {
        double y = a * box.MaxX() + b;
        if(y > box.MinY() && y < box.MaxY()) return TRUE;
    }

    a = (double)(ex - sx) / (ey - sy);
    b = sx - a * sy;
    if((scode & CLIP_DOWN) || (ecode & CLIP_DOWN)) {
        double x = a * box.MinY() + b;
        if(x > box.MinX() && x < box.MaxX()) return TRUE;
    }
    if((scode & CLIP_UP) || (ecode & CLIP_UP)) {
        double x = a * box.MaxY() + b;
        if(x > box.MinX() && x < box.MaxX()) return TRUE;
    }

    return FALSE;
}

BOOLEAN CheckClipBox2DI(const Box2DI &box, int sx, int sy, int ex, int ey) {
    return CheckClipBox2DI(box, sx, sy, ex, ey, OutCodeBox2DI(box, sx, sy),
                           OutCodeBox2DI(box, ex, ey));
}

BOOLEAN CheckClipBox2DI2(const Box2DI &mb, Index2D sp, Index2D ep) {
    return CheckClipBox2DI(mb, sp.x, sp.y, ep.x, ep.y, OutCodeBox2DI(mb, sp),
                           OutCodeBox2DI(mb, ep));
}

BOOLEAN CheckClipBox2DR(Box2DR *mb, Vector2D sp, Vector2D ep, ClipCode scode,
                        ClipCode ecode) {
    ClipCode code = scode | ecode;
    double a, b;

    if(scode == CLIP_CENTER || ecode == CLIP_CENTER) return TRUE;
    if(code == CLIP_DU || code == CLIP_LR) return TRUE;
    if(scode & ecode) return FALSE;

    a = (double)(ep.y - sp.y) / (ep.x - sp.x);
    b = sp.y - a * sp.x;

    if((scode & CLIP_UL) && (ecode & CLIP_UL))
        if(a * mb->MinX() + b > mb->MaxY()) return FALSE;
    if((scode & CLIP_UR) && (ecode & CLIP_UR))
        if(a * mb->MaxX() + b > mb->MaxY()) return FALSE;
    if((scode & CLIP_DL) && (ecode & CLIP_DL))
        return (a * mb->MinX() + b < mb->MinY()) ? FALSE : TRUE;
    if((scode & CLIP_DR) && (ecode & CLIP_DR))
        return (a * mb->MaxX() + b < mb->MinY()) ? FALSE : TRUE;

    return TRUE;
}

BOOLEAN CheckLineCrossBox2DR(Box2DR *mb, Vector2D sp, Vector2D ep) {
    return CheckClipBox2DR(mb, sp, ep, OutCodeBox2DR(mb, sp.x, sp.y),
                           OutCodeBox2DR(mb, ep.x, ep.y));
}

//
// v[0]*x+v[1]*y=v[2]
//
BOOLEAN GetLineEqn(Point pt[2], Vector3D &v) {
    v.z = (double)(pt[0].x * pt[1].y - pt[0].y * pt[1].x);
    if(v.z == 0) {
        if(pt[0].x == pt[1].x) return FALSE; /* same pt */
        v.x = (double)(pt[0].y);
        v.y = (double)(-pt[0].x);
        v.z = 0.f;
    } else {
        v.x = (double)(pt[1].y - pt[0].y);
        v.y = (double)(pt[0].x - pt[1].x);
    }
    return TRUE;
}

BOOLEAN CheckLineCross(Point pt1[2], Point pt2[2], double *x, double *y) {
    Vector3D v1, v2;
    double D;
    long ix, iy;

    if(!GetLineEqn(pt1, v1) || !GetLineEqn(pt2, v2)) return FALSE;
    D = v1.x * v2.y - v1.y * v2.x;
    if(fabs(D) < TolZero) {
        if(fabs(v1.x * v2.z - v2.x * v1.z) > TolZero) return FALSE;
        *x = (pt1[0].x + pt1[1].x) / 2.f;
        *y = (pt1[0].y + pt1[1].y) / 2.f;
    } else {
        *x = (v2.y * v1.z - v1.y * v2.z) / D;
        *y = (v1.x * v2.z - v2.x * v1.z) / D;
    }
    ix = GetRound(*x);
    iy = GetRound(*y);
    if(ix > pt1[0].x && ix > pt1[1].x) return FALSE;
    if(ix < pt1[0].x && ix < pt1[1].x) return FALSE;
    if(iy > pt1[0].y && iy > pt1[1].y) return FALSE;
    if(iy < pt1[0].y && iy < pt1[1].y) return FALSE;
    if(ix > pt2[0].x && ix > pt2[1].x) return FALSE;
    if(ix < pt2[0].x && ix < pt2[1].x) return FALSE;
    if(iy > pt2[0].y && iy > pt2[1].y) return FALSE;
    if(iy < pt2[0].y && iy < pt2[1].y) return FALSE;
    return TRUE;
}
