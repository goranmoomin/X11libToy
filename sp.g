#include "stdafx.h"
#ifdef TOY
#include "toy/zmap.h"
#include "toy/canvas.h"
#include "toy/vector.h"
#else
#include "zmap.h"
#include "canvas.h"
#include "vector.h"
#endif

classs Vector3D {
public:
    double x, y, z;

    void Set(double _x, double _y, double _z) { x = _x;  y = _y;  z = _z; }
};

class Point {
public:
    int x, y; 
};

#define IntersectError		4.0
#define	UNDEFINED -9.9E9f
#define	IsUndefined(x)		((x) < -9.0E9 ? TRUE : FALSE)

void Graph::wtov(double x, double y, double z, Point &p)
{
    WorldToView(x, y, z, &p.x, &p.y);
}

void Graph::wtov(const v3d &v, Point &p)
{
    WorldToView(v.x, v.y, v.z, &p.x, &p.y);
}

typedef enum { VO, VX, VD, VY } Corner;
//
//  VY *-----* VD
//     |     |
//     |     |
//  VO *-----* VX
//       -->X	
Corner Graph::GetClosestCorner(const Matrix &zmap)
{
    int vy = WinToViewY(zmap.RowFrom(), zmap.ColFrom(), 0.);
    int max = vy;
    Corner index = VO;
    vy = WinToViewY(zmap.RowTo(), zmap.ColFrom(), 0.);
    if (vy > max) { max = vy;  index = VX;  }
    vy = WinToViewY(zmap.RowTo(), zmap.ColTo(), 0.);
    if (vy > max) { max = vy;  index = VD;  }
    vy = WinToViewY(zmap.RowFrom(), zmap.ColTo(), 0.);
    if (vy > max) { max = vy;  index = VY;  }
    return index;
}

int Graph::GetFarthestCorner(Matrix *zmap)
{
    int vy = WinToViewY(zmap.RowFrom(), zmap.ColFrom(), 0.);
    int min = vy;
    Corner index = VO;
    vy = WinToViewY(zmap.RowTo(), zmap.ColFrom(), 0.);
    if (V.y < min) { min = V.y;  index = VX;  }
    vy = WinToViewY(zmap.RowTo(), zmap.ColTo(), 0.);
    if (V.y < min) { min = V.y;  index = VD;  }
    vy = WinToViewY(zmap.RowFrom(), zmap.ColTo(), 0.);
    if (V.y < min) { min = V.y;  index = VY;  }
    return index;
}

inline void ToCPoint(const Point& p, CPoint& cp) { cp.x = p.x; cp.y = p.y; }

//  --  BEGIN : Calculate shading intensity -------------------- 
#define    Cc	0.2f	/* Distance Constant */
#define    Cd	0.8f	/* Cc + Cd = 1 , Distance Factor */
#define    Ka	0.4f	/* Ambient diffuse reflection constasnt */
#define    Kd	0.3f	/* Diffuse Factor */
#define    Kr	0.7f	/* (1-Kd+Kr)*100% absorb, Reflect Factor */
#define    Ia	0	/* Incident ambient light intensity */
#define    Ip	120	/* Incident intensity from a point light source */
#define    n	2	/* textile <--> metallic */
#define    factorZ	1
#define    DiffuseTransFac	.9f

Vector3DR Intensity_ViewPoint, Sight;
float   Intensity_Work_Size;

int GetIntensity(Vector3D pt[3])
{
    // Distance from view-point to object
    Vector3D D = Intensity_ViewPoint;
    D -= pt[0];
    D.z *= factorZ;
    if (fabs(pt[1].z - pt[0].z) < TolZeroF &&
    fabs(pt[1].z - pt[2].z) < TolZeroF) {		// Normal is 0,0,1
	double Distance = (double)(85.f/Intensity_Work_Size);
	Vector3D N(0., 0., 1.);
	Vector3D R(0., 0., 1.);
	double Diffuse = 1.f;
	double Reflect = Sight[2];
    } else {
	double Distance = D.GetNorm() / Intensity_Work_Size;
	// unit surface Normal vector
	Vector3D p10 = pt[1];
	p10 -= pt[0];			/*		^2	*/ 
					/*		|	*/
					/*		|	*/
	Vector3D p21 = pt.z;
	p21 -= pt[1];			/*     0 -------+>1	*/

	N = p10 ^ p21;
	if (N.z < 0.) N *= -1.;
	N.Normalize();

	// unit Reflection vector
	double temp = 2. * N.z;
	R = N * temp;
	R.z -= 1.f;

	// Diffuse term;  N dot L
	// Diffuse = N[2];
	// L[0]*N[0] + L[1]*N[1] + L[2]*N[2] 

	if (N.z < TolZeroF)
	    Diffuse = 0.;
	else
	    Diffuse = (float)exp(DiffuseTransFac*log(N[2]));

	// Reflection term;  R dot Sight
	Reflect = R & Sight;
    }

    if (Reflect < TolZeroF)
	Reflect = 0.;
    else
	Reflect = (float)exp(n*log(Reflect));
    
    // return ( (Ia * Ka) + Ip * ( Kd * Diffuse + Kr * Reflect ) / Cc );
    return ( Ip * ( Kd * Diffuse + Kr * Reflect ) / ( Distance*Cd + Cc ) );
}

void Intensity_Initialise(Matrix &zmap)
{
    Vector3D work_center;

    /*---[ sight vector ]---*/
    Vector3D _sight = m_from.GetUnit();
    Sight.x = (double)_sight.x;
    Sight.y = (double)_sight.y;
    Sight.z = (double)_sight.z;

    // Work size
    double tmp = zmap->x1 - zmap->x0;
    double tmp2 = tmp*tmp;
    tmp = zmap->y1 - zmap->y0;
    tmp2 += tmp*tmp;
    Intensity_Work_Size = (double)sqrt(tmp2);
    
    // View Point;  diagonal distance from work center
    work_center.x = (zmap->x0+zmap->x1) / 2.f;
    work_center.y = (zmap->y0+zmap->y1) / 2.f;
    work_center.z = zmap->Zu;
    Intensity_ViewPoint = work_center + Intensity_Work_Size*Sight;
}

#undef    Cc
#undef    Cd
#undef    Ka
#undef    Kd
#undef    Kr
#undef    Ia
#undef    Ip	
#undef    n
#undef    factorZ	

//  --  END : Calculate shading intensity -------------------- 

void Graph::SZ_InsPoint(ClipCode c1, ClipCode c2, int *n, CPoint *OutV) 
{
    if (!(c1 & c2) | (c1 & c2)) return;
    ClipCode code = c1 | c2;
    OutV[*n].x = (code & CLIP_LEFT) ? View().MinX()
				    : View().MaxX();
    OutV[*n].y = (code & CLIP_DOWN) ? View().MinY()
	    			    : View().MaxY();
    (*n)++;
}

/*  --  Clip a line and inserts intersection points to OutV.
*/
void Graph::SZ_ClipLine(Point &pt1, Point &pt2, ClipCode startCode,
    ClipCode endCode, int *n, CPoint OutV[], ClipCode *code)
{

    if (!startCode) {
	ClipCode	tc = 0;

	while (endCode) {
	    tc = SubDivide (&pt2, pt1, endCode);
	    endCode = OutCode (pt2);
	}

	SZ_InsPoint (code[*n], tc, n, OutV);  
	ToCPoint (pt2, OutV[*n]);  (*n)++;  code[*n] = tc;
    } else {
	ClipCode tc1 = 0, tc2 = 0;
	int	     accept = FALSE, reject = FALSE;

	while (!accept && !reject) {
	    if (!(startCode | endCode)) accept = TRUE;
	    else if (startCode & endCode) reject = TRUE;
	    else if (startCode) {
		tc1 = SubDivide (&pt1, pt2, startCode);
		startCode = OutCode (pt1);
	    } else if (endCode) {
		tc2 = SubDivide (&pt2, pt1, endCode);
		endCode = OutCode (pt2);
	    }
	}

	if (accept) {
	    SZ_InsPoint(code[*n], tc1, n, OutV);
	    ToCPoint(pt1, OutV[*n]);  (*n)++;  code[*n] = tc1;
	    ToCPoint(pt2, OutV[*n]);  (*n)++;  code[*n] = tc2;
	}
    }
}
    


