// Toy/cliptype.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_CLIPTYPE_H_
#define _TOY_CLIPTYPE_H_

typedef unsigned char ClipCode;

/*
**   1010 |1000 | 1001
**   -----+-----+-----
**   0010 |0000 | 0001
**   -----+-----+-----
**   0110 |0100 | 0101
*/
#define CLIP_CENTER 0 /* 0000 */
#define CLIP_RIGHT 1  /* 0001 */
#define CLIP_LEFT 2   /* 0010 */
#define CLIP_DOWN 4   /* 0100 */
#define CLIP_UP 8     /* 1000 */
#define CLIP_ANY 15   /* 1111 */

#define CLIP_LR 3  /* 0011 */
#define CLIP_DU 12 /* 1100 */
#define CLIP_UL 10 /* 1010 */
#define CLIP_UR 9  /* 1001 */
#define CLIP_DL 6  /* 0110 */
#define CLIP_DR 5  /* 0101 */

#define CLIP_OUT 0
#define CLIP_IN 1

#define DIR_S CLIP_DOWN
#define DIR_E CLIP_RIGHT
#define DIR_N CLIP_UP
#define DIR_W CLIP_LEFT
#define DIR_SE (DIR_S | DIR_E)
#define DIR_SW (DIR_S | DIR_W)
#define DIR_NE (DIR_N | DIR_E)
#define DIR_NW (DIR_N | DIR_W)

#endif
