/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file engindrwlstm.c
 *     Making drawlists for the 3D engine.
 * @par Purpose:
 *     Implements functions for filling drawlists.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2024 - 12 May 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "engindrwlstm.h"

#include "bfendian.h"
#include "bfmath.h"

#include "display.h"
#include "enginbckt.h"
#include "engindrwlstx.h"
#include "enginshrapn.h"
#include "engintrns.h"
#include "game.h"
#include "game_data.h"
#include "game_speed.h"
#include "swlog.h"
#include "thing.h"
/******************************************************************************/
extern ubyte byte_176D49;

extern long dword_176CAC;
extern long dword_152E4C;

ubyte byte_152EF0[] = {
   0, 10,  5, 10,  7,  7,  8, 10,
  10, 10,  5,  7,  5,  7,  7,  0,
};

/** Add a new draw item and return linked SortLine instance.
 *
 * @param ditype Draw item type, should be one of SortLine related types.
 * @param bckt Destination bucket for this draw item.
 * @return SortLine instance to fill, or NULL if arrays exceeded.
 */
struct SortLine *draw_item_add_line(ubyte ditype, ushort bckt)
{
    struct SortLine *p_sline;

    if (next_sort_line >= mem_game[33].N)
        return NULL;

    p_sline = p_current_sort_line;
    if (!draw_item_add(ditype, next_sort_line, bckt))
        return NULL;

    p_current_sort_line++;
    next_sort_line++;

    return p_sline;
}

/** Add a new draw item and return linked SortSprite instance.
 *
 * @param ditype Draw item type, should be one of SortSprite related types.
 * @param bckt Destination bucket for this draw item.
 * @return SortSprite instance to fill, or NULL if arrays exceeded.
 */
struct SortSprite *draw_item_add_sprite(ubyte ditype, ushort bckt)
{
    struct SortSprite *p_sspr;

    if (next_sort_sprite >= mem_game[32].N)
        return NULL;

    p_sspr = p_current_sort_sprite;
    if (!draw_item_add(ditype, next_sort_sprite, bckt))
        return NULL;

    p_current_sort_sprite++;
    next_sort_sprite++;

    return p_sspr;
}

/** Add a new draw item and return first of linked SpecialPoint instances.
 *
 * @param ditype Draw item type, should be one of SpecialPoint related types.
 * @param bckt Destination bucket for this draw item.
 * @param npoints Amount of consecutive points to reserve.
 * @return SpecialPoint instance to fill, or NULL if arrays exceeded.
 */
struct SpecialPoint *draw_item_add_points(ubyte ditype, ushort offset, ushort bckt, ushort npoints)
{
    struct SpecialPoint *p_scrpoint;

    if (next_screen_point + npoints > mem_game[30].N)
        return NULL;

    p_scrpoint = &game_screen_point_pool[next_screen_point];
    if (!draw_item_add(ditype, offset, bckt))
        return NULL;

    next_screen_point += npoints;

    return p_scrpoint;
}

void draw_mapwho_vect_len(int x1, int y1, int z1, int x2, int y2, int z2, int len, int col)
{
#if 0
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_mapwho_vect_len\n"
        : : "a" (x1), "d" (y1), "b" (z1), "c" (x2), "g" (y2), "g" (z2), "g" (len), "g" (col));
    return;
