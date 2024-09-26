/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file bflib_vidraw.c
 *     Routines to be moved to bflibrary.
 * @par Purpose:
 *     Implement functions from bflibrary but not yet integrated there.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_vidraw.h"
#include "bftringl.h"

#include <stdlib.h>
#include <string.h>
#include "bfscreen.h"
#include "bfline.h"
/******************************************************************************/

#define DRAW_RANGES_COUNT (MAX_SUPPORTED_SCREEN_HEIGHT * 6 / 5)

enum TriangleCase {
    TriCase_Unkn24 = 0,
    TriCase_Unkn50,
    TriCase_Unkn77,
    TriCase_Unkn92,
};

enum RampType {
    Ramp_Single = 0,
    Ramp_SmoothLeft,
    Ramp_SmoothRight,
};

struct TbSPoint {
  short x;
  short y;
};

struct DrawRange { // sizeof = 8
    long x;
    long w;
};

extern struct DrawRange byte_1E957C[DRAW_RANGES_COUNT];

void LbDrawTriangleOutline(short x1, short y1, short x2, short y2, short x3, short y3, TbPixel colour)
{
    LbDrawLine(x1, y1, x2, y2, colour);
    LbDrawLine(x2, y2, x3, y3, colour);
    LbDrawLine(x3, y3, x1, y1, colour);
}

/** Blit given colour to given graphics window buffer, using a range to fill for each line.
 *
 * @param p_screen Output graphics window buffer.
 * @param ranges Array of ranges to fill; contains X coords and widths, each shifted left by 16.
 *
 * This function honors transparency set within DrawFlags.
 */
static void LbBlitSolidRanges(TbPixel *p_screen, struct DrawRange *ranges, int range_num, TbPixel colour)
{
    struct DrawRange *rng;

    rng = ranges;
    for (; range_num > 0; range_num--, rng++)
    {
        ushort m;
        int w, shift;
        ubyte *p_map;
        ubyte *o;

        shift = rng->x >> 16;
        w = rng->w >> 16;
        p_screen += lbDisplay.GraphicsScreenWidth;
        if (shift > 0)
        {
            if (w > lbDisplay.GraphicsWindowWidth)
                w = lbDisplay.GraphicsWindowWidth;
            w -= shift;
            if (w <= 0)
                continue;
            o = &p_screen[shift];
        }
        else
        {
            if (w <= 0)
                continue;
            if (w > lbDisplay.GraphicsWindowWidth)
                w = lbDisplay.GraphicsWindowWidth;
            o = p_screen;
        }
        p_map = lbDisplay.GlassMap;
        if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR4) != 0)
        {
            m = (colour << 8);
            for (; w > 0; w--)
            {
                m = (m & 0xFF00) | (*o & 0xFF);
                *o++ = p_map[m];
            }
        }
        else if ((lbDisplay.DrawFlags & Lb_SPRITE_TRANSPAR8) != 0)
        {
            m = colour;
            for (; w > 0; w--)
            {
                m = (m & 0x00FF) | ((*o << 8) & 0xFF00);
                *o++ = p_map[m];
            }
        }
        else
        {
            m = colour;
            memset(o, m, w);
        }
    }
}

static void LbI_RangeFillSingleRamp(struct DrawRange *ranges, int blt1width,
  int blt1xval, int blt1xinc, int blt1wval, int blt1winc)
{
    struct DrawRange *rng;

    rng = ranges;
    for (; blt1width > 0; blt1width--)
    {
        rng->x = blt1xval;
        blt1xval += blt1xinc;
        rng->w = blt1wval;
        blt1wval += blt1winc;
        rng++;
    }
}

static void LbI_RangeFillTwoRampsSmoothLeft(struct DrawRange *ranges, int blt1width,
  int blt1xval, int blt1xinc, int blt1wval, int blt1winc,
  int blt2width, int blt2wval, int blt2winc)
{
    struct DrawRange *rng;

    rng = ranges;
    for (; blt1width > 0; blt1width--)
    {
        rng->x = blt1xval;
        blt1xval += blt1xinc;
        rng->w = blt1wval;
        blt1wval += blt1winc;
        rng++;
    }
    for (; blt2width > 0; blt2width--)
    {
        rng->x = blt1xval;
        blt1xval += blt1xinc;
        rng->w = blt2wval;
        blt2wval += blt2winc;
        rng++;
    }
}

static void LbI_RangeFillTwoRampsSmoothRight(struct DrawRange *ranges, int blt1width,
  int blt1xval, int blt1xinc, int blt1wval, int blt1winc,
  int blt2width, int blt2xval, int blt2xinc)
{
    struct DrawRange *rng;

    rng = ranges;
    for (; blt1width > 0; blt1width--)
    {
        rng->x = blt1xval;
        blt1xval += blt1xinc;
        rng->w = blt1wval;
        blt1wval += blt1winc;
        rng++;
    }
    for (; blt2width > 0; blt2width--)
    {
        rng->x = blt2xval;
        blt2xval += blt2xinc;
        rng->w = blt1wval;
        blt1wval += blt1winc;
        rng++;
    }
}

// overflow flag of subtraction (x-y)
ubyte __OFSUBW__(short x, short y)
{
    int y2 = y;
    ubyte sx = (x < 0);
    return (sx ^ (y2 < 0)) & (sx ^ (x-y2 < 0));
}

void LbDrawTriangleFilled(short x1, short y1, short x2, short y2, short x3, short y3, TbPixel colour)
{
    //TODO make sure to test whether this works exactly like original, especially for large triangles
#if 0
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_LbDrawTriangleFilled\n"
        :  : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (x3), "g" (y3), "g" (colour));
    return;
