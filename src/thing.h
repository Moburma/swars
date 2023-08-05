/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file thing.h
 *     Header file for thing.c.
 * @par Purpose:
 *     Thing structure support.
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
#ifndef THING_H
#define THING_H

#include "bftypes.h"
#include "cybmod.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

enum ThingType {
    TT_NONE = 0x0,
    TT_UNKN1 = 0x1,
    TT_VEHICLE = 0x2,
    TT_PERSON = 0x3,
    TT_UNKN4 = 0x4,
    SmTT_STATIC = 0x5,
    TT_ROCKET = 0x6,
    TT_UNKN7 = 0x7,
    TT_UNKN8 = 0x8,
    TT_BUILDING = 0x9,
    TT_UNKN10 = 0xA,
    TT_LASER11 = 0xB,
    TT_LASER_GUIDED = 0xC,
    TT_UNKN13 = 0xD,
    TT_UNKN14 = 0xE,
    TT_MINE = 0xF,
    TT_GRENADE = 0x10,
    SmTT_SPARK = 0x11,
    TT_UNKN18 = 0x12,
    TT_LASER_ELEC = 0x13,
    SmTT_INTELLIG_DOOR = 0x14,
    SmTT_SCALE_EFFECT = 0x15,
    SmTT_NUCLEAR_BOMB = 0x16,
    TT_UNKN23 = 0x17,
    SmTT_SMOKE_GENERATOR = 0x18,
    SmTT_MINE = 0x19,
    SmTT_CARRIED_ITEM = 0x1A,
    SmTT_ELECTRIC_STRAND = 0x1B,
    TT_RAZOR_WIRE = 0x1C,
    TT_LASER29 = 0x1D,
    SmTT_TIME_POD = 0x1E,
    TT_AIR_STRIKE = 0x1F,
    SmTT_CANISTER = 0x20,
    TT_UNKN33 = 0x21,
    TT_UNKN34 = 0x22,
    TT_UNKN35 = 0x23,
    SmTT_STASIS_POD = 0x24,
    SmTT_SOUL = 0x25,
    TT_LASER38 = 0x26,
    TT_UNKN39 = 0x27,
    TT_UNKN40 = 0x28,
    TT_UNKN41 = 0x29,
    TT_UNKN42 = 0x2A,
    TT_UNKN43 = 0x2B,
    TT_UNKN44 = 0x2C,
    TT_UNKN45 = 0x2D,
    TT_UNKN46 = 0x2E,
    TT_UNKN47 = 0x2F,
    TT_UNKN48 = 0x30,
    TT_UNKN49 = 0x31,
    SmTT_BANG = 0x32,
    TT_UNKN51 = 0x33,
    SmTT_FIRE = 0x34,
    SmTT_SFX = 0x35,
    SmTT_TEMP_LIGHT = 0x36,
    TT_UNKN55 = 0x37,
    TT_UNKN56 = 0x38,
};

/** State of Thing of type Object/Building.
 */
struct TngUObject
{
    short NextThing;
    short PrevThing;
    ubyte Group;
    ubyte EffectiveGroup;
    short Object;
    short MatrixIndex;
    ubyte NumbObjects;
    ubyte Angle;
    ubyte Token;
    char TokenDir;
    char OffX;
    char OffZ;
    short TargetDX; // within Thing, pos=92
    short TargetDY;
    short TargetDZ;
    ushort BuildStartVect;
    ushort BuildNumbVect;
    ushort ZZ_unused_but_pads_to_long_ObjectNo;
    short ComHead;
    short ComCur;
    short Mood;
    short RaiseDY[2];
    short RaiseY[2];
    short MinY[2];
    short Timer[2];
    short MaxY[2];
    short TNode;
    short Cost;
    short Shite;
    int BHeight;
    ushort Turn;
    ushort TurnPadOnPS;
    short tnode[4];
    ubyte player_in_me;
    ubyte unkn_4D;
    ulong DrawTurn; // set within draw_thing_object() for a building
};

/** State of Thing of type MGun.
 */
struct TngUMGun
{
    short PathIndex;
    ushort UniqueID;
    ubyte Group;
    ubyte EffectiveGroup;
    short Object;
    short MatrixIndex;
    ubyte NumbObjects;
    ubyte ShotTurn;
    short WeaponTurn;
    ushort ObjectNo;
    short PrevThing; // within Thing, pos=92
    short NextThing;
    ubyte Token;
    char TokenDir;
    short AngleDY;
    short AngleX;
    short AngleY;
    short AngleZ;
    short GotoX;
    short GotoY;
    short GotoZ;
    int Dummy11[5];
    short RecoilTimer;
    ushort MaxHealth;
    ubyte CurrentWeapon;
    ubyte Dummy12;
    ubyte Dummy13[6];
};