#endif
    struct SortLine *p_sline;
    int dt_x, dt_y, dt_z;
    int dist;
    int end_x, end_y, end_z;
    int scr_dx, scr_dy;
    short scr_x1, scr_y1, scr_x2, scr_y2;
    int fctr_y, fctr_xz, pers5_range;
    int abs_scr_dy, abs_scr_dx;
    int bckt_shift;
    ubyte flags_A, flags_B;

    dt_y = y2 - y1;
    dt_z = z2 - z1;
    dt_x = x2 - x1;

    dist = LbSqrL(dt_z * dt_z + dt_x * dt_x + dt_y * dt_y);
    if (dist == 0)
        dist = 1;
    end_y = y1 + dt_y * len / dist;
    end_z = z1 + dt_z * len / dist;
    end_x = x1 + dt_x * len / dist;

    fctr_y = 8 * y1 - 8 * engn_yc;
    fctr_xz = (dword_176D10 * x1 + dword_176D14 * z1) >> 16;
    pers5_range = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
    abs_scr_dx = (dword_176D14 * x1 - dword_176D10 * z1) >> 16;

    flags_A = 0;
    flags_B = 0;
    bckt_shift = pers5_range;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = ((0x4000 - pers5_range) * scr_dx) >> 14;

    scr_x1 = dword_176D3C + scr_dx;
    if (scr_x1 < 0) {
        if (scr_x1 < -2000)
            scr_x1 = -2000;
        flags_A |= 0x01;
    } else if (scr_x1 >= vec_window_width) {
        if (scr_x1 > 2000)
            scr_x1 = 2000;
        flags_A |= 0x02;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = (scr_dy * (0x4000 - pers5_range)) >> 14;

    scr_y1 = dword_176D40 - scr_dy;
    if (scr_y1 < 0) {
        if (scr_y1 < -2000)
            scr_y1 = -2000;
        flags_A |= 0x04;
    } else if (scr_y1 >= vec_window_height) {
        if (scr_y1 > 2000)
            scr_y1 = 2000;
        flags_A |= 0x08;
    }

    flags_A |= 0x40;

    fctr_y = 8 * end_y - 8 * engn_yc;
    fctr_xz = (dword_176D10 * end_x + dword_176D14 * end_z) >> 16;
    pers5_range = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
    abs_scr_dx = (dword_176D14 * end_x - dword_176D10 * end_z) >> 16;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = ((0x4000 - pers5_range) * scr_dx) >> 14;

    scr_x2 = dword_176D3C + scr_dx;
    if (scr_x2 < 0) {
        if (scr_x2 < -2000)
            scr_x2 = -2000;
        flags_B |= 0x01;
    } else if (scr_x2 >= vec_window_width) {
        if (scr_x2 > 2000)
            scr_x2 = 2000;
        flags_B |= 0x02;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = ((0x4000 - pers5_range) * scr_dy) >> 14;

    scr_y2 = dword_176D40 - scr_dy;
    if (scr_y2 < 0) {
        if (scr_y2 < -2000)
            scr_y2 = -2000;
        flags_B |= 0x04;
    } else if (scr_y2 >= vec_window_height) {
        if (scr_y2 > 2000)
            scr_y2 = 2000;
        flags_B |= 0x08;
    }

    flags_B |= 0x40;

    if ((flags_B & flags_A & 0xF) != 0)
        return;

    p_sline = draw_item_add_line(DrIT_Unkn11, bckt_shift + 5000);
    if (p_sline == NULL)
        return;

    p_sline->Shade = 32;
    p_sline->Flags = 0;
    p_sline->X1 = scr_x1;
    p_sline->Y1 = scr_y1;
    p_sline->X2 = scr_x2;
    p_sline->Y2 = scr_y2;
    p_sline->Col = col;
}

void draw_e_graphic(int x, int y, int z, ushort frame, int radius, int intensity, struct Thing *p_thing)
{
#if 0
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_e_graphic\n"
        : : "a" (x), "d" (y), "b" (z), "c" (frame), "g" (radius), "g" (intensity), "g" (p_thing));
    return;
#endif
    struct SortSprite *p_sspr;
    int scr_dx, scr_dy;
    short scr_x, scr_y, scr_z;
    int fctr_xz, fctr_y;
    int abs_scr_dy, abs_scr_dx;
    int pers5_range;

    if (current_map == 9) // map009 Singapore on-water map
        y += waft_table[gameturn & 0x1F] >> 3;

    fctr_y = 8 * y - 8 * engn_yc;
    fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
    pers5_range = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
    abs_scr_dx = (dword_176D14 * x - dword_176D10 * z) >> 16;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = ((0x4000 - pers5_range) * scr_dx) >> 14;

    scr_x = dword_176D3C + scr_dx;
    if (scr_x < 0) {
        if (scr_x < -2000)
            scr_x = -2000;
    } else if (scr_x >= vec_window_width) {
        if (scr_x > 2000)
            scr_x = 2000;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = (scr_dy * (0x4000 - pers5_range)) >> 14;

    scr_y = dword_176D40 - scr_dy;
    if (scr_y < 0) {
        if (scr_y < -2000)
            scr_y = -2000;
    } else if (scr_y >= vec_window_height) {
        if (scr_y > 2000)
            scr_y = 2000;
    }

    scr_z = pers5_range - radius;
    if ((ingame.DisplayMode != 50) && ((p_thing->Flag2 & 0x20000000) != 0))
        scr_z = -10000;

    p_sspr = draw_item_add_sprite(DrIT_Unkn3, scr_z + 5000);
    if (p_sspr == NULL)
        return;

    p_sspr->X = scr_x;
    p_sspr->Y = scr_y;
    p_sspr->Z = scr_z;
    p_sspr->Frame = frame;
    p_sspr->Brightness = intensity;
    p_sspr->Scale = 256;
    p_sspr->PThing = p_thing;
}

void draw_e_graphic_scale(int x, int y, int z, ushort frame, int radius, int intensity, int scale)
{
#if 0
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_e_graphic_scale\n"
        : : "a" (x), "d" (y), "b" (z), "c" (frame), "g" (radius), "g" (intensity), "g" (scale));
    return;
#endif
    struct SortSprite *p_sspr;
    int scr_dx, scr_dy;
    short scr_x, scr_y, scr_z;
    int fctr_xz, fctr_y;
    int abs_scr_dy, abs_scr_dx;
    int pers5_range;

    if (current_map == 9) // map009 Singapore on-water map
        y += waft_table[gameturn & 0x1F] >> 3;

    fctr_y = 8 * y - 8 * engn_yc;
    fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
    pers5_range = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - fctr_xz * dword_176D18) >> 16;
    abs_scr_dx = (x * dword_176D14 - z * dword_176D10) >> 16;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = (scr_dx * (0x4000 - pers5_range)) >> 14;

    scr_x = scr_dx + dword_176D3C;
    if (scr_x < 0) {
        if (scr_x < -2000)
            scr_x = -2000;
    } else if (scr_x >= vec_window_width) {
        if (scr_x > 2000)
            scr_x = 2000;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = (scr_dy * (0x4000 - pers5_range)) >> 14;

    scr_y = dword_176D40 - scr_dy;
    if (scr_y < 0) {
        if (scr_y < -2000)
            scr_y = -2000;
    } else if (scr_y >= vec_window_height) {
        if (scr_y > 2000)
            scr_y = 2000;
    }
    scr_z = pers5_range - radius - 100;

    p_sspr = draw_item_add_sprite(DrIT_Unkn15, scr_z + 5000);
    if (p_sspr == NULL)
        return;

    p_sspr->X = scr_x;
    p_sspr->Y = scr_y;
    p_sspr->Z = scr_z;
    p_sspr->Frame = frame;
    p_sspr->Brightness = intensity;
    p_sspr->Scale = scale;
    p_sspr->PThing = NULL;
}

