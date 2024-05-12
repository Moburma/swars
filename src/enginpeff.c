/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginpeff.c
 *     Engine scene post-processing effects.
 * @par Purpose:
 *     Implement functions for adding effects while rendering a scene.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     29 Oct 2023 - 02 May 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "enginpeff.h"

#include "bfgentab.h"
#include "bfscreen.h"
#include "bfutility.h"
#include "display.h"
#include "game_speed.h"
#include "swlog.h"
/******************************************************************************/

void game_process_sub08(void)
{
    asm volatile ("call ASM_game_process_sub08\n"
        :  : );
}

void scene_post_effect_prepare(void)
{
    int i;
    switch (gamep_scene_effect)
    {
    case ScEff_RAIN:
        game_process_sub08();
        break;
    case ScEff_STATIC:
        for (i = 0; i < 10; i++) {
            ushort pos;
            ubyte *ptr;
            pos = LbRandomAnyShort() + (gameturn >> 2);
            ptr = vec_tmap[0] + pos;
            *ptr = pixmap.fade_table[40*PALETTE_8b_COLORS + *ptr];
        }
        break;
    }
}

static void draw_droplet(TbPixel *o, short scanln, short w, short h, TbPixel *coltb)
{
    short x, y;

    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++) {
            o[y * scanln + x] = coltb[o[y * scanln + x]];
        }
    }
}

void draw_falling_rain(int bckt)
{
    ulong icol;
    short limit_y;
    ushort rnd, m;
    ulong shift_y;
    TbPixel *o;
    TbPixel *coltb;
    ulong seed_bkp;
    short x, y;
    short scanln, w, h;

    seed_bkp = lbSeed;
    scanln = lbDisplay.GraphicsScreenWidth;

    icol = (10000 - bckt) / 416 << 7;
    shift_y = gameturn * (10000 - bckt);
    limit_y = 236 - (bckt >> 5);
    if (limit_y < 20)
        return;

    m = lbDisplay.GraphicsScreenHeight / 200;
    if (m == 0) m++;

    lbSeed = bckt;
    rnd = LbRandomPosShort();
    x = (rnd + (engn_xc >> 4) + (engn_anglexz >> 7)) % scanln;
    rnd = LbRandomPosShort();
    y = m * ((rnd + (shift_y >> 10)) % limit_y);
    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    o = &lbDisplay.WScreen[scanln * y + x];
    w = m;
    h = m;
    if (bckt < 4000) h += m;
    if (bckt < 3000) h += m;
    if (bckt < 1000) h += m;
    coltb = &pixmap.ghost_table[256 * pixmap.fade_table[15*PALETTE_8b_COLORS + 63 + icol]];
    draw_droplet(o, scanln, w, h, coltb);

    lbDisplay.DrawFlags = 0;
    lbSeed = seed_bkp;
}

void sub_2AAA0(int a1)
{
    asm volatile (
      "call ASM_sub_2AAA0\n"
        : : "a" (a1));
}

void scene_post_effect_for_bucket(short bckt)
{
    if ((bckt & 7) == 0)
    {
        switch (gamep_scene_effect)
        {
        case ScEff_RAIN:
            draw_falling_rain(bckt);
            break;
        case ScEff_STATIC:
            sub_2AAA0(bckt);
            break;
        default:
            break;
        }
    }
}

/******************************************************************************/