/** State of Thing of type Vehicle.
 */
struct TngUVehicle
{
  short PathIndex;
  ushort UniqueID;
  ubyte Group;
  ubyte EffectiveGroup;
  short Object;
  short MatrixIndex;
  ubyte NumbObjects;
  ubyte Dummy2;
  short WeaponTurn;
  short ReqdSpeed;
  ushort MaxSpeed; // within Thing, pos=92
  ushort PassengerHead;
  short TNode;
  short AngleDY;
  short AngleX;
  short AngleY;
  short AngleZ;
  short GotoX;
  short GotoY;
  short GotoZ;
  short VehicleAcceleration;
  short LeisurePlace;
  short TargetDX;
  short TargetDY;
  short TargetDZ;
  short OnFace;
  short WorkPlace;
  short ComHead;
  short ComCur;
  short Timer2;
  short RecoilTimer;
  ushort MaxHealth;
  int Dummy[3];
  ushort SubThing;
  short Agok;
  int WobbleZP;
  int WobbleZV;
  ubyte Armour;
  ubyte PissedOffWithWaiting;
  short ZebraOldHealth;
  ushort destx;
  ushort destz;
};

/** State of Thing of type Effect.
 */
struct TngUEffect
{
  ushort MaxSpeed;
  short ReqdSpeed;
  ubyte Group;
  ubyte EffectiveGroup;
  short Object;
  short WeaponTurn;
  ubyte NumbObjects;
  ubyte Angle;
  ushort PassengerHead;
  short TNode;
  short AngleDY; // within Thing, pos=92
  short AngleX;
  short AngleY;
  short AngleZ;
  short GotoX;
  short GotoY;
  short GotoZ;
  short VehicleAcceleration;
  short MatrixIndex;
  short LeisurePlace;
};

/** Person Draw Sprite Frame.
 */
struct DrawFrameId
{
  ubyte Version[5];
};

/** State of Thing of type Person.
 */
struct TngUPerson
{
  short PathIndex;
  ushort UniqueID;
  ubyte Group;
  ubyte EffectiveGroup;
  ushort ComHead;
  ushort ComCur;
  char SpecialTimer;
  ubyte Angle;
  short WeaponTurn;
  ubyte Brightness;
  ubyte ComRange;
  ubyte BumpMode; // within Thing, pos=92
  ubyte BumpCount;
  short Vehicle;
  short LinkPassenger;
  ushort Within;
  ushort LastDist;
  short ComTimer;
  short Timer2;
  short StartTimer2;
  ubyte AnimMode;
  ubyte OldAnimMode;
  short OnFace;
  union Mod UMod;
  short Mood;
  struct DrawFrameId FrameId;
  ubyte Shadows[4];
  ubyte RecoilTimer;
  ushort MaxHealth;
  ubyte Flag3;
  ubyte OldSubType;
  short ShieldEnergy;
  ubyte ShieldGlowTimer;
  ubyte WeaponDir;
  ushort SpecialOwner;
  ushort WorkPlace;
  ushort LeisurePlace;
  short WeaponTimer;
  short Target2;
  short MaxShieldEnergy;
  short PersuadePower;
  short MaxEnergy;
  short Energy;
  ubyte RecoilDir;
  ubyte CurrentWeapon;
  short GotoX;
  short GotoZ;
  short TempWeapon;
  short Stamina;
  short MaxStamina;
  ulong WeaponsCarried;
};

/** Structure for storing State of any Thing.
 * Everything in game besides ground terrain, are Things.
 */
struct Thing { // sizeof=168
    short Parent;
    short Next;
    short LinkParent;
    short LinkChild;
    ubyte SubType;
    ubyte Type;
    short State;
    ulong Flag;
    short LinkSame;
    short LinkSameGroup;
    short Radius;
    ushort ThingOffset;
    long X;
    long Y;
    long Z;
    short Frame;
    ushort StartFrame;
    short Timer1;
    short StartTimer1;
    long VX;
    long VY;
    long VZ;
    short Speed;
    short Health;
    ushort Owner;
    char PathOffset;
    char SubState;
    struct Thing *PTarget;
    long Flag2;
    short GotoThingIndex;
    short OldTarget;
    union { // pos=76
        struct TngUObject UObject;
        struct TngUMGun UMGun;
        struct TngUVehicle UVehicle;
        struct TngUEffect UEffect;
        struct TngUPerson UPerson;
    } U;
};

