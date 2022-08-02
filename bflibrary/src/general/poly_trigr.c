/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file poly_trigr.c
 *     Textured 2D triangle rendring, final rasterization modes.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <string.h>
#include "poly_trigr.h"
#include "poly.h"
#include "bfgentab.h"
#include "poly_trigp.h"
#include "privbflog.h"

// TODO Switch to local var when remade - this is global because assembly alters EBP
extern struct TrigLocals lv;

/**
 * whether the subtraction (x-y) of two short ints would overflow
 */
static inline ubyte __OFSUBS__(short x, short y)
{
    return ((x < 0) ^ (y < 0)) & ((x < 0) ^ (x-y < 0));
}

/**
 * whether the addition (x+y) of two short ints would use carry
 */
static inline ubyte __CFADDS__(short x, short y)
{
    return (ushort)(x) > (ushort)(x+y);
}

/**
 * whether the addition (x+y) of two long ints would use carry
 */
static inline ubyte __CFADDL__(long x, long y)
{
    return (ulong)(x) > (ulong)(x+y);
}

/**
 * rotate left unsigned long
 */
static inline ulong __ROL4__(ulong value, int count)
{
    const uint nbits = 4 * 8;

    if (count > 0) {
        count %= nbits;
        ulong high = value >> (nbits - count);
        value <<= count;
        value |= high;
    } else {
        count = -count % nbits;
        ulong low = value << (nbits - count);
        value >>= count;
        value |= low;
    }
    return value;
}

void trig_render_md00(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *o_ln;
    ubyte col;
    short pX, pY;
    ubyte *o;

    pp = polyscans;
    o_ln = lv.var_24;
    col = vec_colour;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o_ln += vec_screen_width;
        if (pX < 0)
        {
            if (pY <= 0)
                continue;
            if (pY > vec_window_width)
                pY = vec_window_width;
            o = &o_ln[0];
        }
        else
        {
            TbBool pY_overflow;
            if (pY > vec_window_width)
                pY = vec_window_width;
            pY_overflow = __OFSUBS__(pY, pX);
            pY = pY - pX;
            if (((pY < 0) ^ pY_overflow) | (pY == 0))
                continue;
            o = &o_ln[pX];
        }
        memset(o, col, pY);
    }
}

