/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_snd_sys.h
 *     Header file for bflib_snd_sys.c.
 * @par Purpose:
 *     Sound playback support.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_SNDSYS_H
#define BFLIB_SNDSYS_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
typedef void * TbSampleHandle;

/******************************************************************************/
void StopAllSamples(void);
TbBool IsSamplePlaying(long tng_offs, ushort smp_id, TbSampleHandle handle);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
