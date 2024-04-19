/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file lvfiles.c
 *     Routines for level and map files handling.
 * @par Purpose:
 *     Implement functions for handling and checking level and map files.
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
#include "lvfiles.h"

#include <assert.h>
#include <string.h>
#include "bffile.h"
#include "bfmath.h"
#include "triangls.h"
#include "trpoints.h"
#include "trstate.h"
#include "pathtrig.h"
#include "bigmap.h"
#include "campaign.h"
#include "command.h"
#include "enginlights.h"
#include "enginpriobjs.h"
#include "enginsngobjs.h"
#include "enginsngtxtr.h"
#include "enginsngobjs.h"
#include "game.h"
#include "game_data.h"
#include "lvobjctv.h"
#include "matrix.h"
#include "building.h"
#include "pepgroup.h"
#include "thing.h"
#include "vehicle.h"
#include "vehtraffic.h"
#include "swlog.h"
/******************************************************************************/

TbBool level_deep_fix = false;

extern struct QuickLoad quick_load_pc[19];


void debug_level(const char *text, int player)
{
    short thing;

    thing = things_used_head;
    while (thing != 0)
    {
        struct Thing *p_thing;

        p_thing = &things[thing];
        // TODO place debug/verification code
        thing = p_thing->LinkChild;
    }
}