void draw_pers_e_graphic(struct Thing *p_thing, int x, int y, int z, int frame, int radius, int intensity)
{
#if 0
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_pers_e_graphic\n"
        : : "a" (p_thing), "d" (x), "b" (y), "c" (z), "g" (frame), "g" (radius), "g" (intensity));
    return;
#endif
    struct SortSprite *p_sspr;
    int scr_dx, scr_dy;
    short scr_x, scr_y;
    int fctr_xz, fctr_y;
    int abs_scr_dy, abs_scr_dx;
    int pers5_range;
    int bckt_shift;
    ubyte bri;

    bri = byte_152EF0[p_thing->SubType] + intensity;

    if (current_map == 9) // map009 Singapore on-water map
        y += waft_table[gameturn & 0x1F] >> 3;

    fctr_y = 8 * y - 8 * engn_yc;
    fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
    pers5_range = (dword_176D18 * fctr_y + dword_176D1C * fctr_xz) >> 16;
    abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
    abs_scr_dx = (dword_176D14 * x - dword_176D10 * z) >> 16;

    scr_dx = (overall_scale * abs_scr_dx) >> 11;
    if (game_perspective == 5)
        scr_dx = (scr_dx * (0x4000 - pers5_range)) >> 14;

    scr_x = dword_176D3C + scr_dx;
    if (scr_x < 0) {
        if (scr_x < -2000)
            scr_x = -2000;
    } else if (scr_x >= vec_window_width) {
        if (scr_x > 2000)
            scr_x = 2000;
    }

    scr_dy = (overall_scale * abs_scr_dy) >> 11;
    if (game_perspective == 5)
        scr_dy = (scr_dy * (0x4000 - pers5_range)) >> 14;

    scr_y = dword_176D40 - scr_dy;
    if (scr_y < 0) {
        if (scr_y < -2000)
            scr_y = -2000;
    } else if (scr_y >= vec_window_height) {
        if (scr_y > 2000)
            scr_y = 2000;
    }

    bckt_shift = pers5_range - radius;
    if (ingame.DisplayMode == 50)
    {
        if ((p_thing->Flag2 & 0x20000000) != 0) {
            if ((p_thing->Flag & 0x02) != 0)
                return;
            bckt_shift -= 1000000;
        }
    }
    else
    {
        if ((p_thing->Flag2 & 0x20000000) != 0)
            bckt_shift = -10000;
    }

    p_sspr = draw_item_add_sprite(DrIT_Unkn13, bckt_shift + 5000);
    if (p_sspr == NULL)
        return;

    p_sspr->X = scr_x;
    p_sspr->Y = scr_y;
    p_sspr->Z = 0;
    p_sspr->Frame = frame;
    p_sspr->PThing = p_thing;
    p_sspr->Brightness = bri;
    p_sspr->Angle = (p_thing->U.UObject.Angle + 8 - byte_176D49) & 7;

    if ((p_thing->Flag2 & 0x20000000) != 0 || p_thing->U.UPerson.OnFace || (p_thing->SubType == SubTT_PERS_MECH_SPIDER))
        return;

    p_sspr = draw_item_add_sprite(DrIT_Unkn19, bckt_shift + 5000 - 200);
    if (p_sspr == NULL)
        return;

    p_sspr->X = scr_x;
    p_sspr->Y = scr_y;
    p_sspr->PThing = p_thing;
}