//  --  Clip a triangle with viewing window.
//    Returns the number of points of cliped polygon.
int Graph::SZ_ClipTri(Point *InV, CPoint *OutV, int n = 3)
{
    ClipCode c1 = OutCode(InV[0]);
    ClipCode c2 = OutCode(InV[1]);
    ClipCode c3 = OutCode(InV[2]);

    if (n != 3) {
	for (int i = 0;  i < n;  i++)
    	ToCPoint(InV[i], OutV[i]);  
	return n;
    }
    if (!(c1|c2|c3)) {
	ToCPoint(InV[0], OutV[0]);  
	ToCPoint(InV[1], OutV[1]);
	ToCPoint(InV[2], OutV[2]);
	return 3;
    } else if (c1 & c2 & c3) {
	return 0;
    } else {
	ClipCode c[8];
	int	 n = 0;  
	c[0] = 0;
	SZ_ClipLine(InV[0], InV[1], c1, c2, &n, OutV, c);
	SZ_ClipLine(InV[1], InV[2], c2, c3, &n, OutV, c);
	SZ_ClipLine(InV[2], InV[0], c3, c1, &n, OutV, c);
	SZ_InsPoint(c[n], c[1], &n, OutV);
	return n;
    }
}

//  --  Clip a triangle and fills the cliped polygon.
//  INPUT : P[3] represents the triangle in world coordinate. They are used in 
//	    calculating the light intensity.
//   	    V[3] represents the triangle in viewport.
void Graph::SZ_ClipAndFill(Vector3DR *P, Point *V, int n = 3)
{
    CPoint	ClipedV[10];
    int		color;

    if (!P) return;

    n = SZ_ClipTri(V, ClipedV, n);

    if (n <= 0) return;
    color = GetIntensity(P);

    if (color >= 0) {
	SetColor(GrayColor(color));
	DrawPolygon(ClipedV, n);
    }
}