ulong load_level_pc_handle(TbFileHandle lev_fh)
{
    ulong fmtver;
    TbBool mech_initialized;
    long limit;
    int i, k, n;

    mech_initialized = 0;
    fmtver = 0;
    LbFileRead(lev_fh, &fmtver, 4);

    if (fmtver >= 1)
    {
        ushort count;
        short new_thing;
        struct Thing loc_thing;
        struct Thing *p_thing;

        count = 0;
        LbFileRead(lev_fh, &count, 2);

        LOGSYNC("Level fmtver=%lu n_things=%hd", fmtver, count);
        for (i = 0; i < count; i++)
        {
            short angle;

            new_thing = get_new_thing();
            p_thing = &things[new_thing];
            memcpy(&loc_thing, p_thing, sizeof(struct Thing));
            if (fmtver >= 13) {
                assert(sizeof(struct Thing) == 168);
                LbFileRead(lev_fh, p_thing, sizeof(struct Thing));
            } else {
                struct ThingOldV9 s_oldthing;
                assert(sizeof(s_oldthing) == 216);
                LbFileRead(lev_fh, &s_oldthing, sizeof(s_oldthing));
                refresh_old_thing_format(p_thing, &s_oldthing, fmtver);
            }
            LOGNO("Thing(%hd,%hd) group %hd at (%d,%d,%d) type=%d,%d",
              p_thing->ThingOffset, p_thing->U.UPerson.UniqueID,
              p_thing->U.UPerson.Group,
              PRCCOORD_TO_MAPCOORD(p_thing->X),
              PRCCOORD_TO_MAPCOORD(p_thing->Y),
              PRCCOORD_TO_MAPCOORD(p_thing->Z),
              (int)p_thing->Type, (int)p_thing->SubType);

            if ((p_thing->Z >> 8) < 256)
                p_thing->Z += (256 << 8);
            if ((p_thing->X >> 16) >= 128)
                p_thing->X = (64 << 16);
            p_thing->PTarget = 0;
            p_thing->LinkParent = loc_thing.LinkParent;
            p_thing->LinkChild = loc_thing.LinkChild;
            // We have limited amount of group definitions
            if (p_thing->U.UObject.Group >= PEOPLE_GROUPS_COUNT)
                p_thing->U.UObject.Group = 0;
            // All relevant thing types must have the values below at same position
            p_thing->U.UObject.EffectiveGroup = p_thing->U.UObject.Group;

            if (new_thing == 0)
                continue;

            add_node_thing(new_thing);

            if (p_thing->Type == TT_PERSON)
            {
                ushort person_anim;

                if (fmtver < 15)
                    // Causes invisible NPCs when non-zero
                    p_thing->Flag2 = 0;
                p_thing->U.UPerson.Flag3 = 0;
                p_thing->Flag2 &= 0x21000000;
                if ((p_thing->Flag & TngF_Unkn02000000) != 0)
                {
                    p_thing->ThingOffset = p_thing - things;
                    remove_thing(new_thing);
                    delete_node(p_thing);
                    continue;
                }
                person_anim = people_frames[p_thing->SubType][p_thing->U.UPerson.AnimMode];
                p_thing->StartFrame = person_anim - 1;
                p_thing->Frame = nstart_ani[person_anim + p_thing->U.UPerson.Angle];
                init_person_thing(p_thing);
                p_thing->Flag |= TngF_Unkn0004;
                if (fmtver < 6)
                    p_thing->Flag |= TngF_Unkn04000000;
                if (fmtver < 12)
                    sanitize_cybmods_fmtver11_flags(&p_thing->U.UPerson.UMod);
            }

            if (p_thing->Type == SmTT_DROPPED_ITEM)
            {
                p_thing->Frame = nstart_ani[p_thing->StartFrame + 1];
            }

            if (p_thing->Type == TT_VEHICLE)
            {
                if (fmtver < 17)
                    p_thing->U.UVehicle.Armour = 4;
                p_thing->U.UVehicle.PassengerHead = 0;
                p_thing->Flag2 &= 0x1000000;
                if (fmtver <= 8)
                    p_thing->Y >>= 3;

                k = next_local_mat++;
                LbFileRead(lev_fh, &local_mats[k], 36);

                p_thing->U.UVehicle.MatrixIndex = next_local_mat - 1;
                byte_1C83D1 = 0;

                n = next_normal;
                func_6031c(PRCCOORD_TO_MAPCOORD(p_thing->X), PRCCOORD_TO_MAPCOORD(p_thing->Z),
                  -prim_unknprop01 - p_thing->StartFrame, PRCCOORD_TO_MAPCOORD(p_thing->Y));
                k = next_normal;
                unkn_object_shift_03(next_object - 1);
                unkn_object_shift_02(n, k, next_object - 1);

                k = p_thing - things;
                p_thing->U.UVehicle.Object = next_object - 1;
                game_objects[next_object - 1].ZScale = k;
                VNAV_unkn_func_207(p_thing);
                k = p_thing->U.UVehicle.MatrixIndex;
                angle = LbArcTanAngle(local_mats[k].R[0][2], local_mats[k].R[2][2]);
                p_thing->U.UVehicle.AngleY = (angle + LbFPMath_PI) & LbFPMath_AngleMask;

                veh_add(p_thing, p_thing->StartFrame);
                k = p_thing->U.UVehicle.MatrixIndex;
                angle = LbArcTanAngle(local_mats[k].R[0][2], local_mats[k].R[2][2]);
                p_thing->U.UVehicle.AngleY = (angle + LbFPMath_PI) & LbFPMath_AngleMask;
                if (p_thing->SubType == SubTT_VEH_MECH)
                {
                    if (!mech_initialized)
                    {
                        init_mech();
                        mech_unkn_func_02();
                        mech_initialized = 1;
                    }
                    mech_unkn_func_09(new_thing);
                }
                debug_level(" placed vehicle", 1);
            }
        }
    }
    LbFileRead(lev_fh, &next_command, sizeof(ushort));
    limit = get_memory_ptr_allocated_count((void **)&game_commands);
    if ((limit >= 0) && (next_command > limit)) {
        LOGERR("Restricting \"%s\", wanted %d, limit %ld", "game_commands", (int)next_command, limit);
        next_command = limit;
    }
    assert(sizeof(struct Command) == 32);
    LbFileRead(lev_fh, game_commands, sizeof(struct Command) * next_command);

    LbFileRead(lev_fh, &level_def, 44);
    for (i = 0; i < 8; i++)
    {
        if (level_def.PlayableGroups[i] >= PEOPLE_GROUPS_COUNT)
            level_def.PlayableGroups[i] = 0;
    }

    LbFileRead(lev_fh, engine_mem_alloc_ptr + engine_mem_alloc_size - 1320 - 33, 1320);
    LbFileRead(lev_fh, war_flags, 32 * sizeof(struct WarFlag));
    LbFileRead(lev_fh, &word_1531E0, sizeof(ushort));
    LOGSYNC("Level fmtver=%lu n_command=%hu word_1531E0=%hu", fmtver, next_command, word_1531E0);
    for (k = 0; k < PEOPLE_GROUPS_COUNT; k++)
    {
        for (i = 0; i < 8; i++)
        {
            if (war_flags[k].Guardians[i] >= 32)
                war_flags[k].Guardians[i] = 0;
        }
    }
    LbFileRead(lev_fh, engine_mem_alloc_ptr + engine_mem_alloc_size - 32000, 15 * word_1531E0);
    LbFileRead(lev_fh, &unkn3de_len, sizeof(ushort));
    LbFileRead(lev_fh, engine_mem_alloc_ptr + engine_mem_alloc_size - 32000, unkn3de_len);

    if (fmtver >= 4)
    {
        ulong count;
        short thing;

        count = 0;
        LbFileRead(lev_fh, &count, 2);
        for (i = count; i > 0; i--)
        {
            struct SimpleThing loc_thing;
            struct SimpleThing *p_thing;

            thing = get_new_sthing();
            p_thing = &sthings[thing];
            memcpy(&loc_thing, p_thing, 60);
            LbFileRead(lev_fh, p_thing, 60);
            if (p_thing->Type == SmTT_CARRIED_ITEM)
            {
                p_thing->LinkParent = loc_thing.LinkParent;
                p_thing->LinkChild = loc_thing.LinkChild;
                remove_sthing(thing);
            }
            else
            {
              if (p_thing->Type == SmTT_DROPPED_ITEM) {
                  p_thing->Frame = nstart_ani[p_thing->StartFrame + 1];
              }
              p_thing->LinkParent = loc_thing.LinkParent;
              p_thing->LinkChild = loc_thing.LinkChild;
              if (thing != 0)
                  add_node_sthing(thing);
            }
            LOGNO("Thing(%hd,%hd) at (%d,%d,%d) type=%d,%d",
              p_thing->ThingOffset, p_thing->UniqueID,
              PRCCOORD_TO_MAPCOORD(p_thing->X),
              PRCCOORD_TO_MAPCOORD(p_thing->Y),
              PRCCOORD_TO_MAPCOORD(p_thing->Z),
              (int)p_thing->Type, (int)p_thing->SubType);
        }
    }

    if (fmtver >= 6)
    {
        LbFileRead(lev_fh, &word_1810E4, 2);
        if (word_1810E4 < 1000)
            word_1810E4 = 1000;
        if (word_1810E4 > 9000)
            word_1810E4 = 4000;
    }

    if (fmtver >= 7) {
        LbFileRead(lev_fh, &next_used_lvl_objective, sizeof(ushort));
        assert(sizeof(struct Objective) == 32);
        LbFileRead(lev_fh, game_used_lvl_objectives, sizeof(struct Objective) * next_used_lvl_objective);
    } else {
        next_used_lvl_objective = 1;
    }

    if (fmtver >= 9) {
        LbFileRead(lev_fh, byte_1810E6, 40);
        LbFileRead(lev_fh, byte_18110E, 40);
    }

    if (fmtver >= 10)
        LbFileRead(lev_fh, game_level_miscs, 4400);

    if (fmtver >= 16)
        LbFileRead(lev_fh, &dword_176D58, 4);

    return fmtver;
}