void FIRE_draw_fire(struct SimpleThing *p_sthing)
{
#if 0
    asm volatile ("call ASM_FIRE_draw_fire\n"
        : : "a" (p_sthing));
    return;
#endif
    struct FireFlame *p_flame;
    ushort flm;

    for (flm = p_sthing->U.UFire.flame; flm; flm = p_flame->next)
    {
        struct SpecialPoint *p_scrpoint;
        int x, y, z;
        int scr_dx, scr_dy;
        int abs_scr_dy, abs_scr_dx;
        int fctr_xz, fctr_y;
        int pers5_range;
        short scr_x, scr_y;
        ubyte flags;

        p_flame = &FIRE_flame[flm];
        x = p_flame->x - engn_xc;
        y = p_flame->y - engn_yc;
        z = p_flame->z - engn_zc;

        if (current_map == 9) // map009 Singapore on-water map
            y += waft_table[gameturn & 0x1F];

        fctr_y = y - 8 * engn_yc;
        fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
        pers5_range = (dword_176D18 * fctr_y + dword_176D1C * fctr_xz) >> 16;
        abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
        abs_scr_dx = (dword_176D14 * x - dword_176D10 * z) >> 16;

        scr_dx = (overall_scale * abs_scr_dx) >> 11;
        if (game_perspective == 5)
            scr_dx = (scr_dx * (0x4000 - pers5_range)) >> 14;

        scr_x = dword_176D3C + scr_dx;
        if (scr_x < 0) {
            if (scr_x < -2000)
                scr_x = -2000;
            flags |= 0x01;
        } else if (scr_x >= vec_window_width) {
            if (scr_x > 2000)
                scr_x = 2000;
            flags |= 0x02;
        }

        scr_dy = (overall_scale * abs_scr_dy) >> 11;
        if (game_perspective == 5)
            scr_dy = (scr_dy * (0x4000 - pers5_range)) >> 14;

        scr_y = dword_176D40 - scr_dy;
        if (scr_y < 0) {
            if (scr_y < -2000)
                scr_y = -2000;
            flags |= 0x04;
        } else if (scr_y >= vec_window_height) {
            if (scr_y > 2000)
                scr_y = 2000;
            flags |= 0x08;
        }

        flags |= 0x40;

        p_flame->PointOffset = next_screen_point;
        p_scrpoint = draw_item_add_points(DrIT_Unkn25, flm, pers5_range + 5000 - 50, 1);
        if (p_scrpoint == NULL)
            break;

        p_scrpoint->X = scr_x;
        p_scrpoint->Y = scr_y;
        p_scrpoint->Z = pers5_range;
    }
}

void draw_bang_phwoar(struct SimpleThing *p_pow)
{
    struct Phwoar *p_phwoar;
    ushort phw;

    for (phw = p_pow->U.UBang.phwoar; phw != 0; phw = p_phwoar->child)
    {
        struct SpecialPoint *p_scrpoint;
        int x, y, z;
        int scr_dx, scr_dy;
        int abs_scr_dy, abs_scr_dx;
        int fctr_xz;
        int pers5_range;
        short scr_x, scr_y;
        ubyte flags;

        p_phwoar = &phwoar[phw];
        x = (p_phwoar->x >> 8) - engn_xc;
        z = (p_phwoar->z >> 8) - engn_zc;
        y = (p_phwoar->y >> 5) - engn_yc - 8 * engn_yc;

        fctr_xz = (dword_176D10 * x + dword_176D14 * z) >> 16;
        pers5_range = (dword_176D18 * y + dword_176D1C * fctr_xz) >> 16;
        abs_scr_dy = (dword_176D1C * y - dword_176D18 * fctr_xz) >> 16;
        abs_scr_dx = (dword_176D14 * x - dword_176D10 * z) >> 16;

        scr_dx = (overall_scale * abs_scr_dx) >> 11;
        if (game_perspective == 5)
            scr_dx = ((0x4000 - pers5_range) * scr_dx) >> 14;

        scr_x = dword_176D3C + scr_dx;
        if (scr_x < 0) {
            if (scr_x < -2000)
                scr_x = -2000;
            flags |= 0x01;
        } else if (scr_x >= vec_window_width) {
            if (scr_x > 2000)
                scr_x = 2000;
            flags |= 0x02;
        }

        scr_dy = (overall_scale * abs_scr_dy) >> 11;
        if (game_perspective == 5)
            scr_dy = (scr_dy * (0x4000 - pers5_range)) >> 14;

        scr_y = dword_176D40 - scr_dy;
        if (scr_y < 0) {
            if (scr_y < -2000)
                scr_y = -2000;
            flags |= 0x04;
        } else if (scr_y >= vec_window_height) {
            if (scr_y > 2000)
                scr_y = 2000;
            flags |= 0x08;
        }

        flags |= 0x40;

        p_phwoar->PointOffset = next_screen_point;
        p_scrpoint = draw_item_add_points(DrIT_Unkn21, phw, pers5_range + 5000 - 100, 1);
        if (p_scrpoint == NULL)
            break;

        p_scrpoint->X = scr_x;
        p_scrpoint->Y = scr_y;
        p_scrpoint->Z = pers5_range;
    }
}