//  --  Shades a zmap with drawing parameter.
//    Returns FALSE, if escape is pressed; otherwise it returns TRUE.
//    It shades only inner surfaces of zmap.
BOOLEAN Graph::SZ_Inner(Matrix &zmap, int si, int sj, int di, int dj,
			int ci, int cj)
{
    Vector3D	P[4];  //- world points
    Point	V[4];  //- viewport points
    float	x, next_x, y;
    int		i, next_i, j, _j, m, n, l;
    int		visible[4];

    double **Z = zmap.m_buf;

    float Zb = zmap->Zd + TolZeroF;

    /*-- calc. increments in world */
    double dx = (double)di;
    double dy = (double)dj;
    double sy = GetZY(zmap, sj);

    BOOLEAN result = TRUE;

    /*-- for all i */
    for (m = 0, i = si, next_i = si+di, x = GetZX(zmap, i), next_x = x+dx;
      m < ci;  m++, i = next_i, next_i += di, x = next_x, next_x += dx) {

	//-- 0th pt.
	P[0].Set(x, start_y, Z[i][sj]);
	if (P[0].z < Zb) { P[0].z = Zb;  visible[0] = 0; } 
	else visible[0] = 1;
	wtov(P[0], &V[0]);
	/*-- 1st pt. */
	P[1].Set(next_x, start_y, Z[next_i][sj]);
	if (P[1].z < Zb) { P[1].z = Zb;  visible[1] = 0; }
	else visible[1] = 1;
	wtov(P[1], &V[1]);

	for (n = 0, j = sj+dj, y = sy+dy;  n < dj;  n++, j += dj, y += dy) {
	    if (bStrip)
	    for (l = n-1, _j = j-inc_j;  l < count_j;  l++, _j += inc_j) {
    		if (fabs(P[0].z-Z[i][_j]) > TolZeroF) break;
    		if (fabs(P[0].z-Z[next_i][_j]) > TolZeroF) break;
	    }
	    if (bStrip && l > n+1) {		// if flat strip
		//-- 2nd pt.
/*   3   2  */  P[2].Set(next_x, sy+dy*l, Z[next_i][sj+dj*l]);
/*   +---+  */  if (P[2].z < Zb) { P[2].z = Zb;  visible[2] = 0;  } 
/*   |   |  */	else visible[2] = 1;
/*   |   |  */  wtov(P[2], &V[2]);
/*   |   |  */  //-- 3rd pt.
/*   +---+  */  P[3].Set(x, sy+dy*l, Z[i][sj+dj*l]);
/*   0   1  */  if (P[3].z < Zb) { P[3].z = Zb;  visible[3] = 0;  }
		else visible[3] = 1;
		wtov(P[3], &V[3]);
	    SZ_ClipAndFill(P, V, 4);
	    P[0] = P[3];  V[0] = V[3];  visible[0] = visible[3];
	    P[1] = P[2];  V[1] = V[2];  visible[1] = visible[2];
	    n = l-1;  j = _j-dj;  y = sy+(n+1)*dy;
	} else {
		//-- 2nd pt
/*   |   |  */	P[2].Set(x, y, Z[i][j]);
/*  2+   +  */	if (P[2].z < Zb) { P[2].z = Zb;  visible[2] = 0;  }
/*   |\  |  */	else visible[2] = 1;
/*   | \ |  */  wtov(P[2], &V[2]);
/*   |  \|  */	
/*   +---+  */  //-- clipping and shading
/*   0   1  */  if (visible[0] + visible[1] + visible[2] > 0)
		    SZ_ClipAndFill(P, V);

/*   |   |  */	//-- assign 1st -> 0th
/*  2+---+1 */  P[0] = P[1];  V[0] = V[1];  visible[0] = visible[1];
/*   |\  |  */
/*   | \ |  */  //-- 1st pt.
/*   |  \|  */  P[1].Set(next_x, y, Z[next_i][j]);
/*   +---+  */  if (P[1].z < Zb) { P[1].z = Zb;  visible[1] = 0; }
/*       0  */  else visible[1] = 1;
		wtov(P[1], &V[1]);
		//-- clipping and shading 
		if (visible[0] + visible[1] + visible[2] > 0)
		    SZ_ClipAndFill(P, V);
		//-- assign 2nd to 0th 
		P[0] = P[2];  V[0] = V[2];  visible[0] = visible[2];
	    }
	}
    }
    return result;
}


//  --  Shades the x-directional side wall of zmap with drawing parameters.
void Graph::SZ_SideWallX(Matrix &zmap, int si, int j, int inc_i, int count)
{
    double **Z = zmap.m_buf;
    double Zb = zmap->Zd + TolZeroF;

    double inc_x = (double)inc_i;

    Vector3DR P[3];
    Point     V[3];
    BOOLEAN   visible;

    int i = si;
    double x = GetZX(zmap, i);
    double y = GetZY(zmap, j);
    double z = Z[i][j];
    if (z < Zb) {
	visible = FALSE;
    } else {
	P[0].Set(x, y, z);
	P[1].Set(x, y, Zb);
	wtov(x, y, z, &V[0]);
	wtov(x, y, Zb, &V[1]);
	visible = TRUE;
    }

    int m;
    for (m = 1, i += inc_i, x += inc_x;  
    	m <= count;  m++, i += inc_i, x += inc_x) {

	z = Z[i][j];
	if (z < Zb) { 				/*     0	*/
    	if (visible) {				/*   --+	*/
    	    P[2].Set(x, y, Zb);  		/*     |\	*/
    	    wtov (x, y, Zb, &V[2]);		/*     | \	*/
    	    SZ_ClipAndFill (P, V);		/*   --+--*	*/
    	    visible = FALSE;			/*     1  2	*/
    	}
	} else {					
    	P[2].Set(x, y, z);      		/*     0  2	*/
    	wtov (x, y, z, &V[2]);			/*   --+--*-	*/
    						/*     | /|	*/
    	if (visible) {				/*     |/ |	*/ 
    	    SZ_ClipAndFill (P, V);		/*   --+--+-   	*/
    						
    	    P[0] = P[1];			/*   --+--+-    */
    	    V[0] = V[1];			/*    1->0	*/
    	} else {
    	    P[0].Set(x-inc_x, y, Zb);
    	    wtov (x-inc_x, y, Zb, &V[0]);	/*   --+--+-	*/
    	    visible = TRUE;			/*     0	*/
    	} 

	    P[1].Set(x, y, Zb); 	 	/*     |/ |	*/
	    wtov(x, y, Zb, &V[1]);		/*   --+--+-	*/
	    SZ_ClipAndFill(P, V);		/*     0  1	*/

	    P[0] = P[2];	  		/*	0	*/
	    V[0] = V[2];			/*   --+--*-	*/
	}
    }
}