short find_group_which_looks_like_human_player(TbBool strict)
{
    short group;
    short partial_group;
    short n_partial;

    n_partial = 0;
    for (group = 0; group < PEOPLE_GROUPS_COUNT; group++)
    {
        int n_all, n_agents, n_zealots, n_punks;

        n_all = count_people_in_group(group, -1);
        if (n_all < 1)
            continue;

        if (strict && (n_all > 4))
            continue;

        n_agents = count_people_in_group(group, SubTT_PERS_AGENT);
        if (n_agents == 4)
            return group;
        n_zealots = count_people_in_group(group, SubTT_PERS_ZEALOT);
        if (n_zealots == 4)
            return group;
        n_punks = count_people_in_group(group, SubTT_PERS_PUNK_F) +
          count_people_in_group(group, SubTT_PERS_PUNK_M);
        if (n_punks == 4)
            return group;

        if ((n_agents > 0) && (!strict || (n_agents <= 4)))
        {
            if (n_partial < n_agents) {
                partial_group = group;
                n_partial = n_agents;
            }
        }
        if ((n_zealots > 0) && (!strict || (n_zealots <= 4)))
        {
            if (n_partial < n_zealots) {
                partial_group = group;
                n_partial = n_zealots;
            }
        }
        if ((n_punks > 0) && (!strict || (n_punks <= 4)))
        {
            if (n_partial < n_punks) {
                partial_group = group;
                n_partial = n_punks;
            }
        }
    }

    if (n_partial > 0)
        return partial_group;
    return -1;
}

ushort person_add_command(short person, ubyte cmdtype)
{
    struct Thing *p_person;
    struct Command *p_cmd;
    ushort cmd;

    if (person <= 0)
        return 0;

    p_person = &things[person];

    cmd = get_new_command();
    p_cmd = &game_commands[cmd];

    p_cmd->Next = p_person->U.UPerson.ComHead;
    p_person->U.UPerson.ComHead = cmd;
    p_cmd->Type = cmdtype;

    return cmd;
}

void add_commands_from_person_states(void)
{
    struct Thing *p_thing;
    short thing;

    for (thing = things_used_head; thing > 0; thing = p_thing->LinkChild)
    {
        p_thing = &things[thing];
        // Per thing code start
        if (p_thing->Type == TT_PERSON && p_thing->U.UPerson.ComHead == 0)
        {
            ushort cmd;
            struct Command *p_cmd;

            switch (p_thing->Type)
            {
            case PerSt_WANDER:
                cmd = person_add_command(thing, PCmd_WAND_TIME);
                p_cmd = &game_commands[cmd];
                p_cmd->Time = 9999;
                break;
            default:
                break;
            }
        }
        // Per thing code end
    }
}

void level_perform_deep_fix(void)
{
    {
        short pv_group, nx_group;
        TbBool need_fix;
        int i;

        pv_group = level_def.PlayableGroups[0];
        i = count_people_in_group(pv_group, -1);
        need_fix = (i < 1) || (i > 4);
        if (need_fix) {
            nx_group = find_group_which_looks_like_human_player(true);
            if (nx_group >= 0) {
                LOGWARN("Local player group %d has no team; switching to better group %d",
                  (int)pv_group, (int)nx_group);
                level_def.PlayableGroups[0] = nx_group;
                need_fix = false;
            }
        }
        if (need_fix) {
            short lp_group;
            int n;

            nx_group = find_group_which_looks_like_human_player(false);
            lp_group = -1;
            if (nx_group >= 0)
                lp_group = find_unused_group_id(true);
            if (lp_group >= 0) {
                thing_group_copy(pv_group, nx_group, 0x01|0x02|0x04);
                n = thing_group_transfer_people(nx_group, lp_group, SubTT_PERS_AGENT, 0, 4);
                if (n <= 0)
                    n = thing_group_transfer_people(nx_group, lp_group, SubTT_PERS_ZEALOT, 0, 4);
                if (n <= 0)
                    n = thing_group_transfer_people(nx_group, lp_group, -1, 0, 4);
                if (n > 0) {
                    LOGWARN("Local player group %d has no team; switching to new group %d based on %d",
                      (int)pv_group, (int)lp_group, (int)nx_group);
                    level_def.PlayableGroups[0] = lp_group;
                    need_fix = false;
                }
            }
        }
        if (need_fix) {
            LOGWARN("Local player group %d has no team; fix attempts failed",
              (int)pv_group);
        }
    }

#if 0
    // While the group 0 sometimes looks suspicious, it generally works correctly
    // We should transfer other things away from that group, instead. So, this got disabled.
    // Also, switching the group would require fixing its index in all objectives and commands.
    if (level_def.PlayableGroups[0] == 0) {
        short pv_group, nx_group;

        pv_group = level_def.PlayableGroups[0];
        nx_group = find_unused_group_id(true);
        LOGWARN("Local player group is %d; switching to %d",
          (int)pv_group, (int)nx_group);
        if (nx_group > 0) {
            thing_group_copy(pv_group, nx_group, 0x01|0x02);
            thing_group_transfer_people(pv_group, nx_group, -1, 0, 4);
        }
    }
#endif
}

void fix_level_indexes(short missi, ulong fmtver, ubyte reload, TbBool deep)
{
    ushort objectv;
    short thing;

    fix_thing_commands_indexes(deep);

    for (objectv = 1; objectv < next_used_lvl_objective; objectv++)
    {
        struct Objective *p_objectv;

        p_objectv = &game_used_lvl_objectives[objectv];
        p_objectv->Level = (current_level - 1) % 15 + 1;
        p_objectv->Map = current_map;
        fix_single_objective(p_objectv, objectv, "UL");
    }

    for (objectv = 1; objectv < next_objective; objectv++)
    {
        struct Objective *p_objectv;

        p_objectv = &game_objectives[objectv];
        fix_single_objective(p_objectv, objectv, "S");
    }

    // Used objectives are not part of the level file, but part of
    // the mission file. If restarting level, leave these intact,
    // as fixups were already applied on first load.
    if (!reload)
    {
        fix_mission_used_objectives(missi);
    }

    for (thing = 1; thing < THINGS_LIMIT; thing++)
    {
        things[thing].ThingOffset = thing;
    }

    for (thing = 1; thing < STHINGS_LIMIT; thing++)
    {
        sthings[-thing].ThingOffset = -thing;
    }
}

void level_misc_update(void)
{
    asm volatile ("call ASM_level_misc_update\n"
        :  :  : "eax" );
}

void load_level_pc(short level, short missi, ubyte reload)
{
    short next_level, prev_level;
    TbFileHandle lev_fh;
    char lev_fname[52];

    next_level = level;
    gameturn = 0;

    prev_level = current_level;
    if (next_level < 0)
    {
        next_level = -next_level;
        word_1C8446 = 1;
        if (next_level <= 15)
            sprintf(lev_fname, "%s/c%03dl%03d.dat", game_dirs[DirPlace_Levels].directory,
                current_map, next_level);
        else
            sprintf(lev_fname, "%s/c%03dl%03d.d%d", game_dirs[DirPlace_Levels].directory,
               current_map, (next_level - 1) % 15 + 1, (next_level - 1) / 15);
        if (next_level > 0)
            current_level = next_level;
    }
    if (next_level <= 0) {
        LOGERR("Next level index is not positive, load skipped");
        return;
    }
    LOGSYNC("Next level %hd prev level %hd mission %hd reload 0x%x",
      next_level, prev_level, missi, (uint)reload);

    /* XXX: This fixes the inter-mission memory corruption bug
     * mefisto: No idea what "the" bug is, to be tested and described properly (or re-enabled)
     */
#if 0
    if ((ingame.Flags & GamF_Unkn0008) == 0)
    {
        if (prev_level)
            global_3d_store(1);
        global_3d_store(0);
    }
#else
    (void)prev_level; // avoid unused var warning
#endif
    debug_level(" load level restart coms", 1);

    lev_fh = LbFileOpen(lev_fname, Lb_FILE_MODE_READ_ONLY);
    if (lev_fh != INVALID_FILE)
    {
        ulong fmtver;
        int i;

        word_1C8446 = 1;
        word_176E38 = 0;

        fmtver = load_level_pc_handle(lev_fh);

        LbFileClose(lev_fh);

        if (fmtver < 5)
            add_commands_from_person_states();

        if (fmtver <= 10)
        {
            for (i = 1; i < next_command; i++)
                game_commands[i].Arg2 = 1;
            for (i = 1; i < next_used_lvl_objective; i++)
                game_used_lvl_objectives[i].Arg2 = 1;
        }

        check_and_fix_commands();

        build_same_type_headers();

        check_and_fix_thing_commands();

        if (fmtver >= 10)
            level_misc_update();

        if (level_deep_fix) {
            level_perform_deep_fix();
        }
        fix_level_indexes(missi, fmtver, reload, level_deep_fix);
    } else
    {
        LOGERR("Could not open mission file, load skipped");
    }
}

void fix_map_outranged_properties(void)
{
    ushort tile_x, tile_y;
    int i;

    for (tile_y = 0; tile_y < MAP_TILE_HEIGHT; tile_y++) {
        for (tile_x = 0; tile_x < MAP_TILE_WIDTH; tile_x++) {
            struct MyMapElement *p_mapel;
            ushort texture;

            p_mapel = &game_my_big_map[tile_y * MAP_TILE_WIDTH + tile_x];
            texture = p_mapel->Texture & 0x3FFF;
            if (texture >= next_floor_texture) {
                LOGERR("Outranged texture %d used in mapel at %d,%d",
                  (int)texture, (int)tile_x, (int)tile_y);
                p_mapel->Texture &= 0xC000;
                p_mapel->Texture |= texture % next_floor_texture;
            }
        }
    }
    for (i = 0; i < next_object_face; i++) {
        struct SingleObjectFace3 *p_face;
        ushort texture;

        p_face = &game_object_faces[i];
        texture = p_face->Texture & 0x3FFF;
        if (texture >= next_face_texture) {
            LOGERR("Outranged texture %d used in face3 %d", (int)texture, (int)i);
            p_face->Texture &= 0xC000;
            p_face->Texture |= texture % next_face_texture;
        }
    }
    for (i = 0; i < next_object_face4; i++) {
        struct SingleObjectFace4 *p_face;
        ushort texture;

        p_face = &game_object_faces4[i];
        texture = p_face->Texture & 0x3FFF;
        if (texture >= next_floor_texture) {
            LOGERR("Outranged texture %d used in face4 %d", (int)texture, (int)i);
            p_face->Texture &= 0xC000;
            p_face->Texture |= texture % next_floor_texture;
        }
    }
    for (i = 0; i < next_floor_texture; i++) {
        struct SingleFloorTexture *p_fltextr;
        ushort page;

        p_fltextr = &game_textures[i];
        page = p_fltextr->Page;
        if (page >= sizeof(vec_tmap)/sizeof(vec_tmap[0]) || vec_tmap[page] == NULL) {
            LOGERR("Outranged texture atlas page %d used in floor texture %d", (int)page, (int)i);
            p_fltextr->Page = 0;
        }
    }
    for (i = 0; i < next_face_texture; i++) {
        struct SingleTexture *p_fctextr;
        ushort page;

        p_fctextr = &game_face_textures[i];
        page = p_fctextr->Page;
        if (page >= sizeof(vec_tmap)/sizeof(vec_tmap[0]) || vec_tmap[page] == NULL) {
            LOGERR("Outranged texture atlas page %d used in face texture %d", (int)page, (int)i);
            p_fctextr->Page = 0;
        }
    }
}