void trig_render_md01(struct TrigLocals *lvu)
{
    short pS;
    TbBool pS_carry;
    struct PolyPoint *pp;
    ushort colS;

    pp = polyscans;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pX, pY;
        ubyte *o;

        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;

        if (pX  < 0)
        {
            long mX;
            short colH;

            if (pY <= 0)
                continue;
            mX = lv.var_60 * (ushort)(-pX);
            pS_carry = __CFADDS__(pp->S, mX);
            pS = pp->S + mX;
            // Delcate code - if we add before shifting, the result is different
            colH = (mX >> 16) + (pp->S >> 16) + pS_carry;
            if (pY > vec_window_width)
                pY = vec_window_width;

            colS = ((colH & 0xFF) << 8) + vec_colour;
        }
        else
        {
            TbBool pY_overflow;
            short colH;

            if (pY > vec_window_width)
              pY = vec_window_width;
            pY_overflow = __OFSUBS__(pY, pX);
            pY = pY - pX;
            if (((pY < 0) ^ pY_overflow) | (pY == 0))
                continue;
            o += pX;
            colH = pp->S >> 16;
            pS = pp->S;

            colS = ((colH & 0xFF) << 8) + vec_colour;
        }

        for (;pY > 0; pY--, o++)
        {
            short colH, colL;

            *o = colS >> 8;

            colL = colS;
            pS_carry = __CFADDS__(lv.var_60, pS);
            pS = lv.var_60 + pS;
            colH = (lv.var_60 >> 16) + pS_carry + (colS >> 8);

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md02(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *o;
    ushort colS;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pX, pY;
        long pU;

        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;

        if (pX < 0)
        {
            ushort colL, colH;
            ulong factorA;
            long mX;

            if (pY <= 0)
                continue;
            mX = lv.var_54 * (-pX);
            factorA = __ROL4__(pp->V + mX, 16);
            colH = factorA;
            mX = lv.var_48 * (-pX);
            pU = (factorA & 0xFFFF0000) | ((pp->U + mX) & 0xFFFF);
            colL = (pp->U + mX) >> 16;
            if (pY > vec_window_width)
                pY = vec_window_width;
            pX = (pp->U + mX) >> 8;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            short colL, colH;
            TbBool pY_overflow;

            if (pY > vec_window_width)
                pY = vec_window_width;
            pY_overflow = __OFSUBS__(pY, pX);
            pY = pY - pX;
            if (((pY < 0) ^ pY_overflow) | (pY == 0))
                continue;
            o += pX;
            pU = __ROL4__(pp->V, 16);
            colH = pU;
            pU = (pU & 0xFFFF0000) | (pp->U & 0xFFFF);
            colL = pp->U >> 16;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;

        for (; pY > 0; pY--, o++)
        {
            short colL, colH;
            TbBool pU_carry;

            *o = m[colS];

            pU_carry = __CFADDS__(lv.var_48, pU);
            pU = (pU & 0xFFFF0000) | ((lv.var_48 + pU) & 0xFFFF);
            colL = (lv.var_48 >> 16) + pU_carry + colS;

            pU_carry = __CFADDL__(lv.var_70, pU);
            pU = lv.var_70 + pU;
            colH = (lv.var_54 >> 16) + pU_carry + (colS >> 8);

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md03(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *o;
    ushort colS;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pX, pY;
        ulong factorA;
        long pU;

        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;

        if (pX < 0)
        {
            short colL, colH;
            long mX;

            if (pY <= 0)
                continue;
            mX = lv.var_54 * (-pX);
            factorA = __ROL4__(pp->V + mX, 16);
            colH = factorA;
            mX = lv.var_48 * (-pX);
            pU = (factorA & 0xFFFF0000) | ((pp->U + mX) & 0xFFFF);
            colL = (pp->U + mX) >> 16;
            if (pY > vec_window_width)
                pY = vec_window_width;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            short colL, colH;
            TbBool pY_overflow;

            if (pY > vec_window_width)
                pY = vec_window_width;
            pY_overflow = __OFSUBS__(pY, pX);
            pY = pY - pX;
            if (((pY < 0) ^ pY_overflow) | (pY == 0))
                continue;
            o += pX;
            pU = __ROL4__(pp->V, 16);
            colH = pU;
            pU = (pU & 0xFFFF0000) | ((pp->U) & 0xFFFF);
            colL = (pp->U) >> 16;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;

        for (; pY > 0; pY--, o++)
        {
            short colL, colH;
            TbBool pU_carry;

            if (m[colS] != 0)
                *o = m[colS];

            pU_carry = __CFADDS__(lv.var_48, pU);
            pU = (pU & 0xFFFF0000) | ((lv.var_48 + pU) & 0xFFFF);
            colL = (lv.var_48 >> 16) + pU_carry + colS;
            pU_carry = __CFADDL__(lv.var_70, pU);
            pU = lv.var_70 + pU;
            colH = (lv.var_54 >> 16) + pU_carry + (colS >> 8);

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

/**
 * Verified in swars - uses:
 * - zealot car antennas
 * - tank lower chassis
 * - Large red and white rocket building - red stipes
 */
void trig_render_md04(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *f;
    ubyte *o;
    ushort colS;

    pp = polyscans;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pX, pY;
        short pU;

        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pX < 0)
        {
            ushort colL, colH;
            TbBool pU_carry;
            long mX;

            if (pY <= 0)
                continue;
            mX = lv.var_60 * (-pX);
            pU_carry = __CFADDS__(pp->S, mX);
            pU = pp->S + mX;
            colH = (pp->S >> 16) + pU_carry + (mX >> 16);
            if (pY > vec_window_width)
                pY = vec_window_width;
            colL = vec_colour;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            TbBool pY_overflow;

            if (pY > vec_window_width)
                pY = vec_window_width;
            pY_overflow = __OFSUBS__(pY, pX);
            pY = pY - pX;
            if (((pY < 0) ^ pY_overflow) | (pY == 0))
                continue;
            o += pX;
            colL = vec_colour;
            pU = pp->S;
            colH = pp->S >> 16;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        f = pixmap.fade_table;

        for (;pY > 0; pY--, o++)
        {
            ushort colL, colH;
            TbBool pU_carry;

            pU_carry = __CFADDS__(lv.var_60, pU);
            pU = lv.var_60 + pU;
            colL = colS;
            colH = (lv.var_60 >> 16) + pU_carry + (colS >> 8);
            *o = f[colS];

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md05(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *f;
    ubyte *o;
    ubyte *o_ln;
    long rfactA, rfactB;

    pp = polyscans;
    {
        ulong factorA, factorB, factorC;
        factorC = lv.var_48;
        // original code used unsigned compare here, making the condition always false
        //if (lv.var_60 < 0) factorC--;
        factorC = __ROL4__(factorC, 16);
        factorA = __ROL4__(lv.var_54, 16);
        factorB = ((ulong)lv.var_60) >> 8;
        lv.var_70 = (factorC & 0xFFFF0000) | (factorB & 0xFFFF);
        lv.var_74 = (factorA & 0xFFFFFF00) | (factorC & 0xFF);
        lv.var_88 = (factorA & 0xFF);
    }

    for (; lv.var_44; lv.var_44--, pp++)
    {
        long pX, pY;
        ushort colM;

        pX = pp->X >> 16;
        pY = pp->Y >> 16;
        o_ln = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;

        if (pX < 0)
        {
            ulong factorA, factorB;
            ushort colL, colH;
            long mX;

            if (pY <= 0)
                continue;
            mX = lv.var_48 * (-pX);
            factorA = __ROL4__(pp->U + mX, 16);
            mX = lv.var_54 * (-pX);
            factorB = __ROL4__(pp->V + mX, 16);
            mX = lv.var_60 * (-pX);
            colL = (pp->S + mX) >> 8;
            colH = factorB;
            rfactB = (factorB & 0xFFFF0000) | (factorA & 0xFF);
            rfactA = (factorA & 0xFFFF0000) | (colL & 0xFFFF);
            if (pY > vec_window_width)
                pY = vec_window_width;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ulong factorA, factorB;
            ushort colL, colH;
            TbBool pY_overflow;

            if (pY > vec_window_width)
                pY = vec_window_width;
            pY_overflow = __OFSUBS__(pY, pX);
            pY = pY - pX;
            if (((pY < 0) ^ pY_overflow) | (pY == 0))
                continue;
            o_ln += pX;
            factorA = __ROL4__(pp->U, 16);
            factorB = __ROL4__(pp->V, 16);
            colL = pp->S >> 8;
            colH = factorB;
            // Should the high part really be preserved?
            rfactB = (factorB & 0xFFFF0000) | (factorA & 0xFF);
            rfactA = (factorA & 0xFFFF0000) | (colL & 0xFFFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        o = o_ln;
        m = vec_map;
        f = pixmap.fade_table;

        for (; pY > 0; pY--, o++)
        {
            ushort colL, colH;
            ushort colS;
            TbBool rfactA_carry;
            TbBool rfactB_carry;

            colM = (colM & 0xFF00) + (rfactB & 0xFF);
            colS = (((rfactA >> 8) & 0xFF) << 8) + m[colM];

            rfactA_carry = __CFADDL__(rfactA, lv.var_70);
            rfactA = rfactA + lv.var_70;

            rfactB_carry = __CFADDL__(rfactB + rfactA_carry, lv.var_74);
            rfactB = rfactB + lv.var_74 + rfactA_carry;

            colH = lv.var_88 + rfactB_carry + (colM >> 8);
            colL = colM;
            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);

            *o = f[colS];
        }
    }
}

/**
 * Verified in swars - uses:
 * - wire fences
 * - cross bars
 */
void trig_render_md06(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ushort colM;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    lv.var_74 = lv.var_60 << 16;
    colM = 0;

    for (; lv.var_44; lv.var_44--, pp++)
    {
        ubyte *m;
        ubyte *f;
        ubyte *o;
        short pXa, pYa;
        long factorA;
        long pY;
        ulong factorB;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;

        if (pXa < 0)
        {
            ushort colL, colH;
            ushort pXMa;
            long pXMb;
            ulong mX;

            if (pYa <= 0)
                continue;
            pXMa = (ushort)-pXa;
            pXMb = pXMa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXMa, 16);
            colH = factorA;
            mX = pp->U + lv.var_48 * pXMa;
            factorA = (factorA & 0xFFFF0000) | (mX & 0xFFFF);
            pXa = mX >> 8;
            colL = (pXa >> 8);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);

            factorB = __ROL4__(pp->S + lv.var_60 * pXMb, 16);
            pXa = (pXa & 0xFFFF00FF) | ((factorB & 0xFF) << 8);
            factorB = (factorB & 0xFFFF0000) | (pYa & 0xFFFF);
            pXa = (pXa & 0xFFFF);
            pY = factorB & 0xFFFF;
            if (pY > vec_window_width)
                pY = vec_window_width;
        }
        else
        {
            ushort colL, colH;
            ubyte pLa_overflow;
            short pLa;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pLa_overflow = __OFSUBS__(pYa, pXa);
            pLa = pYa - pXa;
            if (((pLa < 0) ^ pLa_overflow) | (pLa == 0))
                continue;

            o += pXa;
            colL = (pp->U >> 16);
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorB = __ROL4__(pp->S, 16);
            factorA = (factorA & 0xFFFF0000) | (pp->U & 0xFFFF);
            pXa = (pXa & 0xFFFF00FF) | ((factorB & 0xFF) << 8);
            factorB = (factorB & 0xFFFF0000) | (pLa & 0xFFFF);
            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
            pY = factorB & 0xFFFF;
        }

        m = vec_map;
        f = pixmap.fade_table;

        for (; (pY & 0xFFFF) > 0; pY--, o++)
        {
            ushort colL, colH;
            ubyte fct_carry;

            pXa = (pXa & 0xFF00) | (m[colM] & 0xFF);
            if (pXa & 0xFF)
                *o = f[pXa];

            fct_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) | ((lv.var_48 + factorA) & 0xFFFF);
            colL = (lv.var_48 >> 16) + fct_carry + colM;
            fct_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (lv.var_54 >> 16) + fct_carry + (colM >> 8);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);

            factorB = (factorB & 0xFFFF0000) | (pY & 0xFFFF);
            fct_carry = __CFADDL__(lv.var_74, factorB);
            factorB += lv.var_74;
            pXa = (((pXa >> 8) + (lv.var_60 >> 16) + fct_carry) << 8) | (pXa & 0xFF);
            pY += lv.var_74; // Very alarming. Bug, maybe?
        }
    }
}

void trig_render_md07(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    short pXa;
    ushort colM;
    long pYa;
    ubyte *o;
    long pXm;
    long factorA;
    ubyte *m;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if ( (short)pYa <= 0 )
                continue;
            pXm = (ushort)-(short)pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorC = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) | (factorC & 0xFFFF);
            factorB = factorC >> 8;
            colL = ((factorB >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorB;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) | (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colS = (vec_colour << 8) + m[colM];
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) | ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            *o = f[colS];
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md08(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    short pXa;
    ushort colM;
    long pYa;
    ubyte *o;
    long factorA;
    ubyte *m;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong factorB, factorC;
            long pXm;

            if ( (short)pYa <= 0 )
                continue;
            pXm = (ushort)-(short)pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorC;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colS = (vec_colour << 8) + m[colM];
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            if (colS & 0xFF)
                *o = f[colS];
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md09(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    short pXa;
    ushort colM;
    long factorA;
    short pYa;
    ubyte *o;
    long pXm;
    ubyte *m;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if ( (short)pYa <= 0 )
                continue;
            pXm = (ushort)-(short)pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorC;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colS = m[colM] << 8;
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            if ((colS >> 8) & 0xFF) {
                colS = (colS & 0xFF00) | (*o);
                *o = f[colS];
            }
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md10(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    short pXa;
    ushort colM;
    short pYa;
    ubyte *o;
    ulong factorB;
    long factorA;
    ulong factorC;
    ubyte *m;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            long pXm;

            if ( (short)pYa <= 0 )
                continue;
            pXm = (ushort)-(short)pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorC;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            if (m[colM]) {
                colS = (vec_colour << 8) | (*o);
                *o = f[colS];
            }
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md12(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    long pXa;
    ushort colM;
    short pYa;
    ubyte *o;
    long pXm;
    long factorA;
    ubyte *m;
    ubyte *g;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if ( (short)pYa <= 0 )
                continue;
            pXm = (ushort)-(short)pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorC = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorC & 0xFFFF);
            factorB = factorC >> 8;
            colL = ((factorB >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorB;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colS = (m[colM] << 8) | vec_colour;
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            *o = g[colS];
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md13(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    short pXa;
    ushort colM;
    short pYa;
    ubyte *o;
    long pXm;
    long factorA;
    ubyte *m;
    ubyte *g;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    colM = 0;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if ( (short)pYa <= 0 )
                continue;
            pXm = (ushort)-(short)pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorC;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( (ubyte)(((pYa & 0x8000u) != 0) ^ pY_overflow) | ((ushort)pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colS = m[colM] | (vec_colour << 8);
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            *o = g[colS];
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md14(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *o_ln;
    ushort colM;
    short pXa;
    short pYa;
    ubyte *o;
    ubyte *g;

    pp = polyscans;
    o_ln = lv.var_24;
    colM = (vec_colour << 8);

    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o_ln += vec_screen_width;

        if (pXa < 0)
        {
            if (pYa <= 0)
                continue;
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            o = o_ln;
        }
        else
        {
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o = &o_ln[pXa];
        }

        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
              colM = (colM & 0xFF00) | *o;
              *o = g[colM];
        }
    }
}

void trig_render_md15(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *o_ln;
    ushort colM;
    short pXa;
    short pYa;
    ubyte *o;
    ubyte *g;

    pp = polyscans;
    o_ln = lv.var_24;
    colM = vec_colour;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o_ln += vec_screen_width;
        if (pXa < 0)
        {
            if (pYa <= 0)
                continue;
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            o = o_ln;
        }
        else
        {
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o = &o_ln[pXa];
        }

        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
              colM = (*o << 8) | (colM & 0xFF);
              *o = g[colM];
        }
    }
}

void trig_render_md16(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    short pXa;
    short pYa;
    ushort colM;
    ubyte *o;
    ubyte *g;
    ubyte *f;
    short factorA;

    pp = polyscans;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;

        if (pXa < 0)
        {
            ushort colL, colH;
            ubyte factorA_carry;
            ulong pXMa;
            short pXMb;

            if (pYa <= 0)
                continue;
            pXMa = lv.var_60 * (ushort)-pXa;
            pXMb = pXMa;
            pXa = pXMa >> 8;
            factorA_carry = __CFADDS__(pp->S, pXMb);
            factorA = (pp->S) + pXMb;
            colH = (pXa >> 8) + (pp->S >> 16) + factorA_carry;
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            colL = vec_colour;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            colL = vec_colour;
            factorA = pp->S;
            colH = (pp->S >> 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colS = (f[colM] << 8) | *o;
            *o = g[colS];
            factorA_carry = __CFADDS__(lv.var_60, factorA);
            factorA += (lv.var_60 & 0xFFFF);
            colH = (colM >> 8) + (lv.var_60 >> 16) + factorA_carry;
            colL = colM;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md17(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *o;
    ubyte *g;
    ubyte *f;
    ubyte factorA_carry;
    short factorA;

    pp = polyscans;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa, pYa;
        ushort colS;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;

        if (pXa < 0)
        {
            ushort colL, colH;
            ulong pXMa;
            short pXMb;

            if ( (short)pYa <= 0 )
                continue;
            pXMa = lv.var_60 * (ushort)-pXa;
            pXMb = pXMa;
            pXa = pXMa >> 8;
            factorA_carry = __CFADDS__(pp->S, pXMb);
            factorA = pp->S + pXMb;
            colH = (pXa >> 8) + (pp->S >> 16) + factorA_carry;
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            colL = vec_colour;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if (((pYa < 0) ^ pY_overflow) | (pYa == 0))
                continue;

            o += pXa;
            colL = vec_colour;
            factorA = pp->S;
            colH = (pp->S >> 16);

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colM;

            colM = ((*o) << 8) + f[colS];
            *o = g[colM];

            factorA_carry = __CFADDS__(lv.var_60, factorA);
            factorA += (lv.var_60 & 0xFFFF);
            colH = (colS >> 8) + ((lv.var_60 >> 16) & 0xFF) + factorA_carry;
            colL = colS;

            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md18(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    short pXa;
    ushort colM;
    short pYa;
    ubyte *o;
    long factorA;
    ubyte *m;
    ubyte *g;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        long pXm;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if (pYa <= 0)
                continue;
            pXm = (ushort)-pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = (factorC >> 8);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorC;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_carry;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_carry = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_carry) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = (pp->U >> 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            colH = m[colM];
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) | ((lv.var_48 + factorA) & 0xFFFF);
            colL = *o;
            colS = ((colH & 0xFF) << 8) + (colL & 0xFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            *o = g[colS];
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md19(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        long factorA;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            long pXm;
            ulong factorB, factorC;

            if (pYa <= 0)
                continue;
            pXm = (ushort)-pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colS = ((*o) << 8) + m[colM];
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            *o = g[colS];
            colH = (colM >> 8) + (lv.var_54 >> 16) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md20(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    lv.var_74 = lv.var_60 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        long pXMa;
        long pXMb;
        long factorA;
        long factorC;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            ulong factorB;

            if (pYa <= 0)
                continue;
            if (pYa > vec_window_width)
                pYa = vec_window_width;
            lv.var_7C = pYa;
            pXMa = (ushort)-pXa;
            pXMb = pXMa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXMa, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXMa;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            pXa = factorB >> 8;
            colL = ((pXa >> 8) & 0xFF);
            factorC = __ROL4__(pp->S + lv.var_60 * pXMb, 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);
            lv.var_7C = pYa;
            factorC = __ROL4__(pp->S, 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; lv.var_7C > 0; lv.var_7C--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colS = ((factorC & 0xFF) << 8) + m[colM];
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colS = ((f[colS] & 0xFF) << 8) + *o;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;
            factorA_carry = __CFADDL__(lv.var_74, factorC);
            factorC += lv.var_74;
            *o = g[colS];
            factorC = (factorC & 0xFFFFFF00) | (((lv.var_60 >> 16) + factorA_carry + factorC) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md21(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *m;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    lv.var_74 = lv.var_60 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        ubyte *g;
        ubyte *f;
        long factorA, factorC;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            long pXMa;
            long pXMb;
            ulong factorB;

            if (pYa <= 0)
                continue;
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            lv.var_7C = pYa;
            pXMa = (ushort)-pXa;
            pXMb = pXMa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXMa, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXMa;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            pXa = factorB >> 8;
            colL = ((pXa >> 8) & 0xFF);
            factorC = __ROL4__(pp->S + lv.var_60 * pXMb, 16);
            pXa = (pXa & 0xFFFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);
            lv.var_7C = pYa;
            factorC = __ROL4__(pp->S, 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; lv.var_7C > 0; lv.var_7C--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            colS = ((factorC & 0xFF) << 8) + (m[colM] & 0xFF);
            colS = (((*o) & 0xFF) << 8) + (f[colS] & 0xFF);
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;
            factorA_carry = __CFADDL__(lv.var_74, factorC);
            factorC += lv.var_74;
            *o = g[colS];
            factorC = (factorC & 0xFFFFFF00) | (((lv.var_60 >> 16) + factorA_carry + factorC) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md22(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        long pXm;
        long factorA;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if (pYa <= 0)
                continue;
            pXm = (ushort)-pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = factorC & 0xFFFF;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if ( ((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;
            ubyte factorA_carry;

            if (m[colM]) {
                colS = ((m[colM] & 0xFF) << 8) + *o;
                *o = g[colS];
            }
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md23(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        long pXm;
        long factorA;
        ubyte factorA_carry;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if ( (pXa & 0x8000u) != 0 )
        {
            ushort colL, colH;
            ulong factorB, factorC;

            if (pYa <= 0)
                continue;
            pXm = (ushort)-pXa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXm, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXm;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            factorC = factorB >> 8;
            colL = ((factorC >> 8) & 0xFF);
            if (pYa > vec_window_width)
              pYa = vec_window_width;
            pXa = (ushort)factorC;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if (((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colL, colH;
            ushort colS;

            if (m[colM]) {
                colS = (((*o) & 0xFF) << 8) + m[colM];
                *o = g[colS];
            }
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md24(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    lv.var_74 = lv.var_60 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        long pXMa;
        long pXMb;
        long factorA;
        long factorC;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            ulong factorB;

            if (pYa <= 0)
                continue;
            if (pYa > vec_window_width)
                pYa = vec_window_width;
            lv.var_7C = pYa;
            pXMa = (ushort)-pXa;
            pXMb = pXMa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXMa, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXMa;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            pXa = factorB >> 8;
            colL = ((pXa >> 8) & 0xFF);
            factorC = __ROL4__(pp->S + lv.var_60 * pXMb, 16);
            pXa = (ushort)pXa;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if (((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);
            lv.var_7C = pYa;
            factorC = __ROL4__(pp->S, 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; lv.var_7C > 0; lv.var_7C--, o++)
        {
            ushort colL, colH;
            ubyte factorA_carry;

            if (m[colM]) {
                ushort colS;

                colS = ((factorC & 0xFF) << 8) + m[colM];
                colS = (f[colS] << 8) + *o;
                *o = g[colS];
            }
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;
            factorA_carry = __CFADDL__(lv.var_74, factorC);
            factorC += lv.var_74;
            factorC = (factorC & 0xFFFFFF00) + (((lv.var_60 >> 16) + factorA_carry + factorC) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md25(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;
    ubyte *f;

    pp = polyscans;
    lv.var_70 = lv.var_54 << 16;
    lv.var_74 = lv.var_60 << 16;
    for (; lv.var_44; lv.var_44--, pp++)
    {
        short pXa;
        ushort colM;
        short pYa;
        ubyte *o;
        long pXMa;
        long pXMb;
        long factorA;
        long factorC;

        pXa = (pp->X >> 16);
        pYa = (pp->Y >> 16);
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;
        if (pXa < 0)
        {
            ushort colL, colH;
            ulong factorB;

            if (pYa <= 0)
                continue;
            if (pYa > vec_window_width)
                pYa = vec_window_width;
            lv.var_7C = pYa;
            pXMa = (ushort)-pXa;
            pXMb = pXMa;
            factorA = __ROL4__(pp->V + lv.var_54 * pXMa, 16);
            colH = factorA;
            factorB = pp->U + lv.var_48 * pXMa;
            factorA = (factorA & 0xFFFF0000) + (factorB & 0xFFFF);
            pXa = factorB >> 8;
            colL = ((pXa >> 8) & 0xFF);
            factorC = __ROL4__(pp->S + lv.var_60 * pXMb, 16);
            pXa = (ushort)pXa;

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
        else
        {
            ushort colL, colH;
            ubyte pY_overflow;

            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa = pYa - pXa;
            if (((pYa < 0) ^ pY_overflow) | (pYa == 0) )
                continue;
            o += pXa;
            factorA = __ROL4__(pp->V, 16);
            colH = factorA;
            factorA = (factorA & 0xFFFF0000) + (pp->U & 0xFFFF);
            colL = ((pp->U >> 16) & 0xFF);
            lv.var_7C = pYa;
            factorC = __ROL4__(pp->S, 16);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }

        m = vec_map;
        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; lv.var_7C > 0; lv.var_7C--, o++)
        {
            ushort colL, colH;
            ubyte factorA_carry;

            if (m[colM]) {
                ushort colS;

                colS = ((factorC & 0xFF) << 8) + m[colM];
                colS = (((*o) & 0xFF) << 8) + f[colS];
                *o = g[colS];
            }
            factorA_carry = __CFADDS__(lv.var_48, factorA);
            factorA = (factorA & 0xFFFF0000) + ((lv.var_48 + factorA) & 0xFFFF);
            colL = ((lv.var_48 >> 16) & 0xFF) + factorA_carry + colM;
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA += lv.var_70;
            colH = (colM >> 8) + ((lv.var_54 >> 16) & 0xFF) + factorA_carry;
            factorA_carry = __CFADDL__(lv.var_74, factorC);
            factorC += lv.var_74;
            factorC = (factorC & 0xFFFFFF00) | (((lv.var_60 >> 16) + factorA_carry + factorC) & 0xFF);

            colM = ((colH & 0xFF) << 8) + (colL & 0xFF);
        }
    }
}

void trig_render_md26(struct TrigLocals *lvu)
{
    struct PolyPoint *pp;
    ubyte *m;
    ubyte *g;
    ubyte *f;

    pp = polyscans;
    lv.var_80 = pp;
    {
        ulong v1;
        ulong v2;
        ubyte v3;

        v1 = __ROL4__(lv.var_48, 16);
        v2 = __ROL4__(lv.var_54, 16);
        v3 = v2;
        v2 = (v2 & 0xFFFFFF00) + (v1 & 0xFF);
        v1 = (v1 & 0xFFFF0000) + (((ulong)lv.var_60 >> 8) & 0xFFFF);
        v2 = (v2 & 0xFFFF0000) + (v2 & 0xFF);
        lv.var_70 = v1;
        lv.var_74 = v2;
        lv.var_88 = v3;
    }
    for (; lv.var_44; lv.var_44--)
    {
        long pXa;
        long pYa;
        ubyte *o;
        ulong factorB, factorD;
        long factorA;
        ulong factorC;
        ubyte pY_overflow;
        ushort colM;

        pp = lv.var_80;
        lv.var_80 += 20;
        pXa = pp->X >> 16;
        pYa = pp->Y >> 16;
        o = &lv.var_24[vec_screen_width];
        lv.var_24 += vec_screen_width;

        if (pXa < 0)
        {
            if (pYa <= 0)
                continue;
            pXa = -pXa;
            factorA = __ROL4__(pp->U + pXa * lv.var_48, 16);
            factorB = __ROL4__(pp->V + pXa * lv.var_54, 16);
            factorC = (ulong)(pp->S + pXa * lv.var_60) >> 8;
            factorB = (factorB & 0xFFFFFF00) | (factorA & 0xFF);
            factorA = (factorA & 0xFFFF0000) | (factorC & 0xFFFF);
            factorD = __ROL4__(pp->V + pXa * lv.var_54, 16);
            if (pYa > vec_window_width)
                pYa = vec_window_width;

            colM = (factorC & 0xFF) + ((factorD & 0xFF) << 8);
        }
        else
        {
            if (pYa > vec_window_width)
                pYa = vec_window_width;
            pY_overflow = __OFSUBS__(pYa, pXa);
            pYa -= pXa;
            if (((pYa < 0) ^ pY_overflow) | (pYa == 0))
                continue;
            o += pXa;
            factorA = __ROL4__(pp->U, 16);
            factorB = __ROL4__(pp->V, 16);
            factorC = (ulong)pp->S >> 8;
            factorB = (factorB & 0xFFFFFF00) | (factorA & 0xFF);
            factorA = (factorA & 0xFFFF0000) | (factorC & 0xFFFF);
            factorD = __ROL4__(pp->V, 16);

            colM = (factorC & 0xFF) + ((factorD & 0xFF) << 8);
        }

        factorB = (factorB & 0xFFFF00FF);
        m = vec_map;
        g = pixmap.ghost_table;
        f = pixmap.fade_table;

        for (; pYa > 0; pYa--, o++)
        {
            ushort colS;
            ubyte factorA_carry, factorB_carry;

            colM = (colM & 0xFF00) | (factorB & 0xFF);
            colS = (factorA & 0xFF00) | m[colM];
            factorA_carry = __CFADDL__(lv.var_70, factorA);
            factorA = lv.var_70 + factorA;
            factorB_carry = __CFADDL__(lv.var_74, factorB + factorA_carry);
            factorB = lv.var_74 + factorB + factorA_carry;
            colM = (colM & 0xFF) + ((((colM >> 8) + lv.var_88 + factorB_carry) & 0xFF) << 8);

            if ((colS & 0xFF) <= 0xCu) {
                colS = ((*o) << 8) | f[colS];
                *o = g[colS];
            } else {
                *o = f[colS];
            }
        }
    }
}

/******************************************************************************/