//  --  Shades the x-directional side wall of zmap with drawing parameters.
void Graph::SZ_SideWallY(int sj, int i, int inc_j, int count)
{
    Vector3DR P[3];
    Point     V[3];
    int	      j, m;
    float     **Z, Zb, x, y, z, inc_y;
    BOOLEAN   visible;

    if (!zmap || !(Z = zmap->Z)) return;
    Zb = zmap->Zd + TolZeroF;

    inc_y = zmap->dy * (float)inc_j;

    j = sj;
    x = GetZX (zmap, i);  y = GetZY (zmap, j);  z = Z[i][j];
    if (z < Zb) {
	visible = FALSE;
    } else {
	P[0].Set(x, y, z);
	P[1].Set(x, y, Zb);
	wtov (x, y, z, &V[0]);
	wtov (x, y, Zb, &V[1]);
	visible = TRUE;
    }

    for (m = 1, j += inc_j, y += inc_y;  
    	m <= count;  m++, j += inc_j, y += inc_y) {

	z = Z[i][j];
	if (z < Zb) { 				/*     0	*/
    	if (visible) {				/*   --+	*/
    	    P[2].Set(x, y, Zb);  		/*     |\	*/
    	    wtov(x, y, Zb, &V[2]);		/*     | \	*/
    	    SZ_ClipAndFill (P, V);		/*   --+--*	*/
    	    visible = FALSE;			/*     1  2	*/
    	}
	} else {					
    	P[2].Set(x, y, z);      		/*     0  2	*/
    	wtov(x, y, z, V+2);			/*   --+--*-	*/
    						/*     | /|	*/
    	if (visible) {				/*     |/ |	*/ 
    	    SZ_ClipAndFill (P, V);		/*   --+--+-   	*/
    						
    	    P[0] = P[1];			/*   --+--+-    */
    	    V[0] = V[1];			/*    1->0	*/
    	} else {
    	    P[0].Set(x, y-inc_y, Zb);
    	    wtov (x, y-inc_y, Zb, &V[0]);	/*   --+--+-	*/
    	    visible = TRUE;			/*     0	*/
    	} 

    	P[1].Set(x, y, Zb); 	 		/*     |/ |	*/
    	wtov (x, y, Zb, &V[1]);			/*   --+--+-	*/
    	SZ_ClipAndFill (P, V);			/*	1	*/

    	P[0] = P[2];		  		/*	0	*/
    	V[0] = V[2];				/*   --+--*-	*/
	}
    }
    return;
}


//  --  Estimates optimum shading precision.
//  --  (optimum precision >= GRg.prec) but same visual quality.
//  --  line to be drawn per pixel smaller than 2.0
int Graph::SZ_GetOptimumPrec(Matrix &zmap, int prec, Box2DI *mb)
{
    int	   vsx, vsy, vex, vey;

    int N = ::GetMax(zmap.RowSize(), zmap.ColSize());

    prec = ::GetMax(prec, 1);

    // from (s,s) to (s,e)
    WorldToView(0., 0., 0., &vsx, &vsy);
    WorldToView(0., (double)N, 0., &vex, &vey);
    int maxlen = ::GetMax(abs(vex-vsx), abs(vey-vsy));

    // from (s,s) to (e,s)
    WorldToView((double)N, 0., 0., &vex, &vey);
    maxlen = ::GetMax(maxlen, GetMax(abs(vex-vsx), abs(vey-vsy));

    // calc. the # of line to be drawn per pixel
    double nline = (double)N*maxlen)/prec;

    if (nline < 1.5) 
	return prec;
    else
	return (int)(nline*(double)prec);
}

//  --  Calculates drawing parameters for shading.
BOOLEAN Graph::SZ_CalcParam(Matrix &zmap, int prec, int *si, int *sj,
	int *ei, int *ej, int *inc_i, int *inc_j, int *count_i, int *count_j)
{
    // get optimum precision
    prec = ::GetMax(SZ_GetOptimumPrec(zmap, prec), 1);

    switch (GetClosestCorner(zmap)) {
	case VO :
	    *si = zmap.RowTo();    *sj = zmap.ColTo();
    	    *ei = zmap.RowFrom();  *ej = zmap.ColFrom();
    	    *inc_i = -prec;        *inc_j = -prec;
    	    break;
	case VX :
	    *si = zmap.RowFrom();  *sj = zmap.ColTo();
	    *ei = zmap.RowTo();    *ej = zmap.ColFrom();
	    *inc_i = prec;         *inc_j = -prec;
	    break;
	case VD :
	    *si = zmap.RowFrom();  *sj = zmap.ColFrom();
	    *ei = zmap.RowTo();    *ej = zmap.ColTo();
	    *inc_i = prec;         *inc_j = prec;
	    break;
	case VY :
	    *si = zmap.RowTo();    *sj = zmap.ColFrom();
	    *ei = zmap.RowFrom();  *ej = zmap.ColTo();
	    *inc_i = -prec;        *inc_j = prec;
	    break;
    }
    if (DZ_GetOptimumRegion(zmap, si, sj, ei, ej, prec))
	*inc_i = *inc_j = prec;

    // get the # of line to be drawn
    if (*si < *ei) {
	*count_i = (*ei - *si)/(*inc_i);
    } else if (*si == *ei)
	return FALSE;
    } else {
	*inc_i = *ei - *si;
	*count_i = 1;
    }

    if (*sj < *ej) {
	*count_j = (*ej - *sj)/(*inc_j);
    } else if (*sj == *ej)
	return FALSE;
    } else {
	*inc_j = *ej - *sj;
	*count_j = 1;
    }

    // clac. exact end
    *ei = (*si) + (*count_i) * (*inc_i);
    *ej = (*sj) + (*count_j) * (*inc_j);

    return TRUE;
}

//  --  Shades a zmap in mb with precision.
//    Returns FALSE, if escape is pressed; otherwise it returns TRUE.
BOOLEAN Graph::DrawShade(Marix &zmap, int prec)
{
    int	si, sj, ei, ej,		// start/end index
    	count_i, count_j,	// the # of line to be drawn
    	inc_i, inc_j;		// increments in grid

    // drawing parameters
    if (!SZ_CalcParam(zmap, prec, &si, &sj, &ei, &ej, 
    		&inc_i, &inc_j, &count_i, &count_j)) return FALSE;

    BOOLEAN result = TRUE;

    // initialize intensity routine
    Intensity_Initialise(zmap);

    SZ_ClipAndFill(NULL, NULL);

    // shades inner zmap surfaces
    if (!SZ_Inner(zmap, si, sj, inc_i, inc_j, count_i, count_j, bStrip))
    	return FALSE;

    // shades side walls
    SZ_SideWallX(zmap, ei, ej, -inc_i, count_i);
    SZ_SideWallY(zmap, ej, ei, -inc_j, count_j);

    return result;
}