void load_map_dat_pc_handle(TbFileHandle fh)
{
    ulong fmtver;
    ushort num_sthings, num_things;
    short i;

    LbFileRead(fh, &fmtver, sizeof(fmtver));
    if (fmtver >= 19)
    {
        assert(sizeof(struct MyMapElement) == 18);
        LbFileRead(fh, game_my_big_map, sizeof(struct MyMapElement) * 128 * 128);
    }
    else
    {
        struct MyMapElementOldV7 old_my_map_el;
        assert(sizeof(old_my_map_el) == 24);
        for (i = 0; i < 128 * 128; i++) {
            LbFileRead(fh, &old_my_map_el, sizeof(old_my_map_el));
            refresh_old_my_big_map_format(&game_my_big_map[i], &old_my_map_el, fmtver);
        }
    }
    if (fmtver <= 14)
    {
        struct SingleFloorTexture old_floor_texture; // Same fields, only the textures need matching
        LbFileRead(fh, &next_floor_texture, sizeof(next_floor_texture));
        for (i = 0; i < next_floor_texture; i++) {
            LbFileRead(fh, &old_floor_texture, sizeof(old_floor_texture));
            refresh_old_floor_texture_format(&game_textures[i], &old_floor_texture, fmtver);
        }
    }
    else
    {
        LbFileRead(fh, &next_floor_texture, sizeof(next_floor_texture));
        LbFileRead(fh, game_textures, sizeof(struct SingleFloorTexture) * next_floor_texture);
    }

    if (fmtver <= 14)
    {
        struct SingleTexture old_face_texture; // Same fields, only the textures need matching
        LbFileRead(fh, &next_face_texture, sizeof(next_face_texture));
        for (i = 0; i < next_face_texture; i++) {
            LbFileRead(fh, &old_face_texture, sizeof(old_face_texture));
            refresh_old_face_texture_format(&game_face_textures[i], &old_face_texture, fmtver);
        }
    }
    else
    {
        LbFileRead(fh, &next_face_texture, sizeof(next_face_texture));
        LbFileRead(fh, game_face_textures, sizeof(struct SingleTexture) * next_face_texture);
    }
    {
        LbFileRead(fh, &next_object_point, sizeof(next_object_point));
        LbFileRead(fh, game_object_points, sizeof(struct SinglePoint) * next_object_point);
    }
    if (fmtver >= 19)
    {
        LbFileRead(fh, &next_object_face, sizeof(next_object_face));
        assert(sizeof(struct SingleObjectFace3) == 32);
        LbFileRead(fh, game_object_faces, sizeof(struct SingleObjectFace3) * next_object_face);
    }
    else
    {
        struct SingleObjectFace3OldV7 old_object_face;
        LbFileRead(fh, &next_object_face, sizeof(next_object_face));
        assert(sizeof(old_object_face) == 48);
        for (i = 0; i < next_object_face; i++) {
            LbFileRead(fh, &old_object_face, sizeof(old_object_face));
            refresh_old_object_face_format(&game_object_faces[i], &old_object_face, fmtver);
        }
    }
    {
        LbFileRead(fh, &next_object, sizeof(next_object));
        LbFileRead(fh, game_objects, sizeof(struct SingleObject) * next_object);
    }
    {
        LbFileRead(fh, &next_quick_light, sizeof(next_quick_light));
        LbFileRead(fh, game_quick_lights, sizeof(struct QuickLight) * next_quick_light);
    }
    if (fmtver >= 14)
    {
        LbFileRead(fh, &next_full_light, sizeof(next_full_light));
        assert(sizeof(struct FullLight) == 32);
        LbFileRead(fh, game_full_lights, sizeof(struct FullLight) * next_full_light);
    }
    else
    {
        struct FullLightOldV12 old_full_light;
        LbFileRead(fh, &next_full_light, sizeof(next_full_light));
        assert(sizeof(old_full_light) == 40);
        for (i = 0; i < next_full_light; i++) {
            LbFileRead(fh, &old_full_light, sizeof(old_full_light));
            refresh_old_full_light_format(&game_full_lights[i], &old_full_light, fmtver);
        }
    }
    {
        LbFileRead(fh, &next_normal, sizeof(next_normal));
        LbFileRead(fh, game_normals, sizeof(struct Normal) * next_normal);
    }
    if (fmtver >= 19)
    {
        LbFileRead(fh, &next_object_face4, sizeof(next_object_face4));
        assert(sizeof(struct SingleObjectFace4) == 40);
        LbFileRead(fh, game_object_faces4, sizeof(struct SingleObjectFace4) * next_object_face4);
    }
    else if (fmtver >= 7)
    {
        struct SingleObjectFace4OldV7 old_object_face4;
        LbFileRead(fh, &next_object_face4, sizeof(next_object_face4));
        assert(sizeof(old_object_face4) == 60);
        for (i = 0; i < next_object_face4; i++) {
            LbFileRead(fh, &old_object_face4, sizeof(old_object_face4));
            refresh_old_object_face4_format(&game_object_faces4[i], &old_object_face4, fmtver);
        }
    }
    else
    {
        next_object_face4 = 1;
    }
    LOGSYNC("stats: object_faces=%hu objects=%hu quick_lights=%hu full_lights=%hu normals=%hu object_faces4=%hu",
      next_object_face, next_object, next_quick_light, next_full_light, next_normal, next_object_face4);

    clear_mapwho_on_whole_map();

    if (fmtver >= 5)
    {
        struct SimpleThing loc_sthing;

        LbFileRead(fh, &num_sthings, sizeof(num_sthings));
        assert(sizeof(struct SimpleThing) == 60);
        for (i = num_sthings - 1; i != -1; i--)
        {
            LbFileRead(fh, &loc_sthing, sizeof(struct SimpleThing));
            switch (loc_sthing.Type)
            {
            case SmTT_TRAFFIC:
                new_thing_traffic_clone(&loc_sthing);
                break;
            case SmTT_SMOKE_GENERATOR:
                new_thing_smoke_gen_clone(&loc_sthing);
                break;
            case SmTT_STATIC:
                new_thing_static_clone(&loc_sthing);
                break;
            default:
                {
                  char locbuf[256];
                  snprint_sthing(locbuf, sizeof(locbuf), &loc_sthing);
                  LOGWARN("Discarded: %s", locbuf);
                }
                break;
            }
        }
    }
    else
    {
        num_sthings = 0;
    }

    if (fmtver >= 6)
    {
        LbFileRead(fh, &next_anim_tmap, sizeof(next_anim_tmap));
        assert(sizeof(struct AnimTmap) == 54);
        LbFileRead(fh, game_anim_tmaps, sizeof(struct AnimTmap) * next_anim_tmap);
    }
    else
    {
        next_anim_tmap = 1;
        game_anim_tmaps[1].Texture = 0;
    }

    if (fmtver >= 19)
    {
        ushort num_things;
        struct Thing loc_thing;
        struct M33 loc_mat;
        short shut_h;

        shut_h = 100;
        LbFileRead(fh, &num_things, sizeof(num_things));
        assert(sizeof(struct Thing) == 168);
        for (i = num_things - 1; i > -1; i--)
        {
            LbFileRead(fh, &loc_thing, sizeof(struct Thing));
            if (loc_thing.U.UObject.Object <= 0) {
                char locbuf[256];
                snprint_thing(locbuf, sizeof(locbuf), &loc_thing);
                LOGWARN("Bad object %d in %s",
                  (int)loc_thing.U.UObject.Object, locbuf);
                continue;
            }
            switch (loc_thing.SubType)
            {
            case SubTT_BLD_36:
            case SubTT_BLD_37:
                LbFileRead(fh, &loc_mat, sizeof(struct M33));
                new_thing_building_clone(&loc_thing, &loc_mat, shut_h);
                break;
            default:
                new_thing_building_clone(&loc_thing, NULL, shut_h);
                break;
            }
        }
    }
    else if (fmtver >= 8)
    {
        struct Thing loc_thing;
        struct ThingOldV9 old_thing;

        LbFileRead(fh, &num_things, sizeof(num_things));
        assert(sizeof(old_thing) == 216);
        for (i = num_things - 1; i > -1; i--)
        {
            struct SingleObject *p_sobj;

            LbFileRead(fh, &old_thing, sizeof(old_thing));
            //TODO map fmtver is unlikely to match level fmtver
            refresh_old_thing_format(&loc_thing, &old_thing, fmtver);
            if (loc_thing.U.UObject.Object <= 0) {
                char locbuf[256];
                snprint_thing(locbuf, sizeof(locbuf), &loc_thing);
                LOGWARN("Bad object %d in %s",
                  (int)loc_thing.U.UObject.Object, locbuf);
                continue;
            }
            p_sobj = &game_objects[loc_thing.U.UObject.Object];
            if (p_sobj->OffsetY == 32000) {
                continue;
            }
            new_thing_building_clone(&loc_thing, NULL, 100);
        }
    }
    else
    {
        num_things = 0;
    }

    if (fmtver >= 6)
    {
        LbFileRead(fh, &next_traffic_node, sizeof(next_traffic_node));
        assert(sizeof(struct TrafficNode) == 36);
        LbFileRead(fh, game_traffic_nodes, sizeof(struct TrafficNode) * next_traffic_node);
    }
    else
    {
        next_traffic_node = 1;
    }

    if (fmtver >= 12)
    {
        LbFileRead(fh, &next_light_command, sizeof(next_light_command));
        assert(sizeof(struct LightCommand) == 36);
        LbFileRead(fh, game_light_commands, sizeof(struct LightCommand) * next_light_command);
    }
    else
    {
       next_light_command = 1;
    }
    if (fmtver >= 13)
    {
        LbFileRead(fh, &next_bezier_pt, sizeof(next_bezier_pt));
        assert(sizeof(struct BezierPt) == 28);
        LbFileRead(fh, bezier_pts, sizeof(struct BezierPt) * next_bezier_pt);
    }
    else
    {
        next_bezier_pt = 1;
    }
    LOGSYNC("stats: sthings=%hu things=%hu traffic_nodes=%hu light_commands=%hu bezier_pts=%hu",
      num_sthings, num_things, next_traffic_node, next_light_command, next_bezier_pt);
}

