/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file player.h
 *     Header file for player.c.
 * @par Purpose:
 *     Player state and information handling.
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
#ifndef PLAYER_H
#define PLAYER_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct Thing;

struct __attribute__((packed)) __attribute__((aligned(2))) SpecialUserInput
{
  long Bits;
  char DtX;
  char DtZ;
  ubyte LastDir;
  ubyte CurDir;
  ushort Turn;
  ushort MapX[3];
  ushort MapY[3];
  ushort MapZ[3];
  ushort OnFace;
  ushort ControlMode;
  ushort MapIndex;
};

typedef struct {
  struct SpecialUserInput UserInput[3];
  char field_66[2];
  int field_68[4];
  int field_78;
  int field_7C[2];
  int field_84;
    ulong DirectControl[4]; /* offs=0x88 */
    ulong ControlPad;
    struct Thing *MyAgent[4];
    ubyte PrevWeapon[4];  /* offs=0xAC */
    ubyte PanelState[4];
    ubyte PanelItem[4];
    ushort Dummy98; /* offs=0xB8 */
    ubyte Dummy97;
    ubyte MissionAgents;
  char field_BC[2];
  char field_BE[8];
  char field_C6;
  char field_C7;
  char field_C8;
  char field_C9;
  char field_CA[2];
  int field_CC;
  int field_D0;
  int field_D4;
  int field_D8;
  int field_DC;
  short field_E0;
  short field_E2;
  short field_E4;
  int field_E6[2];
  int field_EE;
  int field_F2;
  int field_F6;
  char field_FA[4];
  int field_FE;
  int field_102;
  char field_106[3];
  char field_109;
  char field_10A[3];
  char field_10D;
  char field_10E[3];
  char field_111;
  char field_112[3];
  char field_115;
  char field_116[4];
  char field_11A;
  char field_11B[125];
  short field_198;
  short field_19A;
  char field_19C[4];
  short field_1A0;
  char field_1A2[4];
  char field_1A6[4];
} PlayerInfo;


#pragma pack()
/******************************************************************************/
extern PlayerInfo players[8];
extern uint8_t local_player_no;

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif