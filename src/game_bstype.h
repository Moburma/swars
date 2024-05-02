/******************************************************************************/
// Syndicate Wars Port - open-source reimplementation of SW.
/******************************************************************************/
/** @file game_bstype.h
 *     Basic data types declaration.
 * @par Purpose:
 *     Declares very basic data types, which are widely used in other headers.
 *     Expands upon data types from bflibrary, adding game-specific types.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Jan 2016 - 03 May 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef GAME_BSTYPE_H
#define GAME_BSTYPE_H

#include "bflib_basics.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

/** Game turn number, used for in-game time computations. */
typedef ulong GameTurn;
/** Signed game turn, for storing game turns difference. */
typedef long SGameTurn;

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