void load_mad_pc_buffer(ubyte *mad_ptr, long rdsize)
{
    short shut_h;
    ulong fmtver;
    short i;

    shut_h = 100;
    fmtver = *(ulong *)mad_ptr;
    mad_ptr += 4;

    if (fmtver != 1) {
        LOGWARN("Unexpected MAD version %lu", fmtver);
    }

    // Set amounts of quick_load array items
    for (i = 0; quick_load_pc[i].Size != 0; i++)
    {
        ushort *p_numb;
        p_numb = quick_load_pc[i].Numb;
        if (p_numb != NULL) {
            *p_numb = *(ushort *)mad_ptr;
            mad_ptr += 2;
        }
    }
    // Set data pointers within the quick_load items
    for (i = 0; quick_load_pc[i].Size != 0; i++)
    {
        int entsize, nentries;
        ushort *p_numb;
        *quick_load_pc[i].Ptr = mad_ptr;
        p_numb = quick_load_pc[i].Numb;
        if (p_numb != NULL) {
            entsize = quick_load_pc[i].Size;
            nentries = quick_load_pc[i].Extra + *p_numb;
        } else {
            nentries = quick_load_pc[i].Size;
            entsize = quick_load_pc[i].Extra;
        }
        mad_ptr += nentries * entsize + 2;
    }
    // Update mem_game[] to the new amounts
    for (i = 1; i < 17; i++)
    {
        ushort *p_numb;
        p_numb = quick_load_pc[i].Numb;
        mem_game[i].N = quick_load_pc[i].Extra + *p_numb;
    }

    memcpy(&selected_triangulation_no, mad_ptr, sizeof(selected_triangulation_no));
    mad_ptr += sizeof(selected_triangulation_no);
    memcpy(&triangulation_initied, mad_ptr, sizeof(triangulation_initied));
    mad_ptr += sizeof(triangulation_initied);
    assert(sizeof(struct Triangulation) == 60);
    memcpy(triangulation, mad_ptr, sizeof(struct Triangulation) * 4);
    mad_ptr += sizeof(struct Triangulation) * 4;
    assert(sizeof(struct TrTriangle) == 16);
    triangulation[0].Triangles = (struct TrTriangle *)mad_ptr;
    mad_ptr += sizeof(struct TrTriangle) * triangulation[0].max_Triangles;
    triangulation[1].Triangles = (struct TrTriangle *)mad_ptr;
    mad_ptr += sizeof(struct TrTriangle) * triangulation[1].max_Triangles;
    triangulation[2].Triangles = (struct TrTriangle *)mad_ptr;
    mad_ptr += sizeof(struct TrTriangle) * triangulation[2].max_Triangles;
    triangulation[3].Triangles = (struct TrTriangle *)mad_ptr;
    mad_ptr += sizeof(struct TrTriangle) * triangulation[3].max_Triangles;
    assert(sizeof(struct TrPoint) == 8);
    triangulation[0].Points = (struct TrPoint *)mad_ptr;
    mad_ptr += sizeof(struct TrPoint) * triangulation[0].max_Points;
    triangulation[1].Points = (struct TrPoint *)mad_ptr;
    mad_ptr += sizeof(struct TrPoint) * triangulation[1].max_Points;
    triangulation[2].Points = (struct TrPoint *)mad_ptr;
    mad_ptr += sizeof(struct TrPoint) * triangulation[2].max_Points;
    triangulation[3].Points = (struct TrPoint *)mad_ptr;
    mad_ptr += sizeof(struct TrPoint) * triangulation[3].max_Points;

    clear_mapwho_on_whole_map();

    ushort num_sthings;
    struct SimpleThing *p_clsthing;

    dword_177750 = mad_ptr;
    num_sthings = *(ushort *)mad_ptr;
    mad_ptr += 2;
    for (i = num_sthings - 1; i > -1; i--)
    {
        p_clsthing = (struct SimpleThing *)mad_ptr;
        mad_ptr += sizeof(struct SimpleThing);
        switch (p_clsthing->Type)
        {
        case SmTT_TRAFFIC:
            new_thing_traffic_clone(p_clsthing);
            break;
        case SmTT_SMOKE_GENERATOR:
            new_thing_smoke_gen_clone(p_clsthing);
            break;
        case SmTT_STATIC:
            new_thing_static_clone(p_clsthing);
            break;
        default:
            {
                char locbuf[256];
                snprint_sthing(locbuf, sizeof(locbuf), p_clsthing);
                LOGWARN("Discarded: %s", locbuf);
            }
            break;
        }
    }

    ushort num_things;
    struct Thing *p_clthing;

    num_things = *(ushort *)mad_ptr;
    mad_ptr += 2;
    for (i = num_things - 1; i > -1; i--)
    {
        p_clthing = (struct Thing *)mad_ptr;
        mad_ptr += sizeof(struct Thing);
        if (p_clthing->U.UObject.Object <= 0)
            continue;
        switch (p_clthing->SubType)
        {
        case SubTT_BLD_36:
        case SubTT_BLD_37:
            new_thing_building_clone(p_clthing, (struct M33 *)mad_ptr, shut_h);
            mad_ptr += sizeof(struct M33);
            break;
        default:
            new_thing_building_clone(p_clthing, NULL, shut_h);
            break;
        }
    }

    if (mad_ptr - (ubyte *)dword_177750 >= 100000)
        unkn_mech_arr7 = mad_ptr;
    else
        unkn_mech_arr7 = dword_177750 + 100000;
}