struct SimpleThing
{
    short Parent;
    short Next;
    short LinkParent;
    short LinkChild;
    ubyte SubType;
    ubyte Type;
    short State;
    ulong Flag;
    short LinkSame;
    short Object;
    short Radius;
    short ThingOffset;
    long X;
    long Y;
    long Z;
    short Frame;
    short StartFrame;
    short Timer1;
    short StartTimer1;
    char U[12];
    long field_38;
};

/** Old structure for storing State of any Thing.
 * Used only to allow reading old, pre-release levels.
 */
struct ThingOldV9 { // sizeof=216
    short Parent;
    short Next;
    short LinkParent;
    short LinkChild;
    ubyte SubType;
    ubyte Type;
    short State; // pos=10
    ulong Flag;
    /* Since fmtver=4, this and all previous fields confirmed to match
     * file layout in final release (from Pre-Alpha Demo code analysis
     * and from comparative analysis of binary data in level files).
     */
    short LinkSame;
    /* Stores index of first item within game_objects[].
     * Since fmtver=4, most types (Object,Vehicle,MGun,Effect) have `Object`
     * and it is at the same place for all types (from Pre-Alpha Demo code
     * analysis). The Vehicle `Object` confirmed in fmtver=8-11 files (from
     * comparative analysis of binary data in level files).
     */
    union {
        short ObjectObject;
        short VehicleObject;
        short MGunObject;
        short EffectObject;
    };
    short Radius; // pos=20
    /* Since fmtver=4, most types (Object,Vehicle,MGun,Effect) have `NumbObjects`
     * and it is at the same place (from Pre-Alpha Demo code analysis).
     */
    union {
        ubyte ObjectNumbObjects;
        ubyte VehicleNumbObjects;
        ubyte MGunNumbObjects;
        ubyte EffectNumbObjects;
    };
    ubyte TngUnkn23;
    long X;
    long Y;
    long Z;
    /* Since fmtver=4, most types have `Frame`, only Object type GATE reuses it
     * for `MinY[0]` (from Pre-Alpha Demo code analysis).
     */
    union {
        short Frame;
        short ObjectMinY0;
    };
    /* Since fmtver=4, most types have `StartFrame`, only Object type GATE
     *  reuses it for `RaiseY[1]` (from Pre-Alpha Demo code analysis).
     */
    union {
        ushort StartFrame;
        short ObjectRaiseY1;
    };
    short Timer1; // pos=40
    short StartTimer1;
    /* Since fmtver=4, usually `Timer2`, but Objects have `Timer[0]` (from
     * Pre-Alpha Demo code analysis).
     */
    union {
        short Timer2;
        short ObjectTimer0;
    };
    /* Since fmtver=4, usually `StartTimer2`, but Objects have `Timer[1]`
     * (from Pre-Alpha Demo code analysis).
     */
    union {
        short StartTimer2;
        short ObjectTimer1;
    };
    /* Since fmtver=4, Person `AnimMode` (from Pre-Alpha Demo code analysis).
     * Confirmed to be low value used against peole in fmtver=8-11 files
     * (from comparative analysis of binary data in level files).
     */
    ubyte PersonAnimMode; // pos=48
    /* Since fmtver=4, Person `OldAnimMode` (from Pre-Alpha Demo code analysis).
     */
    ubyte PersonOldAnimMode;
    ushort ThingOffset; // pos=50
    /* Since fmtver=4, usually `VX`, but Object type GATE reuses it for
     * `RaiseDY[]` (from Pre-Alpha Demo code analysis).
     * The `VX`/`VY`/`VZ` confirmed in fmtver=8-11 (from comparative analysis
     * of binary data in level files).
     */
    union {
        long VX;
        short ObjectRaiseDY[2];
    };
    long VY;
    long VZ;
    /* Since fmtver=4, `AngleX` for Vehicle and MGun (from Pre-Alpha Demo code
     * analysis). Stamina is only here in fmtver=12 files - does not seem to
     * exist in 9-11 (from comparative analysis of binary data in level files).
     */
    union {
        short VehicleAngleX;
        short MGunAngleX;
        short PersonStamina;
    };
    /* Since fmtver=4, `AngleY` for Vehicle and MGun (from Pre-Alpha Demo code
     * analysis). MaxStamina is only here in fmtver=12 files (from comparative
     * analysis of binary data in level files).
     */
    union {
        short VehicleAngleY;
        short MGunAngleY;
        short PersonMaxStamina;
    };
    /* Since fmtver=4, `AngleZ` for Vehicle and MGun (from Pre-Alpha Demo code
     * analysis).
     */
    union { // pos=68
        short VehicleAngleZ;
        short MGunAngleZ;
    };
    /* The `LinkSameGroup` confirmed in fmtver=8-11 (from comparative analysis
     * of binary data in level files).
     */
    short LinkSameGroup; // pos=70
    short TngUnkn72;
    /* Since fmtver=4, `Angle` for Person and Object (from Pre-Alpha Demo code
     * analysis). Confirmed to be some Person stat in fmtver=8-11 (from
     * comparative analysis of binary data in level files).
     */
    union {
        ubyte PersonAngle;
        ubyte ObjectAngle;
    };
    ubyte TngUnkn75;
    /* Since fmtver=4, `Speed` for Vehicle (from Pre-Alpha Demo code analysis).
     */
    short Speed; // pos=76
    short Health; // pos=78
    /* Since fmtver=4, `GotoX` for Person (from Pre-Alpha Demo code analysis).
     */
    short PersonGotoX; // pos=80
    /* Since fmtver=4, `GotoY` for Person (from Pre-Alpha Demo code analysis).
     */
    short PersonGotoY; // pos=82
    /* Since fmtver=4, `GotoZ` for Person (from Pre-Alpha Demo code analysis).
     */
    short PersonGotoZ; // pos=84
    ubyte PersonGroup; // pos=86 There seems to be no EffectiveGroup in early files
    ubyte TngUnkn87; // pos=87
    ulong PersonWeaponsCarried; // pos=88
    ushort PersonComHead; // pos=92
    short TngUnkn94; // pos=94
    short TngUnkn96; // pos=96
    short TngUnkn98; // pos=98
    ushort Owner; // pos=100
    short TngUnkn102; // pos=102  People only, low values
    short TngUnkn104; // pos=104
    short TngUnkn106; // pos=106
    ushort VehicleMatrixIndex; // pos=108
    ushort TngUnkn110; // pos=110
    ushort UnkFrame; // pos=112
    short TngUnkn114; // pos=114
    short PersonMaxShieldEnergy;
    short TngUnkn118;
    long  TngUnkn120;
    short PersonPathIndex; // pos=124
    short TngUnkn128;
    short PersonUniqueID;
    ushort TngUnkn130; // pos=130
    short PersonShieldEnergy; // pos=132
    char PersonSpecialTimer;
    ubyte TngUnkn135;
    short PersonWeaponTurn;
    ubyte PersonBrightness; // pos=138
    ubyte PersonComRange;
    ubyte PersonBumpMode;
    ubyte PersonBumpCount;
    short PersonVehicle;
    short PersonLinkPassenger;
    ushort PersonWithin; // pos=146
    ushort PersonLastDist;
    short PersonComTimer;
    ulong PTarget; // pos=152 cleared during load
    short TngUnkn156; // pos=156
    short PersonOnFace;
    union Mod PersonUMod; // pos=160
    short PersonMood;
    struct DrawFrameId PersonFrameId;
    ubyte PersonShadows; // pos=169
    ushort VehicleTNode;
    ushort TngUnkn172;
    ushort TngUnkn174;
    short PersonMaxEnergy;
    short PersonEnergy;
    ubyte PersonShieldGlowTimer;
    ubyte PersonWeaponDir;
    ushort PersonSpecialOwner; // pos=182
    ushort PersonWorkPlace;
    ushort PersonLeisurePlace;
    short PersonWeaponTimer;
    ushort PersonMaxHealth; // pos=190
    short TngUnkn192;
    short PersonPersuadePower;
    short TngUnkn196;
    short TngUnkn198;
    ubyte PersonRecoilDir; // pos=200
    ubyte PersonCurrentWeapon;
    short TngUnkn202;
    short TngUnkn204;  // Contains vehicle data
    short TngUnkn206;  // These map to values in the third and fourth bytes of the "Dummy" vehicle value of the final level structure. These are vehicle stats that are similar to  VehiclePassengerHead
    short TngUnkn208;
    short TngUnkn210;
    short TngUnkn212; // pos=212
    short TngUnkn214;
};

#pragma pack()
/******************************************************************************/
extern ushort things_used_head;

extern struct Thing *things;
extern struct SimpleThing *sthings;

void init_things(void);
TbResult delete_node(struct Thing *p_thing);
void add_node_thing(ushort new_thing);
short get_new_thing(void);
void remove_thing(short tngno);

void add_node_sthing(ushort new_thing);
short get_new_sthing(void);
void remove_sthing(short tngno);

void refresh_old_thing_format(struct Thing *p_thing, struct ThingOldV9 *p_oldthing, ulong fmtver);

short find_nearest_from_group(struct Thing *p_person, ushort group, ubyte no_persuaded);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