void draw_bang_shrapnel(struct SimpleThing *p_pow)
{
    struct Shrapnel *p_shrapnel;
    ushort shrap;

    for (shrap = p_pow->U.UBang.shrapnel; shrap != 0; shrap = p_shrapnel->child)
    {
        struct SpecialPoint *p_scrpoint;
        int x, y, z;
        int scr_dx, scr_dy;
        int abs_scr_dy, abs_scr_dx;
        int fctr_xz, fctr_y;
        int v80, v99, v65, v70, v71, v72, v73, v74;
        short scr_x1, scr_y1, scr_z1;
        short scr_x2, scr_y2, scr_z2;
        short scr_x3, scr_y3, scr_z3;
        ushort bckt_shift;
        ubyte flags_A, flags_B, flags_C;


        p_shrapnel = &shrapnel[shrap];
        if ((p_shrapnel->type < 1) || (p_shrapnel->type > 3))
            continue;

        {
            s64 tmpLL;
            uint tmpU;
            int fctr_A, fctr_B, fctr_C, fctr_D, fctr_E;
            int cos_yaw, cos_pitch, sin_yaw, sin_pitch;
            short shrap_yaw, shrap_pitch;

            shrap_yaw = 8 * p_shrapnel->yaw;
            shrap_pitch = 8 * p_shrapnel->pitch;
            cos_yaw = lbSinTable[shrap_yaw + 512];
            cos_pitch = lbSinTable[shrap_pitch + 512];
            sin_pitch = lbSinTable[shrap_pitch];
            sin_yaw = lbSinTable[shrap_yaw];

            tmpLL = cos_pitch * cos_yaw;
            tmpU = (tmpLL & 0xFFFF0000) | ((tmpLL >> 32) & 0xFFFF);
            fctr_A = bw_rotl32(tmpU, 16) >> 10;
            tmpLL = cos_pitch * sin_yaw;
            tmpU = (tmpLL & 0xFFFF0000) | ((tmpLL >> 32) & 0xFFFF);
            fctr_B = bw_rotl32(tmpU, 16) >> 10;
            tmpLL = sin_pitch * cos_yaw;
            tmpU = (tmpLL & 0xFFFF0000) | ((tmpLL >> 32) & 0xFFFF);
            fctr_C = bw_rotl32(tmpU, 16) >> 10;
            tmpLL = sin_pitch * sin_yaw;
            tmpU = (tmpLL & 0xFFFF0000) | ((tmpLL >> 32) & 0xFFFF);
            fctr_D = bw_rotl32(tmpU, 16) >> 10;

            fctr_E = sin_yaw >> 10;

            v80 = x + fctr_A;
            v99 = x + fctr_B;
            v65 = x - fctr_A - fctr_B;
            v70 = z + fctr_E;
            v71 = z - fctr_E;
            v72 = y - fctr_C;
            v73 = y - fctr_D;
            v74 = y + fctr_C + fctr_D;
        }

        x = (p_shrapnel->x >> 8) - engn_xc;
        y = (p_shrapnel->y >> 5) - engn_yc;
        z = (p_shrapnel->z >> 8) - engn_zc;

        flags_C = 0;
        flags_B = 0;
        flags_A = 0;

        fctr_y = v72 - 8 * engn_yc;
        fctr_xz = (dword_176D10 * v80 + dword_176D14 * v71) >> 16;
        scr_z1 = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
        abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;
        abs_scr_dx = (dword_176D14 * v80 - dword_176D10 * v71) >> 16;

        scr_dx = (overall_scale * abs_scr_dx) >> 11;
        if (game_perspective == 5)
            scr_dx = (abs_scr_dx * (0x4000 - scr_z1)) >> 14;

        scr_x1 = dword_176D3C + scr_dx;
        if (scr_x1 < 0) {
            if (scr_x1 < -2000)
                scr_x1 = -2000;
            flags_A |= 0x01;
        } else if (scr_x1 >= vec_window_width) {
            if (scr_x1 > 2000)
                scr_x1 = 2000;
            flags_A |= 0x02;
        }

        scr_dy = (overall_scale * abs_scr_dy) >> 11;
        if (game_perspective == 5)
            scr_dy = (scr_dy * (0x4000 - scr_z1)) >> 14;

        scr_y1 = dword_176D40 - scr_dy;
        if (scr_y1 < 0) {
            if (scr_y1 < -2000)
                scr_y1 = -2000;
            flags_A |= 0x04;
        } else if (scr_y1 >= vec_window_height) {
            if (scr_y1 > 2000)
                scr_y1 = 2000;
            flags_A |= 0x08;
        }

        flags_A |= 0x40;

        fctr_y = v73 - 8 * engn_yc;
        abs_scr_dx = (dword_176D14 * v99 - dword_176D10 * z) >> 16;
        fctr_xz = (dword_176D14 * z + dword_176D10 * v99) >> 16;
        scr_z2 = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
        abs_scr_dy = (dword_176D1C * fctr_y - dword_176D18 * fctr_xz) >> 16;

        scr_dx = (overall_scale * abs_scr_dx) >> 11;
        if (game_perspective == 5)
            scr_dx = (scr_dx * (0x4000 - scr_z2)) >> 14;

        scr_x2 = dword_176D3C + scr_dx;
        if (scr_x2 < 0) {
            if (scr_x2 < -2000)
                scr_x2 = -2000;
            flags_B = 0x01;
        } else if (scr_x2 >= vec_window_width) {
            if (scr_x2 > 2000)
                scr_x2 = 2000;
            flags_B = 0x02;
        }

        scr_dy = (overall_scale * abs_scr_dy) >> 11;
        if (game_perspective == 5)
            scr_dy = (scr_dy * (0x4000 - scr_z2)) >> 14;

        scr_y2 = dword_176D40 - scr_dy;
        if (scr_y2 < 0) {
            if (scr_y2 < -2000)
                scr_y2 = -2000;
            flags_B |= 0x04;
        } else if (scr_y2 >= vec_window_height) {
            if (scr_y2 > 2000)
                scr_y2 = 2000;
            flags_B |= 0x08;
        }

        flags_B |= 0x40;

        fctr_y = v74 - 8 * engn_yc;
        abs_scr_dx = (dword_176D14 * v65 - dword_176D10 * v70) >> 16;
        fctr_xz = (dword_176D14 * v70 + dword_176D10 * v65) >> 16;
        scr_z3 = (dword_176D1C * fctr_xz + dword_176D18 * fctr_y) >> 16;
        abs_scr_dy = (dword_176D1C * fctr_y - fctr_xz * dword_176D18) >> 16;

        scr_dx = (overall_scale * abs_scr_dx) >> 11;
        if (game_perspective == 5)
            scr_dx = ((0x4000 - scr_z3) * scr_dx) >> 14;

        scr_x3 = dword_176D3C + scr_dx;
        if (scr_x3 < 0) {
            if (scr_x3 < -2000)
                scr_x3 = -2000;
            flags_C |= 0x01;
        } else if (scr_x3 >= vec_window_width) {
            if (scr_x3 > 2000)
                scr_x3 = 2000;
            flags_C |= 0x02;
        }

        scr_dy = (overall_scale * abs_scr_dy) >> 11;
        if (game_perspective == 5)
            scr_dy = (scr_dy * (0x4000 - scr_z3)) >> 14;

        scr_y3 = dword_176D40 - scr_dy;
        if (scr_y3 < 0) {
            if (scr_y3 < -2000)
                scr_y3 = -2000;
            flags_C |= 0x04;
        } else if (scr_y3 >= vec_window_height) {
            if (scr_y3 > 2000)
                scr_y3 = 2000;
            flags_C |= 0x08;
        }

        flags_C |= 0x40;

        if (((flags_B & flags_A & flags_C) & 0xF) != 0)
            continue;

        bckt_shift = scr_z2;
        if (bckt_shift >= scr_z3)
            bckt_shift = scr_z3;
        if (bckt_shift > scr_z1)
            bckt_shift = scr_z1;

        p_shrapnel->PointOffset = next_screen_point;
        p_scrpoint = draw_item_add_points(DrIT_Unkn20, shrap, bckt_shift + 5000, 3);
        if (p_scrpoint == NULL)
            break;

        p_scrpoint->X = scr_x1;
        p_scrpoint->Y = scr_y1;
        p_scrpoint->Z = scr_z1;
        p_scrpoint++;

        p_scrpoint->X = scr_x2;
        p_scrpoint->Y = scr_y2;
        p_scrpoint->Z = scr_z2;
        p_scrpoint++;

        p_scrpoint->X = scr_x3;
        p_scrpoint->Y = scr_y3;
        p_scrpoint->Z = scr_z3;
    }
}

void build_laser(int x1, int y1, int z1, int x2, int y2, int z2, int itime, struct Thing *p_owner, int colour)
{
    asm volatile (
      "push %8\n"
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_build_laser\n"
        : : "a" (x1), "d" (y1), "b" (z1), "c" (x2), "g" (y2), "g" (z2), "g" (itime), "g" (p_owner), "g" (colour));
}

void draw_bang(struct SimpleThing *p_pow)
{
#if 1
    asm volatile ("call ASM_draw_bang_start\n"
        : : "a" (p_pow));
    asm volatile ("call ASM_draw_bang_shrapnel\n"
        : : "a" (p_pow));
    asm volatile ("call ASM_draw_bang_phwoar\n"
        : : "a" (p_pow));
    return;
#endif
}

ushort draw_rot_object(int offset_x, int offset_y, int offset_z, struct SingleObject *point_object, struct Thing *p_thing)
{
    ushort ret;
    asm volatile (
      "push %5\n"
      "call ASM_draw_rot_object\n"
        : "=r" (ret) : "a" (offset_x), "d" (offset_y), "b" (offset_z), "c" (point_object), "g" (p_thing));
    return ret;
}

ushort draw_rot_object2(int offset_x, int offset_y, int offset_z, struct SingleObject *point_object, struct Thing *p_thing)
{
    ushort ret;
    asm volatile (
      "push %5\n"
      "call ASM_draw_rot_object2\n"
        : "=r" (ret) : "a" (offset_x), "d" (offset_y), "b" (offset_z), "c" (point_object), "g" (p_thing));
    return ret;
}

ushort draw_object(int x, int y, int z, struct SingleObject *point_object)
{
    ushort ret;
    asm volatile ("call ASM_draw_object\n"
        : "=r" (ret) : "a" (x), "d" (y), "b" (z), "c" (point_object));
    return ret;
}

void draw_vehicle_health(struct Thing *p_thing)
{
    asm volatile ("call ASM_draw_vehicle_health\n"
        : : "a" (p_thing));
}

void draw_vehicle_shadow(ushort veh, ushort sort)
{
    asm volatile ("call ASM_draw_vehicle_shadow\n"
        : : "a" (veh), "d" (sort));
}

void draw_explode(void)
{
    asm volatile ("call ASM_draw_explode\n"
        :  :  : "eax" );
}

void func_2e440(void)
{
    asm volatile ("call ASM_func_2e440\n"
        :  :  : "eax" );
}

void build_polygon_circle(int x1, int y1, int z1, int r1, int r2, int flag, struct SingleFloorTexture *p_tex, int col, int bright1, int bright2)
{
    asm volatile (
      "push %9\n"
      "push %8\n"
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_build_polygon_circle\n"
        : : "a" (x1), "d" (y1), "b" (z1), "c" (r1), "g" (r2), "g" (flag), "g" (p_tex), "g" (col), "g" (bright1), "g" (bright2));
}

/******************************************************************************/