TbResult load_map_dat(ushort mapno)
{
    char dat_fname[52];
    TbFileHandle fh;

    next_local_mat = 1;

    sprintf(dat_fname, "%s/map%03d.dat", "maps", mapno);
    fh = LbFileOpen(dat_fname, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE) {
        return Lb_FAIL;
    }
    load_map_dat_pc_handle(fh);
    LbFileClose(fh);

    fix_map_outranged_properties();

    return Lb_SUCCESS;
}

void prepare_map_dat_to_play(void)
{
    update_map_thing_and_traffic_refs();
    unkn_lights_processing();
    update_map_flags();

    triangulation_initied = 0;
    selected_triangulation_no = -1;
    triangulation_allocate(1,9000);
    triangulation_allocate(2,9000);

    triangulation_select(2);
    triangulation_clear();
    generate_map_triangulation();

    triangulation_select(1);
    triangulation_clear();
    generate_map_triangulation();
}

TbResult load_map_mad(ushort mapno)
{
    char mad_fname[52];
    long fsize;

    next_local_mat = 1;

    sprintf(mad_fname, "%s/map%03d.mad", "maps", mapno);
    fsize = LbFileLoadAt(mad_fname, scratch_malloc_mem);
    if (fsize == Lb_FAIL)
        return Lb_FAIL;

    load_mad_pc_buffer(scratch_malloc_mem, fsize);

    update_map_thing_and_traffic_refs();
    unkn_lights_processing();
    triangulation_select(1);

    return Lb_SUCCESS;
}

void load_map_bnb(int a1)
{
    asm volatile ("call ASM_load_map_bnb\n"
        : : "a" (a1));
}

TbResult load_mad_pc(ushort mapno)
{
    TbResult ret;

    ingame.Flags |= GamF_Unkn00010000;
    init_free_explode_faces();
    if (mapno != 0) {
        load_map_bnb(mapno);
        ret = load_map_mad(mapno);
    } else {
        ret = Lb_OK;
    }
    sub_73C64("", 1);
    return ret;
}

/******************************************************************************/