#endif
  struct TbSPoint *p_pt1;
  struct TbSPoint *p_pt2;
  struct TbSPoint *p_pt3;
  int v12;
  int v20;
  int v24;
  ubyte v25;
  short v26;
  int v35;
  int v38;
  short v39;
  int v55;
  ubyte v65;
  TbBool pt2y_overflow;
  char blt2skip;
  TbBool pt3y_overflow;
  ubyte v70;
  TbBool pt1y_underflow;
  short v75;
  short v76;
  short v77;
  short v78;
  int v79;
  int v80;
  int v81;
  int wnd_h;
  int v84;
  int v85, v89;
    enum TriangleCase tricase;
    enum RampType ramp;
    short blt1width, blt2width;
    int blt1xval, blt2xval;
    int blt1xinc, blt2xinc;
    int blt1wval, blt2wval;
    int blt1winc, blt2winc;
    TbPixel *p_screen;
    struct TbSPoint pt3;
    struct TbSPoint pt2;
    struct TbSPoint pt1;

    pt1.x = x1;
    pt2.x = x2;
    pt3.x = x3;
    pt1.y = y1;
    pt2.y = y2;
    pt3.y = y3;

    if (pt1.y == pt2.y)
    {
        if (pt1.y == pt3.y)
            return;
        if (pt1.y < pt3.y)
        {
            if (pt2.x <= pt1.x)
                return;
            p_pt1 = &pt1;
            p_pt2 = &pt2;
            p_pt3 = &pt3;
            tricase = TriCase_Unkn92;
        }
        else
        {
            if (pt1.x <= pt2.x)
                return;
            p_pt1 = &pt3;
            p_pt2 = &pt1;
            p_pt3 = &pt2;
            tricase = TriCase_Unkn77;
        }
    }
    else if (pt1.y <= pt2.y)
    {
        if (pt1.y == pt3.y)
        {
            if (pt1.x <= pt3.x)
                return;
            p_pt1 = &pt3;
            p_pt2 = &pt1;
            p_pt3 = &pt2;
            tricase = TriCase_Unkn92;
        }
        else if (pt1.y >= pt3.y)
        {
            p_pt1 = &pt3;
            p_pt2 = &pt1;
            p_pt3 = &pt2;
            tricase = TriCase_Unkn24;
        }
        else if (pt2.y == pt3.y)
        {
            if (pt2.x <= pt3.x)
                return;
            p_pt1 = &pt1;
            p_pt2 = &pt2;
            p_pt3 = &pt3;
            tricase = TriCase_Unkn77;
        }
        else if (pt2.y <= pt3.y)
        {
            p_pt1 = &pt1;
            p_pt2 = &pt2;
            p_pt3 = &pt3;
            tricase = TriCase_Unkn24;
        }
        else
        {
            p_pt1 = &pt1;
            p_pt2 = &pt2;
            p_pt3 = &pt3;
            tricase = TriCase_Unkn50;
        }
    }
    else
    {
        if (pt1.y == pt3.y)
        {
            if (pt3.x <= pt1.x)
                return;
            p_pt1 = &pt2;
            p_pt2 = &pt3;
            p_pt3 = &pt1;
            tricase = TriCase_Unkn77;
        }
        else if (pt1.y < pt3.y)
        {
            p_pt1 = &pt2;
            p_pt2 = &pt3;
            p_pt3 = &pt1;
            tricase = TriCase_Unkn50;
        }
        else if (pt2.y == pt3.y)
        {
            if (pt3.x <= pt2.x)
                return;
            p_pt1 = &pt2;
            p_pt2 = &pt3;
            p_pt3 = &pt1;
            tricase = TriCase_Unkn92;
        }
        else if (pt2.y < pt3.y)
        {
            p_pt1 = &pt2;
            p_pt2 = &pt3;
            p_pt3 = &pt1;
            tricase = TriCase_Unkn24;
        }
        else
        {
            p_pt1 = &pt3;
            p_pt2 = &pt1;
            p_pt3 = &pt2;
            tricase = TriCase_Unkn50;
        }
    }

    switch (tricase)
    {
    case TriCase_Unkn50:
        if (p_pt1->y < 0)
        {
            p_screen = lbDisplay.WScreen;
            pt1y_underflow = 1;
        }
        else if (p_pt1->y < lbDisplay.GraphicsWindowHeight)
        {
            p_screen = &lbDisplay.WScreen[lbDisplay.GraphicsScreenWidth * p_pt1->y];
            pt1y_underflow = 0;
        }
        else
        {
            return;
        }
        v76 = p_pt1->y;
        blt2skip = p_pt3->y > lbDisplay.GraphicsWindowHeight;
        blt1width = p_pt3->y - p_pt1->y;
        pt2y_overflow = p_pt2->y > lbDisplay.GraphicsWindowHeight;
        wnd_h = p_pt2->y - p_pt1->y;
        blt1xinc = ((p_pt3->x - p_pt1->x) << 16) / blt1width;
        blt1winc = ((p_pt2->x - p_pt1->x) << 16) / wnd_h;
        if (blt1winc <= blt1xinc)
            return;
        blt2width = p_pt2->y - p_pt3->y;
        blt2xinc = ((p_pt2->x - p_pt3->x) << 16) / blt2width;
        v85 = p_pt3->x << 16;
        blt1xval = p_pt1->x << 16;
        blt1wval = blt1xval;
        if (!pt1y_underflow)
        {
            if (pt2y_overflow)
            {
                v38 = lbDisplay.GraphicsWindowHeight - v76;
                wnd_h = v38;
                if (blt2skip) {
                    blt1width = lbDisplay.GraphicsWindowHeight - v76;
                } else {
                    v25 = __OFSUBW__(v38, blt1width);
                    v39 = v38 - blt1width;
                    blt2skip = ((v39 < 0) ^ v25) | (v39 == 0);
                    blt2width = v39;
                }
            }
            blt2xval = v85;
        }
        else
        {
            int v82;

            v35 = -v76;
            wnd_h += v76;
            if (wnd_h <= 0)
                return;
            v81 = -v76;
            if (v35 - blt1width >= 0)
            {
                v82 = v81 - blt1width;
                blt2width -= v82;
                blt2xval = blt2xinc * v82 + v85;
                blt1wval += v82 * blt1winc + blt1width * blt1winc;
                if (pt2y_overflow)
                {
                    blt2width = lbDisplay.GraphicsWindowHeight;
                    wnd_h = lbDisplay.GraphicsWindowHeight;
                }
                blt1width = 0;
            }
            else
            {
                blt1width += v76;
                blt1xval += blt1xinc * v81;
                blt1wval += v81 * blt1winc;
                if (pt2y_overflow)
                {
                    wnd_h = lbDisplay.GraphicsWindowHeight;
                    if (blt2skip) {
                        blt1width = lbDisplay.GraphicsWindowHeight;
                    } else {
                        blt2skip = lbDisplay.GraphicsWindowHeight <= blt1width;
                        blt2width = lbDisplay.GraphicsWindowHeight - blt1width;
                    }
                }
                blt2xval = v85;
            }
        }
        if (blt2skip)
            blt2width = 0;
        ramp = Ramp_SmoothRight;
        break;

    case TriCase_Unkn77:
        if (p_pt1->y < 0)
        {
            p_screen = lbDisplay.WScreen;
            pt1y_underflow = 1;
        }
        else if (p_pt1->y < lbDisplay.GraphicsWindowHeight)
        {
            p_screen = &lbDisplay.WScreen[lbDisplay.GraphicsScreenWidth * p_pt1->y];
            pt1y_underflow = 0;
        }
        else
        {
            return;
        }
        v77 = p_pt1->y;
        pt3y_overflow = p_pt3->y > lbDisplay.GraphicsWindowHeight;
        blt1width = p_pt3->y - p_pt1->y;
        wnd_h = blt1width;
        blt1xinc = ((p_pt3->x - p_pt1->x) << 16) / blt1width;
        blt1winc = ((p_pt2->x - p_pt1->x) << 16) / blt1width;
        blt1xval = p_pt1->x << 16;
        blt1wval = blt1xval;
        if (!pt1y_underflow)
        {
            if (pt3y_overflow)
            {
                wnd_h = lbDisplay.GraphicsWindowHeight - v77;
                blt1width = lbDisplay.GraphicsWindowHeight - v77;
            }
        }
        else
        {
            int v46;

            v46 = -v77;
            blt1width += v77;
            wnd_h += v77;
            if (wnd_h <= 0)
                return;
            blt1xval += blt1xinc * v46;
            blt1wval += v46 * blt1winc;
            if (pt3y_overflow)
            {
                wnd_h = lbDisplay.GraphicsWindowHeight;
                blt1width = lbDisplay.GraphicsWindowHeight;
            }
        }
        ramp = Ramp_Single;
        break;

    case TriCase_Unkn24:
        if (p_pt1->y < 0)
        {
            p_screen = lbDisplay.WScreen;
            pt1y_underflow = 1;
        }
        else if (p_pt1->y < lbDisplay.GraphicsWindowHeight)
        {
            p_screen = &lbDisplay.WScreen[lbDisplay.GraphicsScreenWidth * p_pt1->y];
            pt1y_underflow = 0;
        }
        else
        {
            return;
        }
        v75 = p_pt1->y;
        v12 = p_pt3->y;
        v65 = p_pt3->y > lbDisplay.GraphicsWindowHeight;
        v89 = p_pt3->y - p_pt1->y;
        wnd_h = v89;
        blt2skip = p_pt2->y > lbDisplay.GraphicsWindowHeight;
        blt1width = p_pt2->y - p_pt1->y;
        blt1xinc = ((p_pt3->x - p_pt1->x) << 16) / v89;
        blt1winc = ((p_pt2->x - p_pt1->x) << 16) / v12;
        if (blt1winc <= blt1xinc)
            return;
        blt2winc = ((p_pt3->x - p_pt2->x) << 16) / v12;
        blt2width = p_pt3->y - p_pt2->y;
        v84 = p_pt2->x << 16;
        blt1xval = p_pt1->x << 16;
        blt1wval = blt1xval;
        if (!pt1y_underflow)
        {
            if (v65)
            {
                v24 = lbDisplay.GraphicsWindowHeight - v75;
                wnd_h = v24;
                if (blt2skip) {
                    blt1width = v24;
                } else {
                    v25 = __OFSUBW__(v24, blt1width);
                    v26 = v24 - blt1width;
                    blt2skip = ((v26 < 0) ^ v25) | (v26 == 0);
                    blt2width = v26;
                }
            }
            blt2wval = v84;
        }
        else
        {
            v20 = -v75;
            wnd_h += v75;
            if (wnd_h <= 0)
                return;
            v79 = -v75;
            if (v20 - blt1width >= 0)
            {
                blt2width -= v79 - blt1width;
                v80 = v79 - blt1width;
                blt1xval += blt1xinc * v80 + blt1width * blt1xinc;
                blt2wval = blt2winc * v80 + v84;
                if (v65)
                {
                    blt2width = lbDisplay.GraphicsWindowHeight;
                    wnd_h = lbDisplay.GraphicsWindowHeight;
                }
                blt1width = 0;
            }
            else
            {
                blt1width += v75;
                blt1xval += blt1xinc * v79;
                blt1wval += v79 * blt1winc;
                if (v65)
                {
                    wnd_h = lbDisplay.GraphicsWindowHeight;
                    if (blt2skip) {
                        blt1width = lbDisplay.GraphicsWindowHeight;
                    } else {
                        blt2skip = lbDisplay.GraphicsWindowHeight <= blt1width;
                        blt2width = lbDisplay.GraphicsWindowHeight - blt1width;
                    }
                }
                blt2wval = v84;
            }
        }
        if (blt2skip)
            blt2width = 0;
        ramp = Ramp_SmoothLeft;
        break;

    case TriCase_Unkn92:
        if (p_pt1->y < 0)
        {
            p_screen = lbDisplay.WScreen;
            pt1y_underflow = 1;
        }
        else if (p_pt1->y < lbDisplay.GraphicsWindowHeight)
        {
            p_screen = &lbDisplay.WScreen[lbDisplay.GraphicsScreenWidth * p_pt1->y];
            pt1y_underflow = 0;
        }
        else
        {
            return;
        }
        v78 = p_pt1->y;
        v70 = p_pt3->y > lbDisplay.GraphicsWindowHeight;
        blt1width = p_pt3->y - p_pt1->y;
        wnd_h = blt1width;
        blt1xinc = ((p_pt3->x - p_pt1->x) << 16) / blt1width;
        blt1winc = ((p_pt3->x - p_pt2->x) << 16) / blt1width;
        blt1xval = p_pt1->x << 16;
        blt1wval = p_pt2->x << 16;
        if (!pt1y_underflow)
        {
            if (v70)
            {
                wnd_h = lbDisplay.GraphicsWindowHeight - v78;
                blt1width = lbDisplay.GraphicsWindowHeight - v78;
            }
        }
        else
        {
            v55 = -v78;
            blt1width += v78;
            wnd_h += v78;
            if (wnd_h <= 0)
                return;
            blt1xval += v55 * blt1xinc;
            blt1wval += v55 * blt1winc;
            if (v70)
            {
                wnd_h = lbDisplay.GraphicsWindowHeight;
                blt1width = lbDisplay.GraphicsWindowHeight;
            }
        }
        ramp = Ramp_Single;
        break;
    }

    // Fill the ranges array with two areas
    switch (ramp)
    {
    case Ramp_SmoothLeft:
        LbI_RangeFillTwoRampsSmoothLeft(byte_1E957C, blt1width, blt1xval,
          blt1xinc, blt1wval, blt1winc, blt2width, blt2wval, blt2winc);
        break;
    case Ramp_SmoothRight:
        LbI_RangeFillTwoRampsSmoothRight(byte_1E957C, blt1width, blt1xval,
          blt1xinc, blt1wval, blt1winc, blt2width, blt2xval, blt2xinc);
        break;
    default:
        LbI_RangeFillSingleRamp(byte_1E957C, blt1width, blt1xval,
          blt1xinc, blt1wval, blt1winc);
        break;
    }
    // Draw on graphics buffer
    p_screen += lbDisplay.GraphicsWindowX + lbDisplay.GraphicsScreenWidth * (lbDisplay.GraphicsWindowY - 1);
    LbBlitSolidRanges(p_screen, byte_1E957C, wnd_h, colour);
}

void LbDrawTriangle(short x1, short y1, short x2, short y2, short x3, short y3, TbPixel colour)
{
    if (lbDisplay.DrawFlags & Lb_SPRITE_OUTLINE)
    {
        LbDrawTriangleOutline(x1, y1, x2, y2, x3, y3, colour);
    }
    else
    {
        LbDrawTriangleFilled(x1, y1, x2, y2, x3, y3, colour);
    }
}

void LbDrawSlantBox(short X, short Y, ushort a3, ushort a4)
{
    asm volatile (
      "call ASM_LbDrawSlantBox\n"
        :  : "a" (X), "d" (Y), "b" (a3), "c" (a4));
}

/******************************************************************************/
