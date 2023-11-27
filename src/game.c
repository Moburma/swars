#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "bfdata.h"
#include "bfsprite.h"
#include "bfscreen.h"
#include "bfkeybd.h"
#include "bfmouse.h"
#include "bftext.h"
#include "bfbox.h"
#include "bfpalette.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bffile.h"
#include "bffont.h"
#include "bfgentab.h"
#include "bfwindows.h"
#include "bfpng.h"
#include "bfutility.h"
#include "bfsvaribl.h"
#include "bfmath.h"
#include "bfline.h"
#include "bfaudio.h"
#include "bfmusic.h"
#include "bfsound.h"
#include "ssampply.h"
#include "svesa.h"
#include "swlog.h"
#include "bflib_fmvids.h"
#include "bflib_vidraw.h"
#include "bfscd.h"
#include "bflib_joyst.h"
#include "matrix.h"
#include "drawtext.h"
#include "engintrns.h"
#include "game_data.h"
#include "guiboxes.h"
#include "guitext.h"
#include "febrief.h"
#include "fedebrief.h"
#include "fecntrls.h"
#include "fecryo.h"
#include "feequip.h"
#include "felogin.h"
#include "fenet.h"
#include "feoptions.h"
#include "fepause.h"
#include "feresearch.h"
#include "festorage.h"
#include "feworld.h"
#include "building.h"
#include "campaign.h"
#include "cybmod.h"
#include "pepgroup.h"
#include "lvobjctv.h"
#include "bigmap.h"
#include "display.h"
#include "dos.h"
#include "game.h"
#include "game_data.h"
#include "keyboard.h"
#include "mouse.h"
#include "network.h"
#include "sound.h"
#include "unix.h"
#include "util.h"
#include "windows.h"
#include "command.h"
#include "research.h"
#include "thing.h"
#include "packet.h"
#include "player.h"
#include "vehicle.h"
#include "wadfile.h"
#include "weapon.h"
#include "wrcities.h"

#include "timer.h"

#define SAVEGAME_PATH "qdata/savegame/"

#pragma pack(1)

struct GamePanel
{
  short X;
  short Y;
  short Spr;
  ushort Width;
  ushort Height;
  ushort Use;
  ushort Flags;
  ubyte ID;
  ubyte Type;
};

struct PacketFileHead {
    ulong magic;
    ushort campgn;
    ushort missi;
    ushort mapno;
    ushort levelno;
};

struct Element;
struct Frame;

#pragma pack()

extern char *fadedat_fname;
extern char *pop_dat_fname_fmt;
extern char *pop_tab_fname_fmt;
extern unsigned long unkn_buffer_04;

extern ubyte *small_font_data;
extern ubyte *small2_font_data;
extern ubyte *small_med_font_data;
extern ubyte *med_font_data;
extern ubyte *med2_font_data;
extern ubyte *big_font_data;

extern ubyte *pointer_data;
extern struct TbSprite *sprites_Icons0_0;
extern struct TbSprite *sprites_Icons0_0_end;
extern struct TbSprite *pop1_sprites;
extern struct TbSprite *pop1_sprites_end;
extern ubyte *pop1_data;

extern ubyte *m_spr_data;
extern ubyte *m_spr_data_end;
extern struct TbSprite *m_sprites;
extern struct TbSprite *m_sprites_end;

extern ulong stored_l3d_next_object[1];
extern ulong stored_l3d_next_object_face[1];
extern ulong stored_l3d_next_object_face4[1];
extern ulong stored_l3d_next_object_point[1];
extern ulong stored_l3d_next_normal[1];
extern ulong stored_l3d_next_face_texture[1];
extern ulong stored_l3d_next_floor_texture[1];
extern ulong stored_l3d_next_local_mat[1];
extern ulong stored_level3d_inuse;

extern ulong stored_g3d_next_object[1];
extern ulong stored_g3d_next_object_face[1];
extern ulong stored_g3d_next_object_face4[1];
extern ulong stored_g3d_next_object_point[1];
extern ulong stored_g3d_next_normal[1];
extern ulong stored_g3d_next_face_texture[1];
extern ulong stored_g3d_next_floor_texture[1];
extern ulong stored_g3d_next_local_mat[1];
extern ulong stored_global3d_inuse[1];

extern unsigned char *display_palette;
extern unsigned short unkn2_pos_x;
extern unsigned short unkn2_pos_y;
extern unsigned short unkn2_pos_z;
extern int data_1c8428;
extern const char *primvehobj_fname;
extern unsigned char textwalk_data[640];

extern short save_slot; // = -1;
extern char save_slot_names[8][25];
extern long save_slot_base;
extern short word_1C6E08;
extern short word_1C6E0A;

extern ubyte byte_1C83D1;
extern ubyte byte_1CAB64[];
extern ubyte byte_1DB088[];
extern long dword_1DC36C;
extern long sound_heap_size;
extern struct SampleTable *sound_heap_memory;

extern struct GamePanel *game_panel;
extern struct GamePanel game_panel_lo[];
extern struct GamePanel unknstrct7_arr2[];

extern ushort word_1810E4;
extern ubyte byte_1810E6[40];
extern ubyte byte_18110E[40];

extern ushort unkn3de_len;
extern void *dword_177750;

extern ubyte execute_commands;
extern long gamep_unknval_10;
extern long gamep_unknval_11;
extern long gamep_unknval_12;
extern long nav_stats__ThisTurn;
extern long gamep_unknval_14;
extern long gamep_unknval_15;
extern long gamep_unknval_16;

extern ubyte player_unkn0C9[8];
extern char player_unknCC9[8][128];
extern long scanner_unkn370;
extern long scanner_unkn3CC;

extern ushort netgame_agent_pos_x[8][4];
extern ushort netgame_agent_pos_y[8][4];

extern long dword_176D58;
extern ubyte byte_153198;

extern ubyte unkn_changing_color_1;
extern ubyte unkn_changing_color_2;
extern ulong unkn_changing_color_counter1;

extern short brightness;
extern long game_speed;

struct TbLoadFiles unk02_load_files[] =
{
  { "*VESA",			(void **)&lbVesaData,		(void **)NULL,LB_VESA_DATA_SIZE, 1, 0 },
#if 1 // !defined(BFLIB_WSCREEN_CONTROL)
  { "*W_SCREEN",		(void **)&lbDisplay.WScreen,(void **)NULL,MAX_SUPPORTED_SCREEN_WIDTH*(MAX_SUPPORTED_SCREEN_HEIGHT+1), 0, 0 },
#endif
  { "data/pop2-1.dat",	(void **)&pop1_data,		(void **)NULL,			0, 0, 0 },
  { "data/pop2-1.tab",	(void **)&pop1_sprites,		(void **)&pop1_sprites_end, 0, 0, 0 },
  { "data/mspr-0.dat",	(void **)&m_spr_data,		(void **)&m_spr_data_end,0, 0, 0 },
  { "data/mspr-0.tab",	(void **)&m_sprites,		(void **)&m_sprites_end,0, 0, 0 },
  { "data/mele-0.ani",	(void **)&melement_ani,		(void **)&mele_ani_end,	0, 0, 0 },
  { "data/nsta-0.ani",	(void **)&nstart_ani,		(void **)&nstart_ani_end,0, 0, 0 },
  { "data/nfra-0.ani",	(void **)&frame,			(void **)&frame_end,	0, 0, 0 },
  { "data/font0-0.dat",	(void **)&small_font_data,	(void **)NULL,			0, 0, 0 },
  { "data/font0-0.tab",	(void **)&small_font,		(void **)&small_font_end, 0, 0, 0 },
  { "data/pointers.dat",(void **)&pointer_data,		(void **)NULL,			0, 0, 0 },
  { "data/pointers.tab",(void **)&pointer_sprites,	(void **)&pointer_sprites_end, 0, 0, 0 },
  { "qdata/pal.pal",	(void **)&display_palette,	(void **)NULL,			0, 0, 0 },
  { "",					(void **)NULL, 				(void **)NULL,			0, 0, 0 }
};

TbBool level_deep_fix = false;

extern TbFileHandle packet_rec_fh;
ushort packet_rec_no = 0;

char unk_credits_text_s[] = "";
char unk_credits_text_z[] = "";
char unk_credits_text_p[] = "";

// Original sizes of scanner in low res 64x62, high res 129x119

/** Width of the scanner map area, in percentage of screen. */
ubyte scanner_width_pct = 20;
/** Height of the scanner map area, in percentage of screen. */
ubyte scanner_height_pct = 25;

void PacketRecord_Close(void)
{
    if (in_network_game)
        return;
    LbFileClose(packet_rec_fh);
}

void PacketRecord_OpenWrite(void)
{
#if 0
    asm volatile ("call ASM_PacketRecord_OpenWrite\n"
        :  :  : "eax" );
#endif
    char fname[DISKPATH_SIZE];
    struct Mission *p_missi;
    struct PacketFileHead head;
    int file_no;

    head.magic = 0x544B4350; // 'PCKT'
    head.campgn = background_type;
    head.missi = ingame.CurrentMission;
    p_missi = &mission_list[head.missi];
    head.mapno = p_missi->MapNo;
    head.levelno = p_missi->LevelNo;

    file_no = get_highest_used_packet_record_no(head.campgn, head.missi);
    packet_rec_no = file_no + 1;
    get_packet_record_fname(fname, head.campgn, head.missi, packet_rec_no);
    LOGDBG("%s: Opening for packet save", fname);
    packet_rec_fh = LbFileOpen(fname, Lb_FILE_MODE_NEW);
    LbFileWrite(packet_rec_fh, &head, sizeof(head));
}

void PacketRecord_OpenRead(void)
{
    char fname[DISKPATH_SIZE];
    struct Mission *p_missi;
    struct PacketFileHead head;

    head.campgn = background_type;
    head.missi = ingame.CurrentMission;
    get_packet_record_fname(fname, head.campgn, head.missi, packet_rec_no);
    LOGDBG("%s: Opening for packet input", fname);
    packet_rec_fh = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    if (packet_rec_fh == INVALID_FILE) {
        LOGERR("Packet file open failed");
        pktrec_mode = PktR_NONE;
        exit_game = true;
        return;
    }
    LbFileRead(packet_rec_fh, &head, sizeof(head));
    if (head.missi < next_mission)
        p_missi = &mission_list[head.missi];
    else
        p_missi = &mission_list[0];
    if (head.magic != 0x544B4350)
        LOGWARN("%s: Packet file has bad magic value", fname);
    if ((head.campgn != background_type) && (head.campgn != 0xFFFF))
        LOGWARN("Packet file expects campaign %hu, not %d",
          fname, head.campgn, (int)background_type);
    if ((head.missi != ingame.CurrentMission) && (head.missi != 0xFFFF))
        LOGWARN("Packet file expects mission %hu, not %d",
          fname, head.missi, (int)ingame.CurrentMission);
    if ((head.mapno != p_missi->MapNo) && (head.mapno != 0xFFFF))
        LOGWARN("Packet file expects map %hu, not %d",
          fname, head.mapno, (int)p_missi->MapNo);
    if ((head.levelno != p_missi->LevelNo) && (head.levelno != 0xFFFF))
        LOGWARN("Packet file expects level %hu, not %d",
          fname, head.levelno, (int)p_missi->LevelNo);
}

void debug_trace_place(int place)
{
    LOGDBG("reached place %d", place);
}

short angle_between_points(int x1, int z1, int x2, int z2)
{
  return LbArcTanAngle(x2 - x1, z1 - z2);
}

bool game_initialise(void)
{
    TbResult ret;
    ret = LbBaseInitialise();
    if (ret != Lb_SUCCESS)
    {
        LOGERR("Bullfrog Library initialization failed");
        return false;
    }

#ifdef __unix__
    /* clean up after SDL messing around where it shouldn't */
    unix_restore_signal_handlers();
#endif

    LbSetUserResourceMapping(SWResourceMapping);
    LbSetTitle(PACKAGE_NAME);
    LbSetIcon(1);

    // Make sure file names are properly converted before opening
    setup_file_names();

    return true;
}

void
game_handle_sdl_events (void)
{
    TbBool contn;
    contn = LbWindowsControl();
    if (!contn) {
        game_quit();
    }
}

void *ASM_smack_malloc(int msize);
void ASM_smack_mfree(void *ptr);
void *(*smack_malloc)(int);
void (*smack_free)(void *);


void load_texturemaps(void)
{
    asm volatile ("call ASM_load_texturemaps\n"
        :  :  : "eax" );
}

void test_open(int num)
{
    // Empty for production version
}

ushort my_count_lines(const char *text)
{
    ushort ret;
    asm volatile ("call ASM_my_count_lines\n"
        : "=r" (ret) : "a" (text));
    return ret;
}

void read_textwalk(void)
{
    TbFileHandle handle;
    handle = LbFileOpen("data/textwalk.dat", Lb_FILE_MODE_READ_ONLY);
    if ( handle != INVALID_FILE )
    {
        LbFileRead(handle, textwalk_data, 640);
        LbFileClose(handle);
    }
}

void read_primveh_obj(const char *fname, int a2)
{
    long firstval;
    TbFileHandle fh;

    fh = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    if ( fh == INVALID_FILE )
        return;
    LbFileRead(fh, &firstval, sizeof(long));
    if ( firstval != 1 )
    {
      LbFileRead(fh, &prim_object_points_count, sizeof(ushort));
      LbFileRead(fh, &prim_object_faces_count, sizeof(ushort));
      LbFileRead(fh, &prim_object_faces4_count, sizeof(ushort));
      LbFileRead(fh, &prim_objects_count, sizeof(ushort));
      LbFileRead(fh, &prim4_textures_count, sizeof(ushort));
      LbFileRead(fh, &prim_face_textures_count, sizeof(ushort));
      LbFileRead(fh, &prim_unknprop01, sizeof(ushort));
      LbFileRead(fh, prim_object_points, sizeof(PrimObjectPoint) * prim_object_points_count);
      LbFileRead(fh, prim_object_faces, sizeof(PrimObjectFace) * prim_object_faces_count);
      LbFileRead(fh, prim_object_faces4, sizeof(PrimObjectFace4) * prim_object_faces4_count);
      LbFileRead(fh, prim_objects, sizeof(PrimObject) * prim_objects_count);
      LbFileRead(fh, prim4_textures, sizeof(Prim4Texture) * prim4_textures_count);
      LbFileRead(fh, prim_face_textures, sizeof(PrimFaceTexture) * prim_face_textures_count);
    }
    LbFileClose(fh);
}

void load_prim_quad(void)
{
    unkn2_pos_x = 64;
    unkn2_pos_y = 64;
    unkn2_pos_z = 64;
    data_1c8428 = 0;
    prim_unknprop01 = 1000;
    read_primveh_obj(primvehobj_fname, 1);
    read_textwalk();
    data_19ec6f = 1;
    ingame.DisplayMode = DpM_UNKN_37;
    if (cmdln_param_bcg == 99)
        test_open(99);
    if (cmdln_param_bcg == 100)
        test_open(100);
}

void bang_init(void)
{
    asm volatile ("call ASM_bang_init\n"
        :  :  : "eax" );
}

void init_arrays_1(void)
{
    asm volatile ("call ASM_init_arrays_1\n"
        :  :  : "eax" );
}

void bang_set_detail(int a1)
{
    asm volatile ("call ASM_bang_set_detail\n"
        : : "a" (a1));
}

void game_setup_sub3(void)
{
    asm volatile ("call ASM_game_setup_sub3\n"
        :  :  : "eax" );
}

/** Remains of some Bf debug stuff.
 */
void debug_trace_setup(int place)
{
    LOGDBG("reached place %d", place);
}

/** Remains of some Bf debug stuff.
 */
void debug_trace_turn_bound(ulong turn)
{
    LOGDBG("turn %lu", turn);
}

/** NOOP func which compiler merged from several functions.
 */
void merged_noop_unkn1(int a1)
{
}

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

void player_debug(const char *text)
{
    // TODO place debug/verification code
}

void colour_tables_ghost_fixup(void)
{
    TbPixel *ipal;
    TbPixel *opal;
    ushort i;

    ipal = &pixmap.fade_table[63*PALETTE_8b_COLORS];
    opal = &pixmap.ghost_table[0*PALETTE_8b_COLORS];
    for (i = 0; i < PALETTE_8b_COLORS; i++)
        opal[i] = ipal[i];
}

void game_setup_stuff(void)
{
#if 0
    asm volatile ("call ASM_game_setup_stuff\n"
        :  :  : "eax" );
#else
    TbFileHandle fh;
    ushort i;

    for (i = 0; i < PALETTE_8b_COLORS; i++)
        linear_vec_pal[i] = i;
    vec_pal = linear_vec_pal;

    fh = LbFileOpen("data/nsta-0.ani", Lb_FILE_MODE_READ_ONLY);
    if (fh != INVALID_FILE) {
        nsta_size = LbFileSeek(fh, 0, Lb_FILE_SEEK_END);
        LbFileClose(fh);
    }

    colour_brown2 = LbPaletteFindColour(display_palette, 42, 37, 30);
    colour_grey2 = LbPaletteFindColour(display_palette, 32, 32, 32);
    colour_grey1 = LbPaletteFindColour(display_palette, 16, 16, 16);

    // TODO is there a reason to simplify the load here?
    //LbColourTablesLoad(display_palette, "data/tables.dat");
    LbFileLoadAt("data/tables.dat", &pixmap);

    colour_tables_ghost_fixup();
#endif
}

void smack_malloc_free_all(void)
{
    smack_malloc_used_tot = 0;
}

void flic_unkn03(ubyte a1)
{
    asm volatile ("call ASM_flic_unkn03\n"
        : : "a" (a1));
}

void person_func_unknown_310(ubyte a1)
{
    asm volatile ("call ASM_person_func_unknown_310\n"
        :  : "a" (a1));
}

void sprint_fmv_filename(ushort vid_type, char *fnbuf, ulong buflen)
{
    const char *fname;
    struct Campaign *p_campgn;

    fname = NULL;
    switch (vid_type)
    {
    case MPly_Intro:
        // Intro name is hard-coded, as it is played before any campaign is loaded
        fname = "intro.smk";
        if (game_dirs[DirPlace_Sound].use_cd == 1)
            sprintf(fnbuf, "%slanguage/%s/%s", cd_drive, language_3str, fname);
        else
            sprintf(fnbuf, "intro/%s", fname);
        break;
    case MPly_MissiComplete:
    case MPly_MPartComplete:
        switch (background_type)
        {
        case 0:
            // TODO MAPNO case for a specific map, remove
            if (current_map == 46)
            {
                fname = "syn_ele.smk";
                if (game_dirs[DirPlace_Data].use_cd == 1)
                    sprintf(fnbuf, "%slanguage/%s/%s", cd_drive, language_3str, fname);
                else
                    sprintf(fnbuf, "data/%s", fname);
            }
            else
            {
                fname = "syn_mc.smk";
                sprintf(fnbuf, "qdata/%s", fname);
            }
            break;
        case 1:
            // TODO MAPNO case for a specific map, remove
            if (current_map == 46)
            {
                fname = "chu_ele.smk";
                if (game_dirs[DirPlace_Data].use_cd == 1)
                    sprintf(fnbuf, "%slanguage/%s/%s", cd_drive, language_3str, fname);
                else
                    sprintf(fnbuf, "data/%s", fname);
            }
            else
            {
                fname = "zel-mc.smk";
                sprintf(fnbuf, "qdata/%s", fname);
            }
            break;
        default:
            fnbuf[0] = '\0';
            break;
        }
        break;
    case MPly_MissiFail:
        switch (background_type)
        {
        case 0:
            fname = "syn_fail.smk";
            sprintf(fnbuf, "qdata/%s", fname);
            break;
        case 1:
            fname = "syn_fail.smk";
            sprintf(fnbuf, "qdata/%s", fname);
            break;
        default:
            fnbuf[0] = '\0';
            break;
        }
        break;
    case MPly_GameOver:
        switch (background_type)
        {
        case 0:
            fname = "syn_go.smk";
            sprintf(fnbuf, "qdata/%s", fname);
            break;
        case 1:
            fname = "zel-go.smk";
            sprintf(fnbuf, "qdata/%s", fname);
            break;
        default:
            fnbuf[0] = '\0';
            break;
        }
        break;
    case MPly_Outro:
        p_campgn = &campaigns[background_type];
        //TODO should support use_cd like intro
        sprintf(fnbuf, "%s", p_campgn->OutroFMV);
        break;
    default:
        fnbuf[0] = '\0';
        break;
    }
}

void play_smacker(ushort vid_type)
{
#if 0
    asm volatile ("call ASM_play_smacker\n"
        : : "a" ((int)vid_type));
#else
    char fname[FILENAME_MAX];

    // TODO MAPNO case for a specific map, remove
    if (current_map == 51)
    {
        overall_scale = 256;
        unkn_flags_01 &= ~0x01;
        return;
    }
    person_func_unknown_310(2);
    if (lbDisplay.ScreenMode != screen_mode_fmvid_hi)
        setup_screen_mode(screen_mode_fmvid_hi);
    LbMouseChangeSprite(0);
    show_black_screen();

    sprint_fmv_filename(vid_type, fname, sizeof(fname));
    if (fname[0] != '\0') {
        play_smk(fname, 13, 0);
        smack_malloc_free_all();
    }
    show_black_screen();
#endif
}

void play_intro(void)
{
    char fname[FILENAME_MAX];

    LOGSYNC("Starting");
    lbDisplay.LeftButton = 0;
    lbKeyOn[KC_ESCAPE] = 0;
    if ( (cmdln_param_bcg || is_single_game) && ((ingame.Flags & GamF_SkipIntro) == 0))
    {
        setup_screen_mode(screen_mode_fmvid_lo);
        LbMouseChangeSprite(NULL);
        sprint_fmv_filename(MPly_Intro, fname, sizeof(fname));
        if (fname[0] != '\0') {
            play_smk(fname, 13, 0);
        }
        LbMouseChangeSprite(&pointer_sprites[1]);
        smack_malloc_used_tot = 0;
    }
    if (cmdln_param_bcg)
        setup_screen_mode(screen_mode_menu);
    flic_unkn03(1u);
}

void replay_intro(void)
{
    char fname[FILENAME_MAX];
    TbScreenModeInfo *mdinfo;

    LOGSYNC("Starting");
    mdinfo = LbScreenGetModeInfo(screen_mode_fmvid_lo);
    if (mdinfo->Width == 0) {
        LOGERR("Movies video mode %d is invalid", (int)screen_mode_fmvid_lo);
        return;
    }
    LbScreenSetup(screen_mode_fmvid_lo, mdinfo->Width, mdinfo->Height, display_palette);
    LbMouseSetup(0, 2, 2);
    show_black_screen();
    stop_sample_using_heap(0, 122);
    stop_sample_using_heap(0, 122);
    sprint_fmv_filename(MPly_Intro, fname, sizeof(fname));
    if (fname[0] != '\0') {
        play_smk(fname, 13, 0);
    }
    smack_malloc_free_all();
    play_sample_using_heap(0, 122, 127, 64, 100, -1, 3);
    show_black_screen();
    play_sample_using_heap(0, 122, 127, 64, 100, -1, 3);
}

int setup_heap_manager(struct SampleTable *smptable, size_t a2, const char *fname, unsigned int a4)
{
    int ret;
    asm volatile ("call ASM_setup_heap_manager\n"
        : "=r" (ret) : "a" (smptable), "d" (a2), "b" (fname), "c" (a4));
    return ret;
}

void reset_heaps(void)
{
}

void setup_heaps(int a1)
{
#if 0
    size_t ret;
    asm volatile ("call ASM_setup_heaps\n"
        : "=r" (ret) : "a" (a1));
    return ret;
#endif
    long sz;
    unsigned int n;

    if ((ingame.Flags & GamF_Unkn00020000) == 0)
        return;

    switch (a1)
    {
    case 100:
        sz = sound_heap_size;
        setup_heap_manager(sound_heap_memory, sz, "sound/syncreds.dat", 1622);
        break;
    case 99:
        break;
    case 0:
        sz = 1500000;
        while ((sound_heap_memory == NULL) && (sz > 100))
        {
            sound_heap_memory = LbMemoryAlloc(sz);
            if (sound_heap_memory == NULL)
                sz -= 30000;
        }
        if ((sound_heap_memory == NULL) || sz < 120000) {
            ingame.Flags &= GamF_Unkn00020000;
            break;
        }
        if (sz <= 500000)
            n = 822;
        else
            n = 1622;
        if (!setup_heap_manager(sound_heap_memory, sz, "sound/sound.dat", n))
            ingame.Flags &= GamF_Unkn00020000;
        sound_heap_size = sz;
        break;
    case 2:
        sz = 1500000;
        if (!setup_heap_manager(sound_heap_memory, sz, "sound/sound.dat", 1622))
            ingame.Flags &= GamF_Unkn00020000;
        sound_heap_size = sz;
        break;
    case 1:
    default:
        sz = 1500000;
        reset_heaps();
        LbMemoryFree(sound_heap_memory);
        sound_heap_size = sz;
        break;
    }
}

char func_cc638(const char *text, const char *fname)
{
    char ret;
    asm volatile ("call ASM_func_cc638\n"
        : "=r" (ret) : "a" (text), "d" (fname));
    return ret;
}

void screen_dark_curtain_down(void)
{
    asm volatile ("call ASM_screen_dark_curtain_down\n"
        :  :  : "eax" );
}

ubyte my_char_to_upper(ubyte c)
{
    ubyte ret;
    asm volatile ("call ASM_my_char_to_upper\n"
        : "=r" (ret) : "a" (c));
    return ret;
}

int load_outro_text(ubyte *buf)
{
    int totlen;
    char *s;
    int i;

    totlen = load_file_alltext("outtro.txt", buf);
    if (totlen == -1) {
        buf[0] = '\0';
        return 0;
    }
    buf[totlen] = '\0';
    s = (char *)buf;
    for (i = 0; i < totlen; i++) {
        s[i] = my_char_to_upper(s[i]);
    }

    s = (char *)buf;
    while ((*s != ']') && (*s != '\0')) {
        s++;
    }
    s++;
    outro_text_s = s;

    while ((*s != ']') && (*s != '\0')) {
        s++;
    }
    s++;
    outro_text_z = s;

    return totlen + 1;
}

int load_people_text(ubyte *buf)
{
    int totlen;
    int peptxt_len;
    int groupno;
    char *name;
    char *desc;
    char *s;
    int i;

    totlen = LbFileLoadAt("data/people.txt", buf);
    if (totlen == -1) {
        buf[0] = '\0';
        return 0;
    }
    peptxt_len = totlen;

    s = (char *)buf;
    for (i = 0; i < peptxt_len; i++)
    {
        if (*s == ']')
            *s = '\0';
        s++;
    }

    // List of people
    groupno = 0;
    s = (char *)buf;
    while ( 1 )
    {
        if (*s == '[')
        {
            s++;
            name = s;
            if (strcmp(name, "END") == 0) {
              s += 4;
              break;
            }
            people_credits_desc[2 * groupno + 0] = name;

            while (*s != '[')
            {
                if (*s == '#') {
                    groupno = strtol(s + 1, &desc, 10);
                    s = desc;
                }
                s++;
            }
            s++;
            desc = s;
            people_credits_desc[2 * groupno + 1] = desc;
            continue;
        }
        if (*s == '#') {
            groupno = strtol(s + 1, &desc, 10);
            s = desc;
        }
        s++;
    }

    // Assignment of people to groups
    while ( 1 )
    {
        if (*s == '[')
        {
            ubyte *g;

            s++;
            if (strcmp(s, "END") == 0) {
                s += 4;
                break;
            }
            people_credits_groups[2 * groupno + 0] = s;

            while (*s != '[')
            {
                if (*s == '#') {
                    groupno = strtol(s + 1, &desc, 10);
                    s = desc;
                }
                s++;
            }
            s++;
            g = &buf[totlen];
            people_credits_groups[2 * groupno + 1] = (char *)g; // TODO we should really use a struct here

            // Recognize the list of integers, store them in g
            while ( 1 )
            {
              *g = strtol(s, &desc, 10);
              if (desc > s) {
                  totlen++;
                  s = desc;
                  g++;
              }
              if (*s == '\0')
                  break;
              s++;
            }
            *g = 100; // end-of-list marker
            totlen++;
            continue;
        }
        if (*s == '#')
        {
            groupno = strtol(s + 1, &desc, 10);
            s = desc;
        }
        s++;
    }
    people_groups_count = groupno + 1;

    return totlen;
}

void load_outro_sprites(void)
{
#if 0
    asm volatile ("call ASM_load_outro_sprites\n"
        :  :  : "eax" );
#endif
    ubyte *data_buf;
    ubyte *outtxt_ptr;
    ubyte *peptxt_ptr;
    int next_pos;
    int tit_font_pos;
    int med_font_pos;
    int big_font_pos;
    int big_font_end_pos;
    int med2_font_pos;
    int med2_font_end_pos;

    next_pos = engine_mem_alloc_size - 81920;
    data_buf = engine_mem_alloc_ptr + next_pos;

    med_font_data = &data_buf[0];
    next_pos = LbFileLoadAt("data/tit-font.dat", med_font_data);
    next_pos += 0;
    tit_font_pos = next_pos;
    med_font = (struct TbSprite *)&data_buf[tit_font_pos];
    next_pos = LbFileLoadAt("data/tit-font.tab", med_font);
    next_pos += tit_font_pos;
    med_font_pos = next_pos;
    med_font_end = (struct TbSprite *)&data_buf[med_font_pos];

    big_font_data = &data_buf[med_font_pos];
    next_pos = LbFileLoadAt("data/nam-font.dat", big_font_data);
    next_pos += med_font_pos;
    big_font_pos = next_pos;
    big_font = (struct TbSprite *)&data_buf[big_font_pos];
    next_pos = LbFileLoadAt("data/nam-font.tab", big_font);
    next_pos += big_font_pos;
    big_font_end_pos = next_pos;
    big_font_end = (struct TbSprite *)&data_buf[big_font_end_pos];

    med2_font_data = &data_buf[big_font_end_pos];
    next_pos = LbFileLoadAt("data/qot-font.dat", med2_font_data);
    next_pos += big_font_end_pos;
    med2_font_pos = next_pos;
    med2_font = (struct TbSprite *)&data_buf[med2_font_pos];
    next_pos = LbFileLoadAt("data/qot-font.tab", med2_font);
    next_pos += med2_font_pos;
    med2_font_end_pos = next_pos;
    med2_font_end = (struct TbSprite *)&data_buf[med2_font_end_pos];

    LbSpriteSetup(med_font, med_font_end, med_font_data);
    LbSpriteSetup(med2_font, med2_font_end, med2_font_data);
    LbSpriteSetup(big_font, big_font_end, big_font_data);

    outtxt_ptr = &data_buf[med2_font_end_pos];
    next_pos = load_outro_text(outtxt_ptr);
    next_pos = med2_font_end_pos + next_pos;

    outro_unkn01 = 1;
    outro_unkn02 = 0;
    outro_unkn03 = 0;

    peptxt_ptr = &data_buf[next_pos];
    next_pos = load_people_text(peptxt_ptr);
}

void fill_floor_textures(void)
{
    asm volatile ("call ASM_fill_floor_textures\n"
        :  :  : "eax" );
}

void global_3d_store(int action)
{
    if (action == 2)
    {
        if (stored_global3d_inuse[0])
            draw_text(100, 120, " GLOBAL 3d STORED ->INUSE", colour_lookup[2]);
    }
    else if (action == 1)
    {
        if (stored_global3d_inuse[0])
        {
            next_object = stored_g3d_next_object[0];
            next_object_face = stored_g3d_next_object_face[0];
            next_object_face4 = stored_g3d_next_object_face4[0];
            next_object_point = stored_g3d_next_object_point[0];
            next_normal = stored_g3d_next_normal[0];
            next_face_texture = stored_g3d_next_face_texture[0];
            next_floor_texture = stored_g3d_next_floor_texture[0];
            next_local_mat = stored_g3d_next_local_mat[0];
            stored_global3d_inuse[0] = 0;
        }
    } else
    {
        if (!stored_global3d_inuse[0])
        {
            stored_g3d_next_object[0] = next_object;
            stored_g3d_next_object_face[0] = next_object_face;
            stored_g3d_next_object_face4[0] = next_object_face4;
            stored_g3d_next_object_point[0] = next_object_point;
            stored_g3d_next_normal[0] = next_normal;
            stored_g3d_next_face_texture[0] = next_face_texture;
            stored_g3d_next_floor_texture[0] = next_floor_texture;
            stored_g3d_next_local_mat[0] = next_local_mat;
            stored_global3d_inuse[0] = 1;
        }
    }
}

void unkn_f_pressed_func(void)
{
    struct Command *gcmds;
    short thing;
    short i;

    gcmds = game_commands;
    thing = same_type_head[1];
    for (i = 0; thing != 0; i++)
    {
        struct Thing *p_thing;
        ushort cmd;
        ushort prev_cmd;

        if (i >= max(STHINGS_LIMIT,THINGS_LIMIT)) {
            LOGERR("Infinite loop in same type things list");
            break;
        }
        p_thing = &things[thing];
        cmd = p_thing->U.UPerson.ComHead;
        prev_cmd = 0;
        while (cmd != 0)
        {
            if (!is_command_any_until(&gcmds[cmd]))
            {
                if (prev_cmd)
                    gcmds[prev_cmd].Flags |= 0x20000;
                gcmds[cmd].Flags |= 0x40000;
            }
            prev_cmd = cmd;
            cmd = gcmds[cmd].Next;
        }
        thing = p_thing->LinkSame;
    }
    game_commands = gcmds;
}

void fix_level_indexes(short missi, ulong fmtver, ubyte reload)
{
#if 0
    asm volatile ("call ASM_fix_level_indexes\n"
        :  :  : "eax" );
#endif
    ushort objectv;
    short thing;

    fix_thing_commands_indexes();

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

void build_same_type_headers(void)
{
    asm volatile ("call ASM_build_same_type_headers\n"
        :  :  : "eax" );
}

void level_misc_update(void)
{
    asm volatile ("call ASM_level_misc_update\n"
        :  :  : "eax" );
}

void load_mad_pc(ushort mapno)
{
    asm volatile ("call ASM_load_mad_pc\n"
        : : "a" (mapno));
}

void unkn_object_shift_03(ushort objectno)
{
    asm volatile ("call ASM_unkn_object_shift_03\n"
        : : "a" (objectno));
}

void unkn_object_shift_02(int norm1, int norm2, ushort objectno)
{
    asm volatile ("call ASM_unkn_object_shift_02\n"
        : : "a" (norm1), "d" (norm2), "b" (objectno));
}

void func_6031c(short tx, short tz, short a3, short ty)
{
    asm volatile ("call ASM_func_6031c\n"
        : : "a" (tx), "d" (tz), "b" (a3), "c" (ty));
}

/** Transfer some people of given subtype from one group to the other.
 */
int thing_group_transfer_people(short pv_group, short nx_group, short subtype, int limit)
{
    short thing;
    struct Thing *p_thing;
    int count;

    count = 0;
    for (thing = things_used_head; thing != 0; thing = p_thing->LinkChild)
    {
        p_thing = &things[thing];

        if (p_thing->Type != TT_PERSON)
            continue;

        if ((subtype != -1) && (p_thing->SubType != subtype))
            continue;

        if (p_thing->U.UObject.Group == pv_group) {
            p_thing->U.UObject.Group = nx_group;
            count++;
        }
        if (p_thing->U.UObject.EffectiveGroup == pv_group) {
            p_thing->U.UObject.EffectiveGroup = nx_group;
        }

        if (count >= limit)
            break;
    }
    return count;
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
                n = thing_group_transfer_people(nx_group, lp_group, SubTT_PERS_AGENT, 4);
                if (n <= 0)
                    n = thing_group_transfer_people(nx_group, lp_group, SubTT_PERS_ZEALOT, 4);
                if (n <= 0)
                    n = thing_group_transfer_people(nx_group, lp_group, -1, 4);
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

    if (level_def.PlayableGroups[0] == 0) {
        short pv_group, nx_group;

        pv_group = level_def.PlayableGroups[0];
        nx_group = find_unused_group_id(true);
        LOGWARN("Local player group is %d; switching to %d",
          (int)pv_group, (int)nx_group);
        if (nx_group > 0) {
            thing_group_copy(pv_group, nx_group, 0x01|0x02);
            thing_group_transfer_people(pv_group, nx_group, -1, 4);
        }
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
            if (fmtver <= 12) {
                struct ThingOldV9 s_oldthing;
                assert(sizeof(s_oldthing) == 216);
                LbFileRead(lev_fh, &s_oldthing, sizeof(s_oldthing));
                refresh_old_thing_format(p_thing, &s_oldthing, fmtver);
            } else {
                LbFileRead(lev_fh, p_thing, sizeof(struct Thing));
            }
            LOGNO("Thing(%hd,%hd) group %hd at (%d,%d,%d) type=%d,%d",
              p_thing->ThingOffset, p_thing->U.UPerson.UniqueID,
              p_thing->U.UPerson.Group,
              p_thing->X >> 8, p_thing->Y >> 8, p_thing->Z >> 8,
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
                func_6031c(p_thing->X >> 8, p_thing->Z >> 8, -prim_unknprop01 - p_thing->StartFrame, p_thing->Y >> 8);
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

    LbFileRead(lev_fh, engine_mem_alloc_ptr + engine_mem_alloc_size - 1353, 1320);
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
              p_thing->X >> 8, p_thing->Y >> 8, p_thing->Z >> 8,
              (int)p_thing->Type, (int)p_thing->SubType);
        }
    }

    if (fmtver > 5)
    {
        LbFileRead(lev_fh, &word_1810E4, 2);
        if (word_1810E4 < 1000)
            word_1810E4 = 1000;
        if (word_1810E4 > 9000)
            word_1810E4 = 4000;
    }

    if (fmtver <= 6) {
        next_used_lvl_objective = 1;
    } else {
        LbFileRead(lev_fh, &next_used_lvl_objective, sizeof(ushort));
        LbFileRead(lev_fh, game_used_lvl_objectives, 32 * next_used_lvl_objective);
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

void load_level_pc(short level, short missi, ubyte reload)
{
    short next_level, prev_level;
    TbFileHandle lev_fh;
    char lev_fname[52];

    next_level = level;
    gameturn = 0;
    LbMouseChangeSprite(0);
    if (0) { // No need to conserve memory to such extent - mem_game[] was changed
        // Optimization for memory conservation - reserve no space for game_commands,
        // and instead re-use some of triangulation area during map load.
        void **p = mem_game[34].BufferPtr;
        *p = dword_177750;
        mem_game[34].N = 3100;
    }
    read_primveh_obj(primvehobj_fname, 1);

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

        if (fmtver <= 10)
        {
            for (i = 1; i < next_command; i++)
                game_commands[i].Time = 1;
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
        } else {
            if (level_def.PlayableGroups[0] == 0) {
                LOGWARN("Local player group equal 0 will cause issues; fix the level");
            }
        }
        fix_level_indexes(missi, fmtver, reload);
    } else
    {
        LOGERR("Could not open mission file, load skipped");
    }
    // No idea what exactly pressing F during level load does
    if (lbKeyOn[KC_F])
        unkn_f_pressed_func();
}

TbBool is_unkn_current_player(void)
{
    TbBool ret;
    asm volatile ("call ASM_is_unkn_current_player\n"
        : "=r" (ret) : );
    return ret;
}

void change_current_map(ushort mapno)
{
    current_map = mapno;
    init_things();
    load_mad_pc(mapno);
    fill_floor_textures();
}

void change_brightness(ushort val)
{
    asm volatile ("call ASM_change_brightness\n"
        : : "a" (val));
}

int alt_at_point(ushort x, ushort z)
{
    int ret;
    asm volatile ("call ASM_alt_at_point\n"
        : "=r" (ret) : "a" (x), "d" (z));
    return ret;
}

void traffic_unkn_func_01(void)
{
    asm volatile ("call ASM_traffic_unkn_func_01\n"
        :  :  : "eax" );
}

void process_tank_turret(struct Thing *p_tank)
{
#if 0
    asm volatile ("call ASM_process_tank_turret\n"
        : : "a" (p_tank));
#else
    struct Thing *p_turret;
    int target_x, target_y;
    int turret;
    short angle;
    int dt_angle;

    turret = p_tank->U.UVehicle.SubThing;
    if (turret == 0) {
        p_tank->OldTarget = LbFPMath_PI+1;
        return;
    }
    p_turret = &things[turret];
    if ((p_tank->Flag & TngF_Unkn20000000) != 0)
    {
        target_x = p_tank->U.UVehicle.TargetDX;
        target_y = p_tank->U.UVehicle.TargetDZ;
    }
    else
    {
        struct Thing *p_target;
        p_target = p_tank->PTarget;
        if (p_target == NULL)
        {
            p_tank->OldTarget = 20000;
            return;
        }
        target_x = p_target->X >> 8;
        target_y = p_target->Z >> 8;
    }
    angle = p_turret->U.UMGun.AngleY
        - angle_between_points(target_x, target_y, p_tank->X >> 8, p_tank->Z >> 8);
    if (angle < -LbFPMath_PI)
        angle += 2*LbFPMath_PI;
    else if (angle > LbFPMath_PI)
        angle -= 2*LbFPMath_PI;

    //TODO CONFIG how fast the tank can target could be a difficulty-related setting
    // Travel 1/8 of the distance in each game turn
    dt_angle = angle / 8;
    if (dt_angle > LbFPMath_PI/17)
        dt_angle = LbFPMath_PI/17;
    if (dt_angle < -LbFPMath_PI/17)
        dt_angle = -LbFPMath_PI/17;
    if (dt_angle == 0)
    {
        if (angle > 0)
            dt_angle = 1;
        if (angle < 0)
            dt_angle = -1;
    }

    // Despite being caused by the turret, we bind the sound samples to the vehicle part
    // of the tank. This is because turrets do not contain full position on map, so the
    // sound update would misplace the sound source if it was bound to the turret
    if ((p_turret->Flag2 & 0x0200) != 0)
    {
        if (dt_angle <= 1) {
            // Play rotation stop sample
            if (!IsSamplePlaying(p_tank->ThingOffset, 47, 0))
                play_dist_sample(p_tank, 47, 127, 0x40u, 100, 0, 1);
            p_turret->Flag2 &= ~0x0200;
        }
    }
    else
    {
        // Play rotation sample if moving over 1.2 degree per turn and the angle is not getting smaller.
        // Huge values of OldTarget (beyond pi) indicate that previously we had no target.
        if ((abs(dt_angle) >= LbFPMath_PI/75) && (p_tank->OldTarget < abs(angle) || p_tank->OldTarget > LbFPMath_PI)) {
            if (!IsSamplePlaying(p_tank->ThingOffset, 48, 0))
                play_dist_sample(p_tank, 48, 127, 0x40u, 100, 0, 1);
            p_turret->Flag2 |= 0x0200;
        }
    }

    p_turret->U.UMGun.AngleY -= dt_angle;
    p_tank->OldTarget = abs(angle);
    p_turret->U.UMGun.AngleY = (p_turret->U.UMGun.AngleY + 2*LbFPMath_PI) & LbFPMath_AngleMask;
#endif
}

void process_view_inputs(int thing)
{
    asm volatile ("call ASM_process_view_inputs\n"
        : : "a" (thing));
}

void process_engine_unk1(void)
{
    asm volatile ("call ASM_process_engine_unk1\n"
        :  :  : "eax" );
}

void process_engine_unk2(void)
{
    asm volatile ("call ASM_process_engine_unk2\n"
        :  :  : "eax" );
}

void draw_hud(int thing)
{
    asm volatile ("call ASM_draw_hud\n"
        : : "a" (thing));
}

void SCANNER_unkn_func_200(struct TbSprite *spr, int x, int y, ubyte col)
{
#if 0
    asm volatile (
      "call ASM_SCANNER_unkn_func_200\n"
        : : "a" (spr), "d" (x), "b" (y), "c" (col));
#else
    int xwind_beg;
    int xwind_end;
    int xwind_start;
    sbyte *inp;
    ubyte *oline;
    int opitch;
    int h;
    TbBool needs_window_bounding;

    xwind_beg = lbDisplay.GraphicsWindowX;
    xwind_end = lbDisplay.GraphicsWindowX + lbDisplay.GraphicsWindowWidth;
    xwind_start = lbDisplay.GraphicsWindowX + x;
    inp = (sbyte *)spr->Data;
    opitch = lbDisplay.GraphicsScreenWidth;
    oline = &lbDisplay.WScreen[opitch * (lbDisplay.GraphicsWindowY + y) + lbDisplay.GraphicsWindowX + x];
    if (xwind_start < lbDisplay.GraphicsWindowX) {
        if (xwind_start + 2 * spr->SWidth <= lbDisplay.GraphicsWindowX)
            return;
        needs_window_bounding = true;
    } else {
        if (xwind_start >= xwind_end)
            return;
        needs_window_bounding = (xwind_start + 2 * spr->SWidth > xwind_end);
    }

    if (!needs_window_bounding)
    {
        // Simplified and faster drawing when we do not have to check bounds
        for (h = 0; h < spr->SHeight; h++)
        {
            ubyte *o;

            o = oline;
            while (*inp)
            {
                int ival;
                int i;

                ival = *inp;
                if (ival < 0)
                {
                    inp++;
                    o -= 2 * ival;
                    continue;
                }
                inp += ival + 1;
                for (i = 0; i < ival; i++)
                {
                    o[0] = col;
                    o[opitch + 0] = col;
                    o[1] = col;
                    o[opitch + 1] = col;
                    o += 2;
                }
            }
            inp++;
            oline += 2 * opitch;
        }
    }
    else
    {
        for (h = 0; h < spr->SHeight; h++)
        {
            ubyte *o;
            int xwind_curr;

            o = oline;
            xwind_curr = xwind_start;
            while (*inp)
            {
                int ival;
                int i;

                ival = *inp;
                if (ival < 0)
                {
                    inp++;
                    o -= 2 * ival;
                    xwind_curr -= 2 * ival;
                    continue;
                }
                inp += ival + 1;
                for (i = 0; i < ival; i++)
                {
                    if (xwind_curr >= xwind_beg && xwind_curr < xwind_end) {
                        o[0] = col;
                        o[opitch] = col;
                    }
                    xwind_curr++;
                    o++;
                    if (xwind_curr >= xwind_beg && xwind_curr < xwind_end) {
                        o[0] = col;
                        o[opitch] = col;
                    }
                    xwind_curr++;
                    o++;
                }
            }
            inp++;
            oline += 2 * opitch;
        }
    }
#endif
}


void SCANNER_unkn_func_201(struct TbSprite *spr, int x, int y, ubyte *fade)
{
#if 0
    asm volatile (
      "call ASM_SCANNER_unkn_func_201\n"
        : : "a" (spr), "d" (x), "b" (y), "c" (fade));
#endif
    ubyte *oline;
    ubyte *dt;
    int ich;
    ubyte *o;

    oline = &lbDisplay.WScreen[lbDisplay.GraphicsScreenWidth * y + x];
    dt = spr->Data;
    for (ich = spr->SHeight; ich > 0; ich--)
    {
        o = oline;
        while (1)
        {
            ushort ftidx;
            sbyte len;

            len = *dt;
            if (!len)
                break;
            if (len > 0)
            {
                ++dt;
                while (len)
                {
                    ftidx = *dt;
                    *o = fade[ftidx];
                    ++dt;
                    ++o;
                    len--;
                }
            }
            else
            {
                len = -len;
                o += len;
                ++dt;
            }
        }
        ++dt;
        oline += lbDisplay.GraphicsScreenWidth;
    }
}

void SCANNER_unkn_func_202(struct TbSprite *spr, int x, int y, int ctr, int bri)
{
#if 0
    asm volatile (
      "push %4\n"
      "call ASM_SCANNER_unkn_func_202\n"
        : : "a" (spr), "d" (x), "b" (y), "c" (ctr), "g" (bri));
#endif
    ubyte *oline;
    ubyte *dt;
    int ich;
    ubyte *o;

    if ((x < 0) || (x > lbDisplay.PhysicalScreenWidth))
        return;
    if ((y < 0) || (y > lbDisplay.PhysicalScreenHeight))
        return;
    if ((x + spr->SWidth < 0) || (x + spr->SWidth > lbDisplay.PhysicalScreenWidth))
        return;
    if ((y + spr->SHeight < 0) || (y + spr->SHeight > lbDisplay.PhysicalScreenHeight))
        return;

    oline = &lbDisplay.WScreen[y * lbDisplay.GraphicsScreenWidth + x];
    dword_1DC36C = bri;
    dt = spr->Data;
    for (ich = spr->SHeight; ich > 0; ich--)
    {
        o = oline;
        while (1)
        {
            ushort ftsub, ftidx;
            sbyte len;

            len = *dt;
            if (!len)
                break;
            if (len > 0)
            {
                ++dt;
                while (len)
                {
                    ftidx = *dt++;
                    ftsub = dword_1DC36C + ((byte_1CAB64[ftidx] >> 1) + (byte_1CAB64[*o] >> 1));
                    ftidx |= byte_1DB088[ftsub] << 8;
                    *o++ = pixmap.fade_table[ftidx];
                    len--;
                }
            }
            else
            {
                len = -len;
                o += len;
                ++dt;
            }
        }
        ++dt;
        oline += lbDisplay.GraphicsScreenWidth;
    }
}

void SCANNER_unkn_func_203(int a1, int a2, int a3, int a4, ubyte a5, int a6, int a7)
{
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_SCANNER_unkn_func_203\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5), "g" (a6), "g" (a7));
}

void LbSpriteDraw_1(int x, int y, struct TbSprite *spr)
{
    asm volatile (
      "call ASM_LbSpriteDraw_1\n"
        : : "a" (x), "d" (y), "b" (spr));
}

void LbSpriteDraw_2(int x, int y, struct TbSprite *spr)
{
    asm volatile (
      "call ASM_LbSpriteDraw_2\n"
        : : "a" (x), "d" (y), "b" (spr));
}

void SCANNER_draw_new_transparent(void)
{
    asm volatile ("call ASM_SCANNER_draw_new_transparent\n"
        :  :  : "eax" );
}

void SCANNER_unkn_func_204(int a1, int a2, int a3)
{
    asm volatile (
      "call ASM_SCANNER_unkn_func_204\n"
        : : "a" (a1), "d" (a2), "b" (a3));
}

void SCANNER_unkn_func_205(void)
{
    asm volatile ("call ASM_SCANNER_unkn_func_205\n"
        :  :  : "eax" );
}

/**
 * Draw the button with standard palette, for selectable items.
 * @param px
 * @param py
 * @param spr_id
 */
void draw_new_panel_sprite_std(int px, int py, ulong spr_id)
{
    struct TbSprite *spr;
    int x, y;

    spr = &pop1_sprites[spr_id];
    if (lbDisplay.ScreenMode != 1) {
        x = px;
        y = py;
    } else {
        x = px >> 1;
        y = py >> 1;
    }
    if (ingame.PanelPermutation == -1)
        SCANNER_unkn_func_202(spr, x, y, ingame.Scanner.Contrast, ingame.Scanner.Brightness);
    else
        LbSpriteDraw_1(x, y, spr);
}

/**
 * Draw the button with darkened palette, like the item is unavailable.
 * @param px
 * @param py
 * @param spr_id
 */
void draw_new_panel_sprite_dark(int px, int py, ulong spr_id)
{
    struct TbSprite *spr;
    int x, y;

    spr = &pop1_sprites[spr_id];
    if (lbDisplay.ScreenMode != 1) {
        x = px;
        y = py;
    } else {
        x = px >> 1;
        y = py >> 1;
    }
    if (ingame.PanelPermutation == -1)
        SCANNER_unkn_func_202(spr, x, y, ingame.Scanner.Contrast, 8);
    else
        SCANNER_unkn_func_201(spr, x, y, &pixmap.fade_table[4096]);
}

/**
 * For weapons which contain up to four itemized parts, draw the items.
 *
 * @param a1
 * @param a2
 * @param a3
 * @param a4
 */
void draw_fourpack_items(int a1, ushort a2, short a3, short a4)
{
    asm volatile (
      "call ASM_draw_fourpack_items\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4));
}

sbyte find_nth_weapon_held(ushort index, ubyte n)
{
    char ret;
    asm volatile ("call ASM_find_nth_weapon_held\n"
        : "=r" (ret) : "a" (index), "d" (n));
    return ret;
}

TbBool draw_panel_pickable_thing_below_agent(struct Thing *p_agent)
{
    struct SimpleThing *p_pickup;
    short thing;
    TbBool drawn;

    drawn = false;
    if (p_agent->Flag & TngF_Unkn08000000)
    {
        ushort weptype;
        thing = p_agent->U.UPerson.Vehicle; // Seem to be weapon standing over rather than vehicle
        if (thing != 0)
            p_pickup = &sthings[thing];
        else
            p_pickup = NULL;
        if ((p_pickup != NULL) && (p_pickup->Type == SmTT_DROPPED_ITEM)) {
            lbDisplay.DrawFlags = 0;
            weptype = p_pickup->U.UWeapon.WeaponType;
            if (weptype)
                draw_new_panel_sprite_std(548, 364, weapon_defs[weptype].Sprite & 0xFF);
            else
                draw_new_panel_sprite_std(540, 360, 70);
            draw_new_panel_sprite_std(540, 360, 12);
            drawn = true;
        } else {
            // FIXME a strange place for fixing state of an agent; should be moved to game world update
            p_agent->Flag &= ~TngF_Unkn08000000;
        }
    }
    return drawn;
}

TbBool draw_panel_pickable_thing_player_targeted(PlayerInfo *p_locplayer)
{
    ;
    struct SimpleThing *p_pickup;
    short thing;
    TbBool drawn;

    drawn = false;
    if (p_locplayer->TargetType == 5)
    {
        ushort weptype;
        thing = p_locplayer->field_102;
        if (thing < 0)
        {
            lbDisplay.DrawFlags = 0;
            p_pickup = &sthings[thing];
            weptype = p_pickup->U.UWeapon.WeaponType;
            if (weptype)
                draw_new_panel_sprite_std(548, 364, weapon_defs[weptype].Sprite & 0xFF);
            else
                draw_new_panel_sprite_std(548, 364, 70);
            draw_new_panel_sprite_std(540, 360, 12);
            drawn = true;
        }
    }
    return drawn;
}

/**
 * Counts weapons in given flags, returning total, amount below given weapon and above it.
 * To be used for displaying scrollable list of weapons where current weapon is always visible.
 *
 * @param p_ncarr_below
 * @param p_ncarr_above
 * @param weapons_carried
 * @param current_weapon
 * @return
 */
int count_weapons_in_flags(int *p_ncarr_below, int *p_ncarr_above, ulong weapons_carried, short current_weapon)
{
    int ncarried, ncarr_below, ncarr_above;
    ulong wepflags;
    ushort nweptype;

    ncarried = 0;
    ncarr_above = 0;
    ncarr_below = 0;
    wepflags = weapons_carried;

    for (nweptype = 0; nweptype < WEP_TYPES_COUNT; nweptype++)
    {
        if (!wepflags)
            break;
        if (wepflags & 1)
        {
            ncarried++;
            if (nweptype + 1 > current_weapon)
                ncarr_above++;
            if (nweptype + 1 < current_weapon)
                ncarr_below++;
        }
        wepflags >>= 1;
    }

    *p_ncarr_below = ncarr_below;
    *p_ncarr_above = ncarr_above;
    return ncarried;
}

TbBool draw_weapons_list_single(PlayerInfo *p_locplayer, ushort comcur, ulong weapons_carried, short current_weapon)
{
    ushort nshown;
    int weptype;
    int nchecked;
    ulong wepflags;
    int cy;
    int ncarried;
    int ncarr_below;
    int ncarr_above;
    TbBool ret;

    ret = false;
    ncarried = count_weapons_in_flags(&ncarr_below, &ncarr_above, weapons_carried, current_weapon);
    if (ncarried <= 6)
        ncarr_below = 0;
    else if (ncarr_below <= 4)
        ncarr_below = 0;
    else
        ncarr_below -= 4;

    nshown = 0;
    wepflags = weapons_carried;
    cy = 36;
    nchecked = 0;
    for (weptype = 0; weptype < WEP_TYPES_COUNT; weptype++, wepflags >>= 1)
    {
        if (wepflags == 0)
            break;
        if (wepflags & 1)
        {
            if (ncarr_below > nshown)
            {
                nshown++;
                if (nchecked == 6)
                    break;
                continue;
            }
            lbDisplay.DrawFlags = 0;

            if (!p_locplayer->WepDelays[comcur][weptype + 1] || (gameturn & 1))
            {
                if (nshown == 6)
                    draw_new_panel_sprite_std(22, cy, 13);
                else
                    draw_new_panel_sprite_std(22, cy, 12);
            }

            if (!p_locplayer->WepDelays[comcur][weptype + 1] || (gameturn & 1))
                draw_new_panel_sprite_std(30, cy + 4, weapon_defs[weptype+1].Sprite & 0xFF);
            if (weptype+1 == current_weapon)
            {
                lbDisplay.DrawFlags = 0;
                draw_new_panel_sprite_std(30, cy + 4, (weapon_defs[weptype+1].Sprite & 0xFF) + 27);
            }
            if (!lbDisplay.MRightButton)
            {
                short msy, msx;
                msy = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.RMouseY : lbDisplay.RMouseY;
                msx = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.RMouseX : lbDisplay.RMouseX;
                if (in_box(msx, msy, 22, cy, 76, 22))
                {
                    lbDisplay.DrawFlags = 0;
                    ret = true;
                    p_locplayer->PanelItem[mouser] = weptype + 1;
                    draw_new_panel_sprite_std(22, cy, 90);
                }
            }
            draw_fourpack_items(22, cy, comcur, weptype + 1);
            cy += 28;
            ++nchecked;
            ++nshown;
            if (nchecked == 6)
                break;
        }
    }
    return ret;
}

short draw_current_weapon_button(PlayerInfo *p_locplayer, short nagent)
{
    struct Thing *p_agent;
    ushort curwep, prevwep;
    short cx, cy;

    if (lbDisplay.ScreenMode == 1) {
        cy = 28;
        cx = 158 * nagent + 66;
    } else {
        cy = 29;
        cx = 157 * nagent + 65;
    }
    p_agent = p_locplayer->MyAgent[nagent];
    // Protect from damaged / unfinished levels
    if (p_agent == NULL)
        return 0;

    curwep = p_agent->U.UPerson.CurrentWeapon;
    prevwep = p_locplayer->PrevWeapon[nagent];
    if (!curwep && !prevwep) {
        prevwep = find_nth_weapon_held(p_agent->ThingOffset, 1);
        p_locplayer->PrevWeapon[nagent] = prevwep;
    }

    // Highlight button border on mouse over
    if (curwep || prevwep)
    {
        TbBool wep_highlight;
        short msx, msy;
        wep_highlight = false;
        if (!lbDisplay.MRightButton) {
            msx = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.RMouseX : lbDisplay.RMouseX;
            msy = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.RMouseY : lbDisplay.RMouseY;
            if (in_box(msx, msy, cx - 4, cy - 4, 76, 24))
                wep_highlight = true;
        }
        {
            msx = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
            msy = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
            if (in_box(msx, msy, cx - 4, cy - 4, 76, 24))
                wep_highlight = true;
        }
        if (wep_highlight)
        {
            if (curwep)
                p_locplayer->PanelItem[mouser] = curwep;
            else
                p_locplayer->PanelItem[mouser] = prevwep;
            draw_new_panel_sprite_std(cx - 8, cy - 4, 90);
        }
    }
    if (curwep) // Active weapon - draw lighted weapon shape
    {
        if (!p_locplayer->WepDelays[nagent][curwep] || (gameturn & 1))
        {
            struct Thing *p_agent;
            p_agent = p_locplayer->MyAgent[nagent];
            if (p_agent->State == PerSt_PROTECT_PERSON)
            {
                struct TbSprite *spr;
                if (ingame.PanelPermutation == -1)
                {
                    spr = &pop1_sprites[14];
                    if (lbDisplay.ScreenMode == 1)
                        SCANNER_unkn_func_202(spr, cx >> 1, cy >> 1,
                            ingame.Scanner.Contrast, ingame.Scanner.Brightness);
                    else
                        SCANNER_unkn_func_202(spr, cx, cy,
                            ingame.Scanner.Contrast, ingame.Scanner.Brightness);
                }
                draw_new_panel_sprite_dark(cx, cy, (weapon_defs[curwep].Sprite & 0xFF) + 27);
            }
            else
            {
                struct TbSprite *spr;
                if (ingame.PanelPermutation == -1)
                {
                    spr = &pop1_sprites[14];
                    if (lbDisplay.ScreenMode == 1)
                        SCANNER_unkn_func_202(spr, cx >> 1, cy >> 1,
                            ingame.Scanner.Contrast, ingame.Scanner.Brightness);
                    else
                        SCANNER_unkn_func_202(spr, cx, cy,
                            ingame.Scanner.Contrast, ingame.Scanner.Brightness);
                }
                draw_new_panel_sprite_std(cx, cy, (weapon_defs[curwep].Sprite & 0xFF) + 27);
            }
            draw_fourpack_items(cx - 8, cy - 4, nagent, curwep);
        }
    }
    else
    { // Weapon is hidden - draw with dark weapon shape
        curwep = prevwep;
        if (curwep && (!p_locplayer->WepDelays[nagent][curwep] || (gameturn & 1)))
        {
            struct Thing *p_agent;
            p_agent = p_locplayer->MyAgent[nagent];
            if (p_agent->State == PerSt_PROTECT_PERSON)
                draw_new_panel_sprite_dark(cx, cy, weapon_defs[curwep].Sprite & 0xFF);
            else
                draw_new_panel_sprite_std(cx, cy, weapon_defs[curwep].Sprite & 0xFF);
            draw_fourpack_items(cx - 8, cy - 4, nagent, curwep);
        }
    }
    return curwep;
}

TbBool draw_agent_weapons_selection(PlayerInfo *p_locplayer, struct Thing *p_agent, ubyte *cur_weapons, int nagent)
{
    int cx, cy;
    int weptype;
    int dcx, dcy;
    int nunk1;
    ulong wepflags;
    ushort comcur;
    ushort nshown;
    int nchecked;
    TbBool wep_highlight;
    TbBool ret;

    ret = false;
    nunk1 = 0;
    comcur = p_agent->U.UPerson.ComCur & 3;
    wepflags = p_agent->U.UPerson.WeaponsCarried;
    cy = 44;
    cx = 158 * nagent + 42;
    // Some weapons are not selectable
    wepflags &= ~(1 << (WEP_ENERGYSHLD-1));
    nshown = 0;
    nchecked = 0;
    for (weptype = 0; weptype < WEP_TYPES_COUNT; weptype++, wepflags >>= 1)
    {
        if (wepflags == 0)
            break;
        if ((wepflags & 1) == 0)
            continue;

        if (nunk1 > nshown || cur_weapons[comcur] == weptype + 1)
        {
            ++nshown;
            if (nchecked == 12)
                break;
            continue;
        }
        lbDisplay.DrawFlags = 0;
        if (!p_locplayer->WepDelays[comcur][weptype + 1] || (gameturn & 1))
        {
            if (!nchecked)
                draw_new_panel_sprite_std(cx, cy, 13);
            else
                draw_new_panel_sprite_std(cx, cy, 94);
        }

        if (!p_locplayer->WepDelays[comcur][weptype + 1] || (gameturn & 1))
        {
            if (!nchecked)
                draw_new_panel_sprite_std(cx + 24, cy + 12, weapon_defs[weptype + 1].Sprite & 0xFF);
            else
                draw_new_panel_sprite_std(cx + 8, cy + 8, weapon_defs[weptype + 1].Sprite & 0xFF);
        }
        dcx = 0;
        dcy = 0;
        if (!nchecked)
        {
            dcx = 14;
            dcy = 8;
        }

        wep_highlight = false;
        if (!lbDisplay.MRightButton) {
            short dch;
            short msx, msy;
            dch = nchecked ? 0 : 8;
            msx = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.RMouseX : lbDisplay.RMouseX;
            msy = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.RMouseY : lbDisplay.RMouseY;
            if (in_box(msx, msy, cx + dcx, cy + dcy, 76, 30 - dch))
                wep_highlight = true;
        }
        if (!wep_highlight) {
            short dch;
            short msx, msy;
            dch = nchecked ? 0 : 8;
            msx = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
            msy = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
            if (in_box(msx, msy, cx + dcx, cy + dcy, 76, 30 - dch))
                wep_highlight = true;
        }
        if (wep_highlight)
        {
            lbDisplay.DrawFlags = 0;
            p_locplayer->PanelItem[mouser] = weptype + 1;
            if (nchecked)
                draw_new_panel_sprite_std(cx, cy, 93);
            else
                draw_new_panel_sprite_std(cx, cy, 92);
            ret = 1;
        }

        if (nchecked)
            draw_fourpack_items(cx, cy + 4, comcur, weptype + 1);
        else
            draw_fourpack_items(cx + 16, cy + 8, comcur, weptype + 1);
        if (nchecked) {
            cy += 28;
        } else {
            cx += 16;
            cy += 32;
        }
        ++nchecked;
        ++nshown;
        if (nchecked == 12)
            break;
    }
    return ret;
}

TbBool func_1caf8(void)
{
#if 0
    TbBool ret;
    asm volatile ("call ASM_func_1caf8\n"
        : "=r" (ret) : );
    return ret;
#else
    TbBool ret;
    PlayerInfo *p_locplayer;
    struct Thing *p_agent;
    ubyte cur_weapons[4];
    int nagent;
    int panstate;

    p_locplayer = &players[local_player_no];
    if (p_locplayer->DoubleMode)
        p_agent = &things[p_locplayer->DirectControl[byte_153198-1]];
    else
        p_agent = &things[p_locplayer->DirectControl[0]];

    p_locplayer->PanelItem[mouser] = 0;
    ret = draw_panel_pickable_thing_below_agent(p_agent);
    if (!ret)
        draw_panel_pickable_thing_player_targeted(p_locplayer);

    if (ingame.PanelPermutation >= 0)
    {
        ushort comcur;
        comcur = p_agent->U.UPerson.ComCur & 3;
        ret = draw_weapons_list_single(p_locplayer, comcur,
            p_agent->U.UPerson.WeaponsCarried, p_agent->U.UPerson.CurrentWeapon);
    }
    else
    {
        for (nagent = 0; nagent < playable_agents; nagent++)
        {
            ushort curwep;
            p_agent = p_locplayer->MyAgent[nagent];
            if ((p_agent->Flag & TngF_Unkn0002) != 0) {
                cur_weapons[nagent] = 0;
                continue;
            }
            curwep = draw_current_weapon_button(p_locplayer, nagent);
            cur_weapons[nagent] = curwep;
        }

        ret = false;
        panstate = p_locplayer->PanelState[mouser];
        if (panstate >= 1 && panstate <= 8)
        {
            nagent = (panstate - 1) & 3;
            p_agent = p_locplayer->MyAgent[nagent];
            ret |= draw_agent_weapons_selection(p_locplayer, p_agent, cur_weapons, nagent);

        }
    }

    lbDisplay.DrawFlags = 0;
    return ret;
#endif
}

void func_1efb8(void)
{
    asm volatile ("call ASM_func_1efb8\n"
        :  :  : "eax" );
}

void func_702c0(int a1, int a2, int a3, int a4, int a5, ubyte a6)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_func_702c0\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5), "g" (a6));
}

void draw_shield_level(short x, short y, ushort w, ushort h)
{
#if 0
    asm volatile (
      "call ASM_draw_shield_level\n"
        : : "a" (x), "d" (y), "b" (w), "c" (h));
#else
    ubyte m;
    long waftx, wafty;
    ushort tmx, tmy;
    struct EnginePoint point4;
    struct EnginePoint point2;
    struct EnginePoint point1;
    struct EnginePoint point3;

    m = 1;
    if (lbDisplay.ScreenMode == 1)
        m = 0;
    point1.pp.X = x << m;
    point1.pp.Y = y << m;
    point4.pp.X = (x + w) << m;
    point4.pp.Y = y << m;
    point2.pp.Y = (y + h) << m;
    point2.pp.X = (x + w - 3) << m;
    point3.pp.Y = (y + h) << m;
    point3.pp.X = (x - 3) << m;

    // The shield bar is animated, even if it's not possible to see
    waftx = waft_table[(gameturn >> 3) & 31];
    wafty = waft_table[(gameturn + 16) & 31];
    tmx = ((waftx + 30) >> 1);
    tmy = ((wafty + 30) >> 3) + 64;
    point1.pp.U = tmx << 16;
    point4.pp.U = (tmx + 64) << 16;
    point2.pp.U = (tmx + 64) << 16;
    point1.pp.V = tmy << 16;
    point4.pp.V = tmy << 16;
    point2.pp.V = (tmy + 8) << 16;
    point3.pp.U = tmx << 16;
    point3.pp.V = (tmy + 8) << 16;

    point1.pp.S = 0;
    point2.pp.S = 0;
    point3.pp.S = 0;
    point4.pp.S = 0;

    vec_mode = 18;
    vec_map = dword_1AA280;
    draw_trigpoly(&point1.pp, &point4.pp, &point3.pp);
    if (vec_mode == 2)
        vec_mode = 27;
    draw_trigpoly(&point4.pp, &point2.pp, &point3.pp);
#endif
}

void draw_health_level(short x, short y, ushort w, ushort h, short lv, ushort lvmax, ubyte col, ubyte a8)
{
#if 0
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_health_level\n"
        : : "a" (x), "d" (y), "b" (w), "c" (h), "g" (lv), "g" (lvmax), "g" (col), "g" (a8));
#else
    short cw, ch;

    if ((lv <= 0) || (lvmax == 0))
        return;

    cw = w * lv / lvmax;
    if (a8)
    {
        draw_shield_level(x, y, cw, h);
    }
    else if (lbDisplay.ScreenMode == 1)
    {
        short cx, cy;
        cx = x;
        cy = y;
        for (ch = h; ch > 0; ch--)
        {
            if (lbDisplay.ScreenMode == 1)
                LbDrawLine(2 * cx >> 1, 2 * cy >> 1, 2 * (cx + cw) >> 1, 2 * cy >> 1, col);
            else
                LbDrawLine(2 * cx, 2 * cy, 2 * (cx + cw), 2 * cy, col);
            --cx;
            ++cy;
        }
    }
    else
    {
        short cx, cy;
        cx = 2 * x;
        cy = 2 * y;
        cw *= 2;
        for (ch = 2 * h; ch > 0; ch--)
        {
            if (lbDisplay.ScreenMode == 1)
                LbDrawLine(cx >> 1, cy >> 1, (cx + cw) >> 1, cy >> 1, col);
            else
                LbDrawLine(cx, cy, cx + cw, cy, col);
            --cx;
            ++cy;
        }
    }
#endif
}

void func_1eae4(int x, short y, int w, ushort h, short lv, ushort lvmax, ubyte col, int a8)
{
#if 0
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_func_1eae4\n"
        : : "a" (x), "d" (y), "b" (w), "c" (h), "g" (lv), "g" (lvmax), "g" (col), "g" (a8));
#else
    short cw, ch;

    if ((lv <= 0) || (lvmax == 0))
        return;

    ch = h * lv / lvmax;
    if (lbDisplay.ScreenMode == 1)
    {
        short cx, cy;
        cx = x;
        cy = y;
        for (cw = w; cw > 0; cw--)
        {
            short cy1, cy2;
            cy1 = cy + h;
            cy2 = cy + h - ch;
            if (lbDisplay.ScreenMode == 1)
                SCANNER_unkn_func_203(2 * cx >> 1, 2 * cy1 >> 1, 2 * cx >> 1, 2 * cy2 >> 1, col,
                    ingame.Scanner.Contrast, ingame.Scanner.Brightness);
            else
                SCANNER_unkn_func_203(2 * cx, 2 * cy1, 2 * cx, 2 * cy2, col,
                    ingame.Scanner.Contrast, ingame.Scanner.Brightness);
            ++cx;
            ++cy;
        }
    }
    else
    {
        short cx, cy;
        cx = 2 * x;
        cy = 2 * y;
        for (cw = 2 * w; cw > 0; cw--)
        {
            short cy1, cy2;
            cy1 = 2*h + cy;
            cy2 = 2*h + cy - 2*ch;
            if (lbDisplay.ScreenMode == 1)
                LbDrawLine(cx >> 1, cy1 >> 1, cx >> 1, cy2 >> 1, col);
            else
                LbDrawLine(cx, cy1, cx, cy2, col);
            ++cx;
            ++cy;
        }
    }
#endif
}

void func_1ec68(short a1, short a2, ushort a3, int a4, int a5)
{
    asm volatile (
      "push %4\n"
      "call ASM_func_1ec68\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5));
}

void func_1ee14(short a1, short a2, short a3, short a4, int a5, int a6)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_func_1ee14\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5), "g" (a6));
}

void draw_unkn1_rect(int x1, int y1, int len_mul, int len_div)
{
    asm volatile (
      "call ASM_draw_unkn1_rect\n"
        : : "a" (x1), "d" (y1), "b" (len_mul), "c" (len_div));
}

void draw_new_panel()
{
#if 0
    asm volatile ("call ASM_draw_new_panel\n"
        :  :  : "eax" );
#else
    int i;
    PlayerInfo *p_locplayer;

    p_locplayer = &players[local_player_no];
    // If an agent has a medkit, use the sprite with lighted cross
    for (i = 0; i < playable_agents; i++)
    {
        struct Thing *p_agent;
        p_agent = p_locplayer->MyAgent[i];
        if (person_carries_weapon(p_agent, WEP_MEDI2) || person_carries_weapon(p_agent, WEP_MEDI1))
            game_panel[8+i].Spr = 96;
        else
            game_panel[8+i].Spr = 95;
    }

    { // If supershield is enabled for the current agent, draw energy bar in red
        struct Thing *p_agent;
        p_agent = &things[p_locplayer->DirectControl[0]];
        if ((p_agent->Flag & TngF_Unkn0100) != 0)
        {
            game_panel[16].Spr = 99;
            if (lbDisplay.ScreenMode != 1)
                game_panel[17].Spr = 106;
        }
        else
        {
            game_panel[16].Spr = 10;
            if (lbDisplay.ScreenMode != 1)
                game_panel[17].Spr = 105;
        }
    }

    for (i = 0; true; i++)
    {
        struct GamePanel *panel;

        panel = &game_panel[i];
        if (panel->Spr < 0)
          break;
        if (panel->Spr == 0)
          continue;
        lbDisplay.DrawFlags = 0;

        if ( panel->Type != 1 && panel->Type != 6 && panel->Type != 5 )
        {
            draw_new_panel_sprite_std(panel->X, panel->Y, panel->Spr);
        }
        else
        {
            TbBool is_visible;

            is_visible = true;
            if (panel->Type == 5 && panel->ID < playable_agents)
            {
                struct Thing *p_agent;
                ubyte weapon;

                p_agent = p_locplayer->MyAgent[panel->ID];
                if (p_agent < &things[0] || p_agent > &things[THINGS_LIMIT])
                    break;
                weapon = p_agent->U.UPerson.CurrentWeapon;
                if (weapon == 0)
                    weapon = p_locplayer->PrevWeapon[panel->ID];
                is_visible = !p_locplayer->WepDelays[panel->ID][weapon] || (gameturn & 1);
            }

            if (p_locplayer->DoubleMode)
            {
                struct Thing *p_agent;
                if (!is_visible)
                    continue;
                if (panel->ID >= playable_agents)
                    continue;
                p_agent = p_locplayer->MyAgent[panel->ID];
                if (p_agent->Flag & TngF_Unkn0002)
                    continue;
                draw_new_panel_sprite_std(panel->X, panel->Y, panel->Spr);
            }
            else
            {
                struct Thing *p_agent;
                if (!is_visible)
                    continue;
                if (panel->ID >= playable_agents)
                    continue;
                p_agent = p_locplayer->MyAgent[panel->ID];
                if (p_agent->Flag & TngF_Unkn0002)
                    continue;

                if (panel->Type == 5) {
                    ubyte weapon;

                    weapon = p_agent->U.UPerson.CurrentWeapon;
                    if (weapon == 0)
                        weapon = p_locplayer->PrevWeapon[panel->ID];
                    if (weapon == 0)
                        continue;
                }

                if ((p_agent->State == PerSt_PROTECT_PERSON) || (p_agent->Flag2 & 0x10000000))
                    draw_new_panel_sprite_dark(panel->X, panel->Y, panel->Spr);
                else
                    draw_new_panel_sprite_std(panel->X, panel->Y, panel->Spr);
            }
        }
    }

    if (gameturn & 4)
    {
        int x, cc;
        int agent;
        struct Thing *p_agent;

        x = 0;
        agent = p_locplayer->DoubleMode ? p_locplayer->DirectControl[byte_153198-1] : p_locplayer->DirectControl[0];
        p_agent = &things[agent];
        if ((p_agent->Flag & TngF_Unkn0002) == 0 && (p_agent->Flag2 & 0x800) == 0)
        {
            cc = p_agent->U.UPerson.ComCur & 3;
            if (lbDisplay.ScreenMode == 1)
            {
                switch (cc)
                {
                case 0:
                  x = 4;
                  break;
                case 1:
                  x = 154;
                  break;
                case 2:
                  x = 308;
                  break;
                case 3:
                  x = 472;
                  break;
                }
            }
            else
            {
                switch (cc)
                {
                case 0:
                  x = 4;
                  break;
                case 1:
                  x = 153;
                  break;
                case 2:
                  x = 306;
                  break;
                case 3:
                  x = 469;
                  break;
                }
            }
            draw_new_panel_sprite_std(x, 2, 6 + cc);
        }
    }
    lbDisplay.DrawFlags = 0;

    if (!func_1caf8())
    {
        if (ingame.Flags & GamF_Unkn0200) {
            ulong md, y;
            md = p_locplayer->UserInput[0].ControlMode & 0x1FFF;
            if (md == 1 && pktrec_mode != PktR_PLAYBACK) {
                y = alt_at_point(mouse_map_x, mouse_map_z);
                func_702c0(mouse_map_x, y >> 5, mouse_map_z, 64, 64, colour_lookup[2]);
            }
        }
    }
    func_1efb8();

    // Fill the left energy bar
    {
        int agent;
        struct Thing *p_agent;
        int lv, lvmax, col, w;

        agent = p_locplayer->DoubleMode ? p_locplayer->DirectControl[byte_153198-1] : p_locplayer->DirectControl[0];
        p_agent = &things[agent];
        if ((p_agent->U.UPerson.Energy < 50) && (gameturn & 1))
            col = 2;
        else
            col = 1;
        lvmax = p_agent->U.UPerson.MaxEnergy;
        lv = p_agent->U.UPerson.Energy;
        if (lbDisplay.ScreenMode == 1)
            w = 0;
        else
            w = 45;
        func_1eae4(3, 30, 4, 54+w, lv, lvmax, colour_lookup[col], 0);
    }

    // Fill the agents bar at top
    for (i = 0; i < playable_agents; i++)
    {
        struct Thing *p_agent;
        int lv, lvmax, x, w;

        p_agent = p_locplayer->MyAgent[i];
        if ((p_agent->Flag & TngF_PlayerAgent) == 0) {
            LOGERR("Agent %d unexpected flags", i);
            return;
        }
        if ((p_agent->Flag & TngF_Unkn0002) == 0)
        {
            x = 79 * i + 27;
            // Draw health level
            lv = p_agent->Health;
            lvmax = p_agent->U.UPerson.MaxHealth;
            if (lv <= lvmax) { // Normal health amount
                draw_health_level(x, 2, 44, 2, lv, lvmax, colour_lookup[1], 0);
            } else { // Health reinforced beyond max is drawn in red
                draw_health_level(x, 2, 44, 2, lvmax, lvmax, colour_lookup[1], 0);
                draw_health_level(x, 2, 44, 2, lv - lvmax, lvmax, colour_lookup[2], 0);
            }
            // Draw shield level over health
            lv = p_agent->U.UPerson.ShieldEnergy;
            draw_health_level(x, 2, 44, 2, lv, 0x400, colour_lookup[1], 1);
            // Draw drug level aka mood (or just a red line if no drugs)
            w = game_panel[4+i].Width;
            x = game_panel[4+i].X >> 1;
            func_1ec68(x, 6, w, 3, p_agent->U.UPerson.Mood);
            // Draw stamina level which caps the mood level
            lv = p_agent->U.UPerson.Stamina;
            lvmax = p_agent->U.UPerson.MaxStamina;
            func_1ee14(x, 6, w, 2, lv, lvmax);
            if (lbDisplay.ScreenMode == 1)
                x = 158 * i + 54;
            else
                x = 157 * i + 54;
            lv = p_agent->U.UPerson.Energy;
            lvmax = p_agent->U.UPerson.MaxEnergy;
            draw_unkn1_rect(x + 80, 18, lv, lvmax);
        }
    }

    ingame.Scanner.MX = engn_xc >> 7;
    ingame.Scanner.MZ = engn_zc >> 7;
    ingame.Scanner.Angle = 2047 - ((dword_176D58 >> 5) & 0x7FF);
    SCANNER_draw_new_transparent();

    // Objective text, or net players list
    {
        int x, y, w;
        x = ingame.Scanner.X1 - 1;
        if (x < 0)
            x = 0;
        if (lbDisplay.GraphicsScreenWidth >= 640)
            y = lbDisplay.GraphicsScreenHeight - 18;
        else
            y = lbDisplay.GraphicsScreenHeight - 9;
        if (in_network_game) {
            SCANNER_unkn_func_205();
            w = lbDisplay.PhysicalScreenWidth;
        } else {
            // original width 67 low res, 132 high res
            w = ingame.Scanner.X2 - ingame.Scanner.X1 + 3;
        }
        SCANNER_unkn_func_204(x, y, w);
    }

    // Thermal vision button light
    if ((ingame.Flags & GamF_Unkn8000) != 0) {
        int x;
        x = 238;
        if (lbDisplay.ScreenMode != 1)
            x += 89;
        draw_new_panel_sprite_std(4, x, 91);
    }
#endif
}

void draw_screen(void)
{
    asm volatile ("call ASM_draw_screen\n"
        :  :  : "eax" );
}

void process_engine_unk3(void)
{
    asm volatile ("call ASM_process_engine_unk3\n"
        :  :  : "eax" );
}

void setup_engine_nullsub4(void)
{
}

void process_sound_heap(void)
{
    asm volatile ("call ASM_process_sound_heap\n"
        :  :  : "eax" );
}

void func_2e440(void)
{
    asm volatile ("call ASM_func_2e440\n"
        :  :  : "eax" );
}

void func_cc0d4(char **str)
{
    asm volatile ("call ASM_func_cc0d4\n"
        : : "a" (str));
}

void init_outro(void)
{
#if 0
    asm volatile ("call ASM_init_outro\n"
        : : );
    return;
#else
    TbClockMSec last_loop_time;
    struct Campaign *p_campgn;
    const char *text1;
    const char *text2;
    TbFileHandle fh;
    int i;

    gamep_unknval_01 = 0;
    StopAllSamples();
    StopCD();
    show_black_screen();
    swap_wscreen();
    setup_screen_mode(screen_mode_fmvid_lo);
    LbMouseChangeSprite(0);
    lbKeyOn[KC_SPACE] = 0;
    lbKeyOn[KC_RETURN] = 0;
    lbKeyOn[KC_ESCAPE] = 0;

    p_campgn = &campaigns[background_type];

    {
        char fname[FILENAME_MAX];

        sprint_fmv_filename(MPly_Outro, fname, sizeof(fname));
        if (fname[0] != '\0') {
            play_smk(fname, 13, 0);
        }
    }
    data_155704 = -1;
    screen_buffer_fill_black();

    fh = LbFileOpen(p_campgn->OutroBkFn, Lb_FILE_MODE_READ_ONLY);
    if (fh != INVALID_FILE)
    {
        for (i = 24; i != 24+150; i++)
        {
            ubyte *buf;
            buf = &lbDisplay.WScreen[i*lbDisplay.GraphicsScreenWidth + 10];
            LbFileRead(fh, buf, 300);
        }
    }
    LbFileClose(fh);

    LbFileLoadAt("qdata/pal.pal", display_palette);
    LbScreenWaitVbi();
	game_hacky_update();

    {
        char str[FILENAME_MAX];
        sprintf(str, "qdata/pal%d.dat", 0);
        LbFileLoadAt(str, display_palette);
        LbPaletteSet(display_palette);
    }
    swap_wscreen();
    screen_dark_curtain_down();
    init_things();
    change_current_map(51);
    load_outro_sprites();

    switch (background_type)
    {
    case 0:
    default:
        text1 = unk_credits_text_s;
        text2 = outro_text_s;
        break;
    case 1:
        text1 = unk_credits_text_z;
        text2 = outro_text_z;
        break;
    case 2:
        text1 = unk_credits_text_p;
        text2 = outro_text_z;
        break;
    }
    func_cc638(text1, text2);

    // Sleep for up to 10 seconds
    last_loop_time = LbTimerClock();
    for (i = 10*GAME_FPS; i != 0; i--)
    {
        if ( lbKeyOn[KC_SPACE] )
          break;
        if ( lbKeyOn[KC_ESCAPE] )
          break;
        if ( lbKeyOn[KC_RETURN] )
          break;
        TbClockMSec sleep_end = last_loop_time + 1000/GAME_FPS;
        LbSleepUntil(sleep_end);
        last_loop_time = LbTimerClock();
    }
    lbKeyOn[KC_SPACE] = 0;
    lbKeyOn[KC_ESCAPE] = 0;
    lbKeyOn[KC_RETURN] = 0;

    LbPaletteFade(0, 0xC8u, 1);
    screen_buffer_fill_black();
    swap_wscreen();
    StopAllSamples();
    reset_heaps();
    setup_heaps(100);
    play_sample_using_heap(0, 1, 127, 64, 100, -1, 3u);

    data_197150 = 1;
    data_1dd91c = 0;
    unkn_flags_01 = 1;
    overall_scale = 40;
    palette_brightness = 0;
    change_brightness(-64);

    for (i = 0; i < 128; i++)
    {
        if (i & 1)
          change_brightness(1);
        traffic_unkn_func_01();
        process_engine_unk1();
        process_sound_heap();
        func_2e440();
        swap_wscreen();
        screen_buffer_fill_black();
    }

    while (1)
    {
        if ( lbKeyOn[KC_SPACE] )
          break;
        if ( lbKeyOn[KC_ESCAPE] )
          break;
        if ( lbKeyOn[KC_RETURN] )
          break;
        gameturn++;
        traffic_unkn_func_01();
        process_engine_unk1();
        // Play applause sample
        if (((LbRandomAnyShort() & 0xF) == 0) && (data_155704 == -1 || !IsSamplePlaying(0, data_155704, NULL)) )
        {
            //TODO Why we're not storing new value of data_155704 ??
            play_sample_using_heap(0, 7 + (LbRandomAnyShort() % 5), 127, 64, 100, 0, 3u);
        }
        process_sound_heap();
        func_2e440();
        if (outro_unkn01)
        {
            outro_unkn02++;
            func_cc0d4((char **)&people_credits_groups[2 * outro_unkn03]);
            if (data_1ddb68 + 50 < outro_unkn02)
            {
              outro_unkn02 = 0;
              if (++outro_unkn03 == people_groups_count)
                  outro_unkn03 = 0;
            }
          }
          swap_wscreen();
          screen_buffer_fill_black();
    }
    StopAllSamples();
    reset_heaps();
    setup_heaps(2);
    // We've overwritten area used by campaigns; reload these
    load_campaigns();
    load_missions(background_type);
#endif
}

void load_pop_sprites_lo(void)
{
    asm volatile ("call ASM_load_pop_sprites_lo\n"
        :  :  : "eax" );
}

void load_pop_sprites_hi(void)
{
    asm volatile ("call ASM_load_pop_sprites_hi\n"
        :  :  : "eax" );
}

void srm_scanner_set_size_at_bottom_left(int margin, int width, int height)
{
    int i;
    int hlimit, cutout;

    hlimit = sizeof(ingame.Scanner.Width)/sizeof(ingame.Scanner.Width[0]);
    if (height >= hlimit)
        height = hlimit - 1;
    cutout = 24;

    ingame.Scanner.X1 = 1;
    ingame.Scanner.Y1 = lbDisplay.GraphicsScreenHeight - margin - height;
    ingame.Scanner.X2 = ingame.Scanner.X1 + width;
    ingame.Scanner.Y2 = ingame.Scanner.Y1 + height;

    for (i = 0; i + ingame.Scanner.Y1 <= ingame.Scanner.Y2; i++) {
        ingame.Scanner.Width[i] = min(width - cutout + i, width);
    }
}

void srm_scanner_size_update(void)
{
    int margin, width, height;

    width = lbDisplay.GraphicsScreenWidth * scanner_width_pct / 100;
    height = lbDisplay.GraphicsScreenHeight * scanner_height_pct / 100;
    if (lbDisplay.GraphicsScreenWidth >= 640) {
        margin = 20;
        width = width * 101 / 100;
        height = height * 99 / 100;
    } else {
        margin = 11;
        // width without change
        height = height * 124 / 100;
    }
    srm_scanner_set_size_at_bottom_left(margin, width, height);
}

void init_scanner(void)
{
    ubyte col;

    if (ingame.PanelPermutation != 2 && ingame.PanelPermutation != -3)
        col = 2;
    else
        col = 1;
    SCANNER_set_colour(col);
    dword_1AA5C4 = 0;
    dword_1AA5C8 = 0;
    SCANNER_fill_in();
    ingame.Scanner.Brightness = 8;
    ingame.Scanner.Contrast = 5;
    SCANNER_width = ingame.Scanner.Width;
    ingame.Scanner.Zoom = 128;
    ingame.Scanner.Angle = 0;
    srm_scanner_size_update();
    SCANNER_init();
}

/**
 * Updates engine parameters for best display for current video mode within the tactical mission.
 */
void adjust_mission_engine_to_video_mode(void)
{
    game_high_resolution = (lbDisplay.ScreenMode == screen_mode_game_hi);
    if (lbDisplay.GraphicsScreenWidth >= 640)
    {
        overall_scale = 400;
        load_pop_sprites_hi();
        render_area_a = 30;
        render_area_b = 30;
    }
    else
    {
        overall_scale = 150;
        load_pop_sprites_lo();
        render_area_a = 24;
        render_area_b = 24;
    }
    srm_scanner_size_update();
}

void video_mode_switch_to_next(void)
{
    TbScreenMode nmode;

    if (lbDisplay.ScreenMode == screen_mode_game_lo)
        nmode = screen_mode_game_hi;
    else
        nmode = screen_mode_game_lo;
    LOGSYNC("Switching to mode %d", (int)nmode);

    StopCD();
    setup_screen_mode(nmode);
    adjust_mission_engine_to_video_mode();
}

void teleport_current_agent(PlayerInfo *p_locplayer)
{
    int dctrl_thing;
    dctrl_thing = p_locplayer->DirectControl[mouser];
    delete_node(&things[dctrl_thing]);
    things[dctrl_thing].X = mouse_map_x << 8;
    things[dctrl_thing].Z = mouse_map_z << 8;
    things[dctrl_thing].Y = alt_at_point(mouse_map_x, mouse_map_z);
    add_node_thing(dctrl_thing);
}

void person_resurrect(struct Thing *p_person)
{
    ulong person_anim;
    p_person->Flag &= ~TngF_Unkn0002;
    p_person->Flag &= ~TngF_Unkn02000000;
    p_person->State = PerSt_WAIT;
    p_person->U.UPerson.AnimMode = 1;
    person_anim = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode];
    p_person->StartFrame = person_anim - 1;
    p_person->Frame = nstart_ani[person_anim + p_person->U.UPerson.Angle];
}

void person_give_all_weapons(struct Thing *p_person)
{
    p_person->U.UPerson.WeaponsCarried = 0x3FFBDFFF;
    do_weapon_quantities1(p_person);
}

void beefup_all_agents(PlayerInfo *p_locplayer)
{
    int i;
    for (i = 0; i < playable_agents; i++)
    {
        struct Thing *p_agent;
        p_agent = p_locplayer->MyAgent[i];
        if (p_agent->Flag & TngF_Unkn0002)
            person_resurrect(p_agent);
        person_give_all_weapons(p_agent);
        if (lbShift & KMod_SHIFT)
        {
            person_give_best_mods(p_agent);

            p_agent->U.UPerson.MaxHealth = PERSON_MAX_HEALTH_LIMIT;
            p_agent->Health = 2 * p_agent->U.UPerson.MaxHealth; // double health - fill red bar
            p_agent->U.UPerson.MaxEnergy = 32000;
            p_agent->U.UPerson.Energy = p_agent->U.UPerson.MaxEnergy;
        }
    }
    research.WeaponsCompleted = 0x3FFFFFFF;
}

void game_graphics_inputs(void)
{
#if 0
    asm volatile ("call ASM_game_graphics_inputs\n"
        : : );
    return;
#else
    PlayerInfo *p_locplayer;

    p_locplayer = &players[local_player_no];
    if ((ingame.DisplayMode != DpM_UNKN_32) &&
        (ingame.DisplayMode != DpM_UNKN_3B))
        return;
    if (in_network_game && p_locplayer->PanelState[mouser] != 17)
        return;

    if (lbKeyOn[KC_F] && (lbShift == KMod_NONE))
    {
        lbKeyOn[KC_F] = 0;
        if (game_perspective == 5)
            game_perspective = 0;
        else
            game_perspective = 5;
    }

    if ((ingame.Cheats & 0x04) && lbKeyOn[KC_T] && (lbShift & KMod_ALT))
    {
        lbKeyOn[KC_T] = 0;
        teleport_current_agent(p_locplayer);
    }

    if ((ingame.Cheats & 0x04) && !in_network_game)
    {
        if (lbKeyOn[KC_Q] && (lbShift == KMod_SHIFT || lbShift == KMod_NONE))
        {
            lbKeyOn[KC_Q] = 0;
            beefup_all_agents(p_locplayer);
        }
    }

    if (lbKeyOn[KC_F8])
    {
        lbKeyOn[KC_F8] = 0;
        video_mode_switch_to_next();
    }
#endif
}

void init_syndwars(void)
{
#if 0
    asm volatile ("call ASM_init_syndwars\n"
        :  :  : "eax" );
#endif
    //char locstr[100];
    AudioInitOptions audOpts;

    LOGSYNC("Starting");
    //sprintf(locstr, "%sSound", cd_drive); -- unused

    audOpts.SoundDataPath = "Sound";
    audOpts.SoundDriverPath = "Sound";
    audOpts.IniPath = "Sound";
    audOpts.AutoScan = 1;
    audOpts.StereoOption = 1;
    audOpts.DisableLoadSounds = 1;
    audOpts.InitRedbookAudio = 2;
    audOpts.UseCurrentAwe32Soundfont = 1;
    audOpts.AbleFlags = 3;
    audOpts.SoundType = 1622;
    audOpts.MaxSamples = 10;
    InitAudio(&audOpts);

    if (!GetCDAble())
        StopMusicIfActive();
    fill_ail_sample_ids();
    if (GetSoundInstalled() && GetSoundAble() && GetSoundActive())
        ingame.Flags |= GamF_Unkn00020000;
    setup_heaps(0);
}

void setup_host_sub6(void)
{
    asm volatile ("call ASM_setup_host_sub6\n"
        :  :  : "eax" );
}

void setup_debug_obj_trace(void)
{
    // Code for setting memory ranges was here
    // Removed, as we no longer have such tracking/profiling mechanism
    debug_trace_place(0);
}

void set_smack_malloc(void *(*cb)(int))
{
    smack_malloc = cb;
}

void set_smack_free(void (*cb)(void *ptr))
{
    smack_free = cb;
}

void BAT_unknsub_20(int a1, int a2, int a3, int a4, unsigned long a5)
{
    asm volatile (
      "push %4\n"
      "call ASM_BAT_unknsub_20\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5));
}

int joy_func_067(struct DevInput *dinp, int a2)
{
    int ret;
    asm volatile ("call ASM_joy_func_067\n"
        : "=r" (ret) : "a" (dinp), "d" (a2));
    return ret;
}

void setup_host(void)
{
    BAT_unknsub_20(0, 0, 0, 0, unkn_buffer_04 + 41024);
    set_smack_malloc(ASM_smack_malloc);
    set_smack_free(ASM_smack_mfree);
    LOGDBG("&setup_host() = 0x%lx", (ulong)setup_host);
    {
        TbScreenModeInfo *mdinfo;

        if (game_high_resolution)
            lbDisplay.ScreenMode = screen_mode_game_hi;
        else
            lbDisplay.ScreenMode = screen_mode_game_lo;
        mdinfo = LbScreenGetModeInfo(lbDisplay.ScreenMode);
        LbScreenSetup(lbDisplay.ScreenMode, mdinfo->Width, mdinfo->Height, display_palette);
    }
    LbSpriteSetup(pointer_sprites, pointer_sprites_end, pointer_data);
    { // Make mouse pointer sprite 1 an empty (zero size) sprite
        struct TbSprite *spr;
        spr = &pointer_sprites[1];
        spr->SWidth = 0;
        spr->SHeight = 0;
    }
    if ( keyboard_mode_direct )
        LbKeyboardOpen();
    else
        LbIKeyboardOpen();
    init_buffered_keys();
    lbMouseAutoReset = false;
    LbMouseSetup(&pointer_sprites[1], 2, 2);

    setup_debug_obj_trace();
    LbSpriteSetup(m_sprites, m_sprites_end, m_spr_data);
    ingame.PanelPermutation = -2;
    {
        char fname[DISKPATH_SIZE];
        int file_len;
        sprintf(fname, pop_dat_fname_fmt, 1);
        LbFileLoadAt(fname, pop1_data);
        sprintf(fname, pop_tab_fname_fmt, -ingame.PanelPermutation - 1);
        file_len = LbFileLoadAt(fname, pop1_sprites);
        pop1_sprites_end = &pop1_sprites[file_len/sizeof(struct TbSprite)];
        LbSpriteSetup(pop1_sprites, pop1_sprites_end, pop1_data);
    }
    ingame.TrenchcoatPreference = 0;
    game_panel = game_panel_lo;
    LbGhostTableGenerate(display_palette, 50, "data/synghost.tab");
    init_memory(mem_game);

    init_syndwars();
    LoadSounds(0);
    LoadMusic(0);

    setup_host_sub6();
    if (pktrec_mode == PktR_RECORD)
    {
      if ( !in_network_game )
      {
          PacketRecord_OpenWrite();
      }
    }
    if (pktrec_mode == PktR_PLAYBACK)
    {
        PacketRecord_OpenRead();
    }
    play_intro();
    if ( cmdln_param_bcg )
    {
        TbScreenModeInfo *mdinfo;

        lbDisplay.ScreenMode = screen_mode_menu;
        mdinfo = LbScreenGetModeInfo(lbDisplay.ScreenMode);
        LbScreenSetup(lbDisplay.ScreenMode, mdinfo->Width, mdinfo->Height, display_palette);
        LOGDBG("Video mode %dx%d", mdinfo->Width, mdinfo->Height);
    }
    LbMouseSetup(&pointer_sprites[1], 2, 2);
    if (cmdln_param_bcg)
        LbMouseChangeSprite(NULL);
}

void set_default_user_settings(void)
{
    asm volatile ("call ASM_set_default_user_settings\n"
        :  :  : "eax" );
}

void apply_user_settings(void)
{
    if (unkn_gfx_option_2)
        ingame.Flags |= GamF_Unkn0002;
    else
        ingame.Flags &= ~GamF_Unkn0002;

    if (unkn_option_3)
        ingame.Flags |= GamF_Unkn0001;
    else
        ingame.Flags &= ~GamF_Unkn0001;

    if (unkn_option_4)
        ingame.Flags |= GamF_Unkn0400;
    else
        ingame.Flags &= ~GamF_Unkn0400;

    bang_set_detail(ingame.DetailLevel == 0);
    SetSoundMasterVolume(127 * startscr_samplevol / 322);
    SetMusicMasterVolume(127 * startscr_midivol / 322);
    SetCDVolume((70 * (127 * startscr_cdvolume / 322) / 100));
}

void read_user_settings(void)
{
#if 0
    asm volatile ("call ASM_read_user_settings\n"
        :  :  : "eax" );
#else
    char fname[52];
    TbFileHandle fh;
    TbBool read_mortal_salt_backup;
    int i;

    read_mortal_salt_backup = false;
    get_user_settings_fname(fname, login_name);

    fh = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);

    // Try default settings file instead
    if ((fh == INVALID_FILE) && (strlen(login_name) > 0))
    {
        get_user_settings_fname(fname, "");
        fh = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
        read_mortal_salt_backup = true;
    }

    if (fh == INVALID_FILE)
    {
        set_default_user_settings();
        read_mortal_salt_backup = true;
    } else
    {
        ushort cheats;

        LbFileRead(fh, kbkeys, 23 * sizeof(ushort));
        LbFileRead(fh, jskeys, 23 * sizeof(ushort));
        LbFileRead(fh, &byte_1C4A9F, 1);
        LbFileRead(fh, &players[local_player_no].DoubleMode,
          sizeof(players[local_player_no].DoubleMode));
        for (i = 0; i != 4; i++)
        {
            PlayerInfo *p_locplayer;
            p_locplayer = &players[local_player_no];
            LbFileRead(fh, &p_locplayer->UserInput[i].ControlMode,
              sizeof(p_locplayer->UserInput[i].ControlMode));
        }
        LbFileRead(fh, &startscr_samplevol, sizeof(startscr_samplevol));
        LbFileRead(fh, &startscr_midivol, sizeof(startscr_midivol));
        LbFileRead(fh, &startscr_cdvolume, sizeof(startscr_cdvolume));
        LbFileRead(fh, &unkn_gfx_option_2, sizeof(unkn_gfx_option_2));
        LbFileRead(fh, &unkn_option_3, sizeof(unkn_option_3));
        LbFileRead(fh, &unkn_option_4, sizeof(unkn_option_4));
        LbFileRead(fh, &ingame.DetailLevel, sizeof(ingame.DetailLevel));
        LbFileRead(fh, &game_high_resolution, sizeof(game_high_resolution));
        LbFileRead(fh, &game_projector_speed, sizeof(game_projector_speed));
        LbFileRead(fh, &game_perspective, sizeof(game_perspective));
        LbFileRead(fh, &ingame.PanelPermutation, sizeof(ingame.PanelPermutation));
        LbFileRead(fh, &ingame.TrenchcoatPreference, sizeof(ingame.TrenchcoatPreference));
        LbFileRead(fh, &ingame.CDTrack, sizeof(ingame.CDTrack));
        LbFileRead(fh, &ingame.DangerTrack, sizeof(ingame.DangerTrack));
        LbFileRead(fh, &ingame.UseMultiMedia, sizeof(ingame.UseMultiMedia));
        LbFileRead(fh, &cheats, sizeof(cheats));
        LbFileRead(fh, &save_mortal_salt, sizeof(save_mortal_salt));
        LbFileClose(fh);

        apply_user_settings();
    }

    if (read_mortal_salt_backup)
    {
        // Read mortal game encryption salt from backup
        fh = LbFileOpen("qdata/keys.dat", Lb_FILE_MODE_READ_ONLY);
        if (fh != INVALID_FILE)
        {
            i = sizeof(save_mortal_salt);
            LbFileSeek(fh, -i, Lb_FILE_SEEK_END);
            LbFileRead(fh, &save_mortal_salt, i);
            LbFileClose(fh);
        }
    }

    i = -1;
    if (byte_1C4A9F)
        i = joy_func_067(&joy, byte_1C4A9F);
    if (i != 1)
        byte_1C4A9F = 0;
#endif
}

ubyte save_user_settings(void)
{
#if 0
    TbBool ret;
    asm volatile ("call ASM_save_user_settings\n"
        : "=r" (ret) : );
    return ret;
#endif
    char fname[52];
    TbFileHandle fh;
    int i;

    get_user_settings_fname(fname, login_name);

    fh = LbFileOpen(fname, Lb_FILE_MODE_NEW);
    if (fh == INVALID_FILE)
        return 1;

    LbFileWrite(fh, kbkeys, 23 * sizeof(ushort));
    LbFileWrite(fh, jskeys, 23 * sizeof(ushort));
    LbFileWrite(fh, &byte_1C4A9F, sizeof(byte_1C4A9F));
    LbFileWrite(fh, &players[local_player_no].DoubleMode,
      sizeof(players[local_player_no].DoubleMode));
    for (i = 0; i != 4; i++)
    {
        PlayerInfo *p_locplayer;
        p_locplayer = &players[local_player_no];
        LbFileWrite(fh, &p_locplayer->UserInput[i].ControlMode,
          sizeof(p_locplayer->UserInput[i].ControlMode));
    }
    LbFileWrite(fh, &startscr_samplevol, sizeof(startscr_samplevol));
    LbFileWrite(fh, &startscr_midivol, sizeof(startscr_midivol));
    LbFileWrite(fh, &startscr_cdvolume, sizeof(startscr_cdvolume));
    LbFileWrite(fh, &unkn_gfx_option_2, sizeof(unkn_gfx_option_2));
    LbFileWrite(fh, &unkn_option_3, sizeof(unkn_option_3));
    LbFileWrite(fh, &unkn_option_4, sizeof(unkn_option_4));
    LbFileWrite(fh, &ingame.DetailLevel, sizeof(ingame.DetailLevel));
    LbFileWrite(fh, &game_high_resolution, sizeof(game_high_resolution));
    LbFileWrite(fh, &game_projector_speed, sizeof(game_projector_speed));
    LbFileWrite(fh, &game_perspective, sizeof(game_perspective));
    LbFileWrite(fh, &ingame.PanelPermutation, sizeof(ingame.PanelPermutation));
    LbFileWrite(fh, &ingame.TrenchcoatPreference, sizeof(ingame.TrenchcoatPreference));
    LbFileWrite(fh, &ingame.CDTrack, sizeof(ingame.CDTrack));
    LbFileWrite(fh, &ingame.DangerTrack, sizeof(ingame.DangerTrack));
    LbFileWrite(fh, &ingame.UseMultiMedia, sizeof(ingame.UseMultiMedia));
    LbFileWrite(fh, &ingame.Cheats, sizeof(ingame.Cheats));
    LbFileWrite(fh, &save_mortal_salt, sizeof(save_mortal_salt));
    LbFileClose(fh);
    return 0;
}

void setup_color_lookups(void)
{
    asm volatile ("call ASM_setup_color_lookups\n"
        :  :  : "eax" );
}

void init_engine(void)
{
    asm volatile ("call ASM_init_engine\n"
        :  :  : "eax" );
}

void unkn_truce_groups(void)
{
    asm volatile ("call ASM_unkn_truce_groups\n"
        :  :  : "eax" );
}

void blind_progress_game(ulong nturns)
{
    ulong n;

    for (n = 0; n < nturns; n++)
    {
        process_things();
        gameturn++;
    }
}

void init_level(void)
{
    asm volatile ("call ASM_init_level\n"
        :  :  : "eax" );
}

void init_level_3d(ubyte flag)
{
    asm volatile ("call ASM_init_level_3d\n"
        : : "a" (flag));
}

void unkn1_handle_agent_groups(void)
{
    asm volatile ("call ASM_unkn1_handle_agent_groups\n"
        :  :  : "eax" );
}

void init_game_controls(void)
{
    asm volatile ("call ASM_init_game_controls\n"
        :  :  : "eax" );
}

void simulated_level(void)
{
    asm volatile ("call ASM_simulated_level\n"
        :  :  : "eax" );
}

/** Initializes player presence on a level.
 *
 * CurrentMission needs to be set before this funcion is called.
 */
void init_player(void)
{
#if 0
    asm volatile ("call ASM_init_player\n"
        :  :  : "eax" );
#endif
    PlayerInfo *p_locplayer;
    int i;

    p_locplayer = &players[local_player_no];

    gamep_unknval_10 = 0;
    gamep_unknval_12 = 0;
    nav_stats__ThisTurn = 0;
    ingame.Flags &= ~GamF_Unkn0100;
    gamep_unknval_16 = 0;
    init_level_3d(0);
    init_level();
    if (in_network_game)
    {
        unkn1_handle_agent_groups();
        unkn_truce_groups();
    }
    else
    {
        struct Thing *p_agent;
        place_single_player();
        p_agent = p_locplayer->MyAgent[0];
        if (p_agent != NULL) {
            ingame.TrackX = p_agent->X >> 8;
            ingame.TrackZ = p_agent->Z >> 8;
        } else {
            ingame.TrackX = 128;
            ingame.TrackZ = 128;
        }
    }
    player_debug(" after place player");
    if (current_level == 0)
        simulated_level();
    player_debug(" after sim level");
    gameturn = 1;
    player_debug(" after group action");
    {
        struct Thing *p_agent;
        p_agent = p_locplayer->MyAgent[0];
        if (p_agent != NULL) {
            ingame.MyGroup = p_agent->U.UPerson.EffectiveGroup;
        } else {
            ingame.MyGroup = 0;
        }
    }

    switch (playable_agents)
    {
    case 1:
        p_locplayer->DoubleMode = 0;
        break;
    case 2:
        if (p_locplayer->DoubleMode > 1)
            p_locplayer->DoubleMode = 1;
        break;
    case 3:
        if (p_locplayer->DoubleMode > 2)
              p_locplayer->DoubleMode = 2;
        break;
    default:
        break;
    }

    for (i = 0; i < playable_agents; i++)
    {
        struct Thing *p_agent;
        ulong wep;
        p_agent = p_locplayer->MyAgent[i];
        if (p_agent != NULL)
            wep = find_nth_weapon_held(p_agent->ThingOffset, 1);
        else
            wep = 0;
        p_locplayer->PrevWeapon[i] = wep;
    }

    init_game_controls();

    {
        struct Mission *p_missi;
        p_missi = &mission_list[ingame.CurrentMission];
        LOGSYNC("PreProcess %d turns for mission %d",
          (int)p_missi->PreProcess, (int)ingame.CurrentMission);
        blind_progress_game(p_missi->PreProcess);
    }
}

struct Thing *new_sim_person(int x, int y, int z, ubyte subtype)
{
    struct Thing *p_person;
    asm volatile ("call ASM_new_sim_person\n"
        : "=r" (p_person) : "a" (x), "d" (y), "b" (z), "c" (subtype));
    return p_person;
}


ushort make_group_into_players(ushort group, ushort plyr, ushort max_agent, short new_type)
{
#if 0
    ushort ret;
    asm volatile ("call ASM_make_group_into_players\n"
        : "=r" (ret) : "a" (group), "d" (plyr), "b" (max_agent), "c" (new_type));
    return ret;
#endif
    ulong n, nframe;
    ushort nagents, high_tier;
    PlayerInfo *p_player;
    struct Thing *p_person;

    p_player = &players[plyr];
    p_person = NULL;
    nagents = 0;
    high_tier = 0;
    for (n = things_used_head; n != 0; n = p_person->LinkChild)
    {
        p_person = &things[n];
        if ((p_person->U.UPerson.Group != group) || (p_person->Type != TT_PERSON))
            continue;

        if (nagents > p_player->DoubleMode)
        {
            if (in_network_game && p_player->DoubleMode) {
                p_person->State = PerSt_DEAD;
                p_person->Flag |= TngF_Unkn02000000 | TngF_Unkn0002;
            }
            p_player->DirectControl[nagents] = 0;
        }
        else
        {
            p_player->DirectControl[nagents] = p_person->ThingOffset;
            p_person->Flag |= TngF_Unkn1000;
            if ((plyr == local_player_no) && (nagents == 0)) {
                ingame.TrackX = p_person->X >> 8;
                ingame.TrackZ = p_person->Z >> 8;
            }
        }
        players[plyr].MyAgent[nagents] = p_person;
        p_person->Flag |= TngF_PlayerAgent;
#if 0 // This no longer makes sense - campaign is given with mission number
        if (!cmdln_param_bcg)
        {
            if (p_person->SubType == SubTT_PERS_ZEALOT)
                background_type = 1;
        }
#endif
        if (in_network_game)
            set_person_stats_type(p_person, 1);

        if (ingame.GameMode != GamM_Unkn2)
        {
            if ((p_person->SubType == SubTT_PERS_AGENT) || (p_person->SubType == SubTT_PERS_ZEALOT))
            {
                p_person->U.UPerson.WeaponsCarried = p_player->Weapons[high_tier] | 0x400000;
                p_person->U.UPerson.UMod.Mods = p_player->Mods[high_tier].Mods;
            }
            p_person->U.UPerson.CurrentWeapon = 0;
        }

        if (game_commands[p_person->U.UPerson.ComHead].Type == PCmd_HARD_AS_AGENT)
        {
            set_person_stats_type(p_person, 1);
            p_person->U.UPerson.ComHead = game_commands[p_person->U.UPerson.ComHead].Next;
        }

        if (game_commands[p_person->U.UPerson.ComHead].Type == PCmd_FULL_STAMINA)
        {
            p_person->U.UPerson.Stamina = peep_type_stats[1].MaximumStamina;
            p_person->U.UPerson.MaxStamina = peep_type_stats[1].MaximumStamina;
            p_person->U.UPerson.ComCur = p_person->U.UPerson.ComHead;
            p_person->Flag |= TngF_Unkn0040;
        }

        if ((p_person->U.UPerson.ComHead != 0) &&
            (game_commands[p_person->U.UPerson.ComHead].Type == PCmd_EXECUTE_COMS))
        {
            p_person->Flag2 |= 0x0800;
            p_person->U.UPerson.ComCur = 4 * plyr + nagents;
            if (ingame.GameMode == GamM_Unkn3)
                do_weapon_quantities_proper1(p_person);
            else
                do_weapon_quantities1(p_person);
            p_person->Flag |= TngF_Unkn0040;
            p_person->U.UPerson.ComCur = p_person->U.UPerson.ComHead;
            ingame.Flags |= GamF_Unkn0100;
        }
        else
        {
            p_person->U.UPerson.ComCur = 4 * plyr + nagents;
            p_person->U.UPerson.ComHead = 0;
            if (ingame.GameMode == GamM_Unkn3)
                do_weapon_quantities_proper1(p_person);
            else
                do_weapon_quantities1(p_person);
        }
        netgame_agent_pos_x[plyr][nagents] = p_person->X >> 8;
        netgame_agent_pos_y[plyr][nagents] = p_person->Z >> 8;
        p_person->State = PerSt_NONE;
        { // Why are we tripling the health?
            uint health;
            health = 3 * p_person->Health;
            if (health > PERSON_MAX_HEALTH_LIMIT)
                health = PERSON_MAX_HEALTH_LIMIT;
            p_person->Health = health;
            p_person->U.UPerson.MaxHealth = health;
        }

        switch (new_type)
        {
        case 0:
            p_person->SubType = SubTT_PERS_AGENT;
            nframe = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode];
            p_person->StartFrame = nframe - 1;
            p_person->Frame = nstart_ani[nframe + p_person->U.UPerson.Angle];
            break;
        case 1:
            p_person->SubType = SubTT_PERS_ZEALOT;
            nframe = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode];
            p_person->StartFrame = nframe - 1;
            p_person->Frame = nstart_ani[nframe + p_person->U.UPerson.Angle];
            break;
        case 2:
            p_person->SubType = SubTT_PERS_PUNK_M;
            nframe = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode];
            p_person->StartFrame = nframe - 1;
            p_person->Frame = nstart_ani[nframe + p_person->U.UPerson.Angle];
            break;
        }
        p_person->U.UPerson.FrameId.Version[0] = 0;
        if (p_person->U.UPerson.CurrentWeapon == 0)
        {
            nframe = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode];
            p_person->Frame -= nstart_ani[nframe + p_person->U.UPerson.Angle];
            p_person->U.UPerson.AnimMode = 0;
            nframe = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode];
            p_person->Frame += nstart_ani[nframe + p_person->U.UPerson.Angle];
        }

        if ((p_person->SubType == SubTT_PERS_AGENT) || (p_person->SubType == SubTT_PERS_ZEALOT))
            high_tier++;

        if (++nagents == max_agent)
            break;
    }
    return nagents;
}

int place_default_player(ushort player_id, TbBool replace)
{
    int ret;
    asm volatile ("call ASM_place_default_player\n"
        : "=r" (ret) : "a" (player_id), "d" (replace));
    return ret;
}

void place_single_player(void)
{
#if 0
    asm volatile ("call ASM_place_single_player\n"
        :  :  : "eax" );
#endif
    PlayerInfo *p_locplayer;
    ulong n;
    ushort nagents, pl_agents, pl_group;

    p_locplayer = &players[local_player_no];

    // Figure out how many agents are attending the mission
    if (p_locplayer->DoubleMode)
        nagents = p_locplayer->DoubleMode + 1;
    else
        nagents = 4;
    if (p_locplayer->MissionAgents < (1 << nagents) - 1)
    {
        switch (p_locplayer->MissionAgents)
        {
        case 1:
            nagents = 1;
            break;
        case 3:
            nagents = 2;
            break;
        case 7:
            nagents = 3;
            break;
        }
    }

    pl_group = level_def.PlayableGroups[0];
    pl_agents = make_group_into_players(pl_group, local_player_no, nagents, -1);
    if (pl_agents == 0) {
        struct Thing *p_person;
        LOGERR("Player %d group %d playable agents not found amongst %d things",
          (int)local_player_no, (int)pl_group, (int)things_used_head);
        p_person = new_sim_person(513, 1, 513, SubTT_PERS_AGENT);
        p_person->U.UPerson.Group = pl_group;
        p_person->U.UPerson.EffectiveGroup = pl_group;
        pl_agents = make_group_into_players(pl_group, local_player_no, 1, -1);
    } else {
        LOGSYNC("Player %d group %d playable agents found %d expected %d",
          (int)local_player_no, (int)pl_group, (int)pl_agents, (int)nagents);
    }

    n = things_used_head;
    while (n != 0)
    {
        struct Thing *p_thing;

        p_thing = &things[n];
        n = p_thing->LinkChild;
        if ((p_thing->U.UPerson.Group == pl_group) && (p_thing->Type == TT_PERSON) && !(p_thing->Flag & TngF_PlayerAgent))
        {
            remove_thing(p_thing->ThingOffset);
            delete_node(p_thing);
        }
    }
    playable_agents = pl_agents;
    if (pl_agents == 0)
      place_default_player(0, 1);
}

void init_game(ubyte reload)
{
    ushort missi, next_mapno;
    short next_level;

    missi = ingame.CurrentMission;
    next_mapno = mission_list[missi].MapNo;
    LOGSYNC("Init mission %hu on map %hu", missi, next_mapno);
    if (current_map != next_mapno)
        change_current_map(next_mapno);
    debug_trace_setup(0);

    if ((reload) && (mission_list[missi].ReLevelNo != 0)) {
        next_level = mission_list[missi].ReLevelNo;
    } else {
        next_level = mission_list[missi].LevelNo;
    }

    load_level_pc(-next_level, missi, reload);
    if (ingame.GameMode == GamM_None)
        ingame.GameMode = GamM_Unkn2;
    debug_trace_setup(1);
    init_player();
    debug_trace_setup(2);
    execute_commands = 1;
    ingame.DisplayMode = DpM_UNKN_32;
    debug_trace_setup(3);
}

void unkn_lights_func_11(void)
{
    asm volatile ("call ASM_unkn_lights_func_11\n"
        :  :  : "eax" );
}

void prep_single_mission(void)
{
    load_missions(background_type);
    load_objectives_text();
    init_game(0);
    load_multicolor_sprites();
    adjust_mission_engine_to_video_mode();
}

void restart_back_into_mission(ushort missi)
{
    ushort mapno;

    mapno = mission_list[missi].MapNo;
    mission_result = 0;
    ingame.CurrentMission = missi;
    mission_list[missi].Complete = 0;
    change_current_map(mapno);
    unkn_lights_func_11();
    if (ingame.GameMode == GamM_Unkn2)
        execute_commands = 0;
    engn_yc = 0;
    init_game(1);
    lbSeed = 0xD15C1234;
    if (pktrec_mode == PktR_RECORD)
    {
        PacketRecord_Close();
        PacketRecord_OpenWrite();
    }
}

void compound_mission_immediate_start_next(void)
{
    short i;
    ushort missi;

    show_black_screen();
    LbFileLoadAt("qdata/pal.pal", display_palette);
    LbPaletteSet(display_palette);

    i = find_mission_state_slot(ingame.CurrentMission);

    missi = ingame.CurrentMission;
    missi = mission_list[missi].SuccessTrigger[0];

    brief_store[open_brief - 1].Mission = missi;
    mission_open[i] = missi;
    mission_state[i] = 0;

    // TODO MISSI specific missions hard-coded - remove
    if (ingame.CurrentMission == 84)
    {
        ushort bkpmode;

        bkpmode = lbDisplay.ScreenMode;
        play_smacker(MPly_MPartComplete);
        LbFileLoadAt("qdata/pal.pal", display_palette);
        setup_screen_mode(bkpmode);
    }

    restart_back_into_mission(missi);
}

// deprecated - use compound_mission_immediate_start_next()
void tweak_for_compound_mission_m84(void)
{
#if 0
    asm volatile ("call ASM_tweak_for_compound_mission_m84\n"
        :  :  : "eax" );
#endif
    compound_mission_immediate_start_next();
}

short test_missions(ubyte flag)
{
    short ret;
    asm volatile ("call ASM_test_missions\n"
        : "=r" (ret) : "a" (flag));
    return ret;
}

void create_tables_file_from_fade(void)
{
    long len;
    int i, k;
    unsigned char *curbuf;
    // Note that the input file is not normally available with the game
    len = LbFileLoadAt("data/fade.dat", fade_data);
    if (len == Lb_FAIL) {
        LOGERR("Cannot find input file for tables update");
        return;
    }
    curbuf = fade_data;
    for (i = 0; i < 256; i++) {
        for (k = 0; k < 64; k++) {
          pixmap.fade_table[256 * k + i] = *(curbuf + (256+64) * k + i);
        }
    }
    fade_data = curbuf;
    LbFileSaveAt("data/tables.dat", &pixmap, sizeof(pixmap));
}

/** Universal way to generate the colour tables.
 */
void create_tables_file_from_palette(void)
{
    LbColourTablesGenerate(display_palette, fade_unaffected_colours,
      "data/tables.dat");
}

void game_setup(void)
{
    engine_mem_alloc_ptr = LbMemoryAlloc(engine_mem_alloc_size);
    load_texturemaps();
    LbDataLoadAll(unk02_load_files);
    read_weapons_conf_file();
    read_cybmods_conf_file();
    bang_init();
    init_arrays_1();
    init_search_spiral();
    bang_set_detail(0);
    game_setup_sub3();
    ingame.draw_unknprop_01 = 0;
    debug_trace_setup(-5);
    game_setup_stuff();
    create_strings_list(gui_strings, gui_strings_data, gui_strings_data_end);
    setup_host();
    debug_trace_setup(-4);
    read_user_settings();
    debug_trace_setup(-3);
    setup_color_lookups();
    init_things();
    debug_trace_setup(-2);
    LbSpriteSetup(small_font, small_font_end, small_font_data);
    load_peep_type_stats();
    load_campaigns();
    players[local_player_no].MissionAgents = 0x0F;
    debug_trace_setup(-1);
    if ( is_single_game || cmdln_param_bcg )
    {
        load_prim_quad();
    }
    debug_trace_setup(0);
    init_engine();
    if ( !cmdln_param_bcg )
    {
        screen_buffer_fill_black();
        swap_wscreen();
        LbPaletteSet(display_palette);
    }
    test_open(15);
    debug_trace_setup(1);
    if (is_single_game && (ingame.CurrentMission != 0))
    {
        prep_single_mission();
    }
    if (in_network_game || cmdln_param_bcg)
      ingame.DisplayMode = DpM_UNKN_37;
    debug_trace_setup(2);
    switch (cmdln_colour_tables)
    {
    case 1:
        create_tables_file_from_palette();
        break;
    case 2:
        create_tables_file_from_fade();
        break;
    }
}

void game_process_sub01(void)
{
    unsigned long tick_time = clock();
    tick_time = tick_time / 100;
    curr_tick_time = tick_time;
    if (tick_time != prev_tick_time)
    {
        unsigned long tmp;
        tmp = gameturn - prev_gameturn;
        prev_gameturn = gameturn;
        turns_delta = tmp;
    }
    if ( turns_delta != 0 ) {
        fifties_per_gameturn = 800 / turns_delta;
    } else {
        fifties_per_gameturn = 50;
    }
    if ( in_network_game )
        fifties_per_gameturn = 80;
    if ( fifties_per_gameturn > 400 )
        fifties_per_gameturn = 400;
    prev_tick_time = curr_tick_time;
}

void game_process_sub08(void);

void game_process_sub09(void)
{
    int i;
    switch ( gamep_unknval_01 )
    {
    case 1:
        game_process_sub08();
        break;
    case 2:
        for (i = 0; i < 10; i++) {
            ushort pos;
            ubyte *ptr;
            pos = LbRandomAnyShort() + (gameturn >> 2);
            ptr = &vec_tmap[pos];
            *ptr = pixmap.fade_table[40*PALETTE_8b_COLORS + *ptr];
        }
        break;
    }
}

void load_multicolor_sprites(void)
{
    ulong sz;
    char fname[100];

    sprintf(fname, "data/mspr-%d.dat", ingame.TrenchcoatPreference);
    LbFileLoadAt(fname, m_spr_data);
    sprintf(fname, "data/mspr-%d.tab", ingame.TrenchcoatPreference);
    sz = LbFileLoadAt(fname, m_sprites);
    m_sprites_end = (struct TbSprite *)((ubyte *)m_sprites + sz);
    LbSpriteSetup(m_sprites, m_sprites_end, m_spr_data);
    //unknown_unused(); -- nop function, not sure what was its purpose
}

int func_6edb8(ubyte a1)
{
    int ret;
    asm volatile ("call ASM_func_6edb8\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}

void debug_m_sprite(int idx)
{
    int i;
    char strdata[100];
    char *str;
    struct TbSprite *spr;
    unsigned char *ptr;
    spr = &m_sprites[idx];
    str = strdata;
    sprintf(str, "spr %d width %d height %d ptr 0x%lx data",
      idx, (int)spr->SWidth, (int)spr->SHeight, (ulong)spr->Data);
    ptr = spr->Data;
    for (i = 0; i < 10; i++)
    {
        str = strdata + strlen(strdata);
        sprintf(str, " %02x", (int)*ptr);
        ptr++;
    }
    LOGDBG("m_sprites: %s", strdata);
}

void mapwho_unkn01(int a1, int a2)
{
    asm volatile ("call ASM_mapwho_unkn01\n"
        : : "a" (a1), "d" (a2));
}

void new_bang(int a1, int a2, int a3, int a4, int a5, int a6)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_new_bang\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5), "g" (a6));
}

void new_bang_3(int a1, int a2, int a3, int a4)
{
    new_bang(a1, a2, a3, a4, 0, 0);
}

void input(void)
{
    uint16_t n;
    n = lbShift;
    if ( lbKeyOn[KC_LSHIFT] || lbKeyOn[KC_RSHIFT] )
        n |= KMod_SHIFT;
    else
        n &= ~KMod_SHIFT;
    if ( lbKeyOn[KC_LCONTROL] || lbKeyOn[KC_RCONTROL] )
        n |= KMod_CONTROL;
    else
        n &= ~KMod_CONTROL;
    if ( lbKeyOn[KC_RALT] || lbKeyOn[KC_LALT] )
        n |= KMod_ALT;
    else
        n &= ~KMod_ALT;
    lbShift = n;
}

void gproc3_unknsub3(int a1)
{
    // Empty
}

void draw_line_purple_list(int x1, int y1, int x2, int y2, int colour)
{
    asm volatile (
      "push %4\n"
      "call ASM_draw_line_purple_list\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (colour));
}

void draw_box_purple_list(int x, int y, ulong width, ulong height, int colour)
{
    asm volatile (
      "push %4\n"
      "call ASM_draw_box_purple_list\n"
        : : "a" (x), "d" (y), "b" (width), "c" (height), "g" (colour));
}

void draw_text_purple_list2(int x, int y, const char *text, ushort line)
{
    asm volatile (
      "call ASM_draw_text_purple_list2\n"
        : : "a" (x), "d" (y), "b" (text), "c" (line));
}

void draw_sprite_purple_list(int x, int y, struct TbSprite *sprite)
{
    asm volatile (
      "call ASM_draw_sprite_purple_list\n"
        : : "a" (x), "d" (y), "b" (sprite));
}

void copy_box_purple_list(long x, long y, ulong width, ulong height)
{
    asm volatile (
      "call ASM_copy_box_purple_list\n"
        : : "a" (x), "d" (y), "b" (width), "c" (height));
}

void ASM_show_game_engine(void);
void show_game_engine(void)
{
#if 0
    ASM_show_game_engine();
#endif
    int zoom;
    int dctrl_thing;

    dctrl_thing = players[local_player_no].DirectControl[0];
    process_view_inputs(dctrl_thing);// inlined call gengine_ctrl

    zoom = (900 - overall_scale) >> 1;
    if (zoom < 50)
        ingame.Scanner.Zoom = 50;
    else if (zoom > 556)
        ingame.Scanner.Zoom = 556;
    else
        ingame.Scanner.Zoom = zoom;
    process_engine_unk1();
    process_engine_unk2();
    process_engine_unk3();
    setup_engine_nullsub4();
}

void ASM_gproc3_unknsub2(void);
void gproc3_unknsub2(void)
{
    ASM_gproc3_unknsub2();
}

void ASM_BAT_play(void);
void BAT_play(void)
{
    ASM_BAT_play();
}

TbResult init_read_all_sprite_files(void)
{
    TbResult ret;
    asm volatile ("call ASM_init_read_all_sprite_files\n"
        : "=r" (ret) : );
    return ret;
}

#if 0
TbPixel LbPaletteFindColour(ubyte *pal, ubyte rval, ubyte gval, ubyte bval)
{
    TbResult ret;
    asm volatile ("call ASM_LbPaletteFindColour\n"
        : "=r" (ret) : "a" (pal), "d" (rval), "b" (gval), "c" (bval));
    return ret;
}
#endif

ubyte show_title_box(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_title_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

void check_buy_sell_button(void)
{
    asm volatile ("call ASM_check_buy_sell_button\n"
        :  :  : "eax" );
}

ubyte select_all_agents(ubyte click)
{
    selected_agent = 4;
    check_buy_sell_button();
    return 1;
}

ubyte change_panel_permutation(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_change_panel_permutation\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte change_trenchcoat_preference(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_change_trenchcoat_preference\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte alert_OK(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_alert_OK\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte accept_mission(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_accept_mission\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_unkn1_CANCEL(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_unkn1_CANCEL\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte do_sysmnu_button(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_sysmnu_button\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

void save_crypto_make_hashtable(ubyte simple_salt)
{
    asm volatile ("call ASM_save_crypto_make_hashtable\n"
        :  : "a" (simple_salt));
}

ulong save_crypto_transform1(ubyte a1)
{
    ulong ret;
    asm volatile ("call ASM_save_crypto_transform1\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}

ulong save_crypto_transform2(ubyte a1)
{
    ulong ret;
    asm volatile ("call ASM_save_crypto_transform2\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}

ulong save_crypto_transform3(ubyte a1)
{
    ulong ret;
    asm volatile ("call ASM_save_crypto_transform3\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}

ubyte load_game(int slot, char *desc)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_load_game\n"
        : "=r" (ret) : "a" (slot), "d" (desc));
    return ret;
#else
    char str[52];
    ulong gblen, fmtver, decrypt_verify;
    TbFileHandle fh;

    get_saved_game_fname(str, slot);

    fh = LbFileOpen(str, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE)
        return 1;
    LbFileRead(fh, desc, 25);
    LbFileRead(fh, &gblen, 4);
    LbFileRead(fh, &fmtver, 4);
    LbFileRead(fh, save_game_buffer, gblen);
    LbFileRead(fh, &decrypt_verify, 4);
    LbFileClose(fh);

    if (fmtver > 8)
    {
        // Decrypt the file
        ubyte *gbpos;
        ubyte *gbend;
        save_crypto_make_hashtable(slot);
        gbpos = save_game_buffer;
        gbend = &save_game_buffer[4 * (gblen >> 2)];
        while (gbpos < gbend)
        {
            ulong key, keysel;
            ulong *cryptpos;
            keysel = (save_crypto_data_state[1] << 8) | (save_crypto_data_state[0] << 16) | save_crypto_data_state[2];
            cryptpos = (ulong *)gbpos;
            switch (keysel)
            {
            case 0x00001:
            case 0x10100:
                key = save_crypto_transform1(1);
                key ^= save_crypto_transform2(1);
                key ^= save_crypto_transform3(0);
                break;
            case 0x00100:
            case 0x10001:
                key = save_crypto_transform1(1);
                key ^= save_crypto_transform2(0);
                key ^= save_crypto_transform3(1);
                break;
            case 0x00000:
            case 0x10101:
                key = save_crypto_transform1(1);
                key ^= save_crypto_transform2(1);
                key ^= save_crypto_transform3(1);
                break;
            case 0x00101:
            case 0x10000:
                key = save_crypto_transform1(0);
                key ^= save_crypto_transform2(1);
                key ^= save_crypto_transform3(1);
                break;
            default:
                key = 0;
                break;
            }
            gbpos += 4;
            *cryptpos ^= key;
        }
    }

    { // Verify data
        ulong *cpos;
        ulong clen, hash;
        ulong i;

        if (gblen & 3)
            clen = gblen + 4;
        else
            clen = gblen;
        clen >>= 2;
        cpos = (ulong *)save_game_buffer;

        hash = *cpos;
        for (i = 1; i < clen; i++) {
            cpos++;
            hash ^= *cpos;
        }
        if (hash != decrypt_verify)
            return 2;
    }

    memcpy(&ingame.Credits, &save_game_buffer[0], sizeof(ingame.Credits));

    gblen = 4;
    if (fmtver <= 4)
    {
        ushort cryo_no;
        memcpy(&cryo_agents, &save_game_buffer[gblen], offsetof(struct AgentInfo, FourPacks));
        gblen += offsetof(struct AgentInfo, FourPacks);
        memcpy(&cryo_agents.NumAgents, &save_game_buffer[gblen], 1);
        gblen += 1;
        for (cryo_no = 0; cryo_no < 32; cryo_no++)
        {
            // Remove bad mod flags
            cybmod_fix_all(&cryo_agents.Mods[cryo_no]);
            // Check weapons count, reset bad amounts of consumable weapons
            sanitize_weapon_quantities(&cryo_agents.Weapons[cryo_no],
              &cryo_agents.FourPacks[cryo_no]);
        }
    }
    else
    {
        ushort cryo_no;
        memcpy(&cryo_agents, &save_game_buffer[gblen], offsetof(struct AgentInfo, NumAgents));
        gblen += offsetof(struct AgentInfo, NumAgents);
        memcpy(&cryo_agents.NumAgents, &save_game_buffer[gblen], sizeof(cryo_agents.NumAgents));
        gblen += sizeof(cryo_agents.NumAgents);
        for (cryo_no = 0; cryo_no < 32; cryo_no++)
        {
            // Remove bad mod flags
            cybmod_fix_all(&cryo_agents.Mods[cryo_no]);
            // Check weapons count, reset bad amounts of consumable weapons
            sanitize_weapon_quantities(&cryo_agents.Weapons[cryo_no],
              &cryo_agents.FourPacks[cryo_no]);
        }
    }

    if (fmtver <= 2)
    {
        int i, k;
        // Old version has one byte progress
        for (i = 0; i < 32; i++)
        {
            for (k = 0; k < 10; k++)
            {
                research.WeaponProgress[i][k] = save_game_buffer[gblen];
                gblen++;
            }
        }
        for (i = 0; i < 32; i++)
        {
            for (k = 0; k < 10; k++)
            {
                research.ModProgress[i][k] = save_game_buffer[gblen];
                gblen++;
            }
        }
        i = sizeof(struct ResearchInfo) - offsetof(struct ResearchInfo, WeaponDaysDone);
        assert(i == 92);
        memcpy(research.WeaponDaysDone, &save_game_buffer[gblen], i);
        gblen += 732; // and not 92? we don't have old saves, so cannot verify.
    }
    else
    {
        assert(sizeof(struct ResearchInfo) == 1372);
        memcpy(&research, &save_game_buffer[gblen], sizeof(struct ResearchInfo));
        gblen += sizeof(struct ResearchInfo);
    }

    if (fmtver <= 3)
    {
        PlayerInfo *p_locplayer;
        int agent, i;
        p_locplayer = &players[local_player_no];

        i = sizeof(PlayerInfo) - offsetof(PlayerInfo, FourPacks);
        assert(i == 262);
        memcpy(p_locplayer, &save_game_buffer[gblen], i);
        gblen += i;
        // The old struct matches until FourPacks
        i = 8;
        memcpy(p_locplayer->FourPacks, &save_game_buffer[gblen], i);
        gblen += i;

        for (i = 0; i < 4; i++)
        {
            // Remove bad mod flags
            cybmod_fix_all(&p_locplayer->Mods[i]);
            // Reset bad amounts of consumable weapons
            if (weapons_has_weapon(p_locplayer->Weapons[i], WEP_NUCLGREN))
                p_locplayer->FourPacks[WFRPK_NUCLGREN][i] = 1;
            if (weapons_has_weapon(p_locplayer->Weapons[i], WEP_ELEMINE))
                p_locplayer->FourPacks[WFRPK_ELEMINE][i] = 1;
            if (weapons_has_weapon(p_locplayer->Weapons[i], WEP_EXPLMINE))
                p_locplayer->FourPacks[WFRPK_EXPLMINE][i] = 1;
            if (weapons_has_weapon(p_locplayer->Weapons[i], WEP_KOGAS))
                p_locplayer->FourPacks[WFRPK_KOGAS][i] = 1;
            if (weapons_has_weapon(p_locplayer->Weapons[i], WEP_CRAZYGAS))
                p_locplayer->FourPacks[WFRPK_CRAZYGAS][i] = 1;

            p_locplayer->field_19A[i] = 0;
            p_locplayer->field_1A2[i] = 0;
        }

        for (agent = 0; agent < 32; agent++)
        {
            for (i = 0; i < 4; i++) {
                p_locplayer->WepDelays[i][agent] = 0;
            }
        }
    }
    else if (fmtver <= 6)
    {
        PlayerInfo *p_locplayer;
        int i;
        p_locplayer = &players[local_player_no];

        i = sizeof(PlayerInfo) - offsetof(PlayerInfo, WepDelays);
        assert(i == 282);
        memcpy(p_locplayer, &save_game_buffer[gblen], i);
        gblen += i;
        // The old struct matches until WepDelays
        i = 48;
        memcpy(p_locplayer->WepDelays, &save_game_buffer[gblen], i);
        gblen += i;
        // Remove bad mod flags
        for (i = 0; i < 4; i++)
        {
            cybmod_fix_all(&p_locplayer->Mods[i]);
        }
    }
    else
    {
        PlayerInfo *p_locplayer;
        int i;
        assert(sizeof(PlayerInfo) == 426);
        p_locplayer = &players[local_player_no];
        memcpy(p_locplayer, &save_game_buffer[gblen], sizeof(PlayerInfo));
        gblen += sizeof(PlayerInfo);
        // Remove bad mod flags
        for (i = 0; i < 4; i++)
        {
            cybmod_fix_all(&p_locplayer->Mods[i]);
        }
    }
    players_sync_from_cryo();

    memcpy(&global_date, &save_game_buffer[gblen], sizeof(struct SynTime));
    gblen += sizeof(struct SynTime);
    memcpy(&research_curr_wep_date, &save_game_buffer[gblen], sizeof(struct SynTime));
    gblen += sizeof(struct SynTime);
    memcpy(&research_curr_mod_date, &save_game_buffer[gblen], sizeof(struct SynTime));
    gblen += sizeof(struct SynTime);

    next_email = save_game_buffer[gblen + 0];
    next_brief = save_game_buffer[gblen + 2];
    old_mission_brief = save_game_buffer[gblen + 4];
    open_brief = save_game_buffer[gblen + 6];
    next_ref = save_game_buffer[gblen + 8];
    new_mail = save_game_buffer[gblen + 10];
    background_type = save_game_buffer[gblen + 11];
    gblen += 12;

    load_missions(background_type);

    if (fmtver <= 9)
    {
        // Mission status block did not existed in this version
    }
    else if (fmtver <= 11)
    {
        int i;
        i = sizeof(struct MissionStatus) - offsetof(struct MissionStatus, Expenditure);
        assert(i == 32);
        memcpy(&mission_status[open_brief], &save_game_buffer[gblen], i);
        gblen += i;
        gblen += 2;
        mission_status[open_brief].AgentsLost = save_game_buffer[gblen];
        gblen++;
        mission_status[open_brief].AgentsGained = save_game_buffer[gblen];
        gblen++;
    }
    else
    {
        int i;
        i = sizeof(struct MissionStatus);
        assert(i == 40);
        memcpy(&mission_status[open_brief], &save_game_buffer[gblen], i);
        gblen += i;
    }

    memcpy(email_store, &save_game_buffer[gblen], 5 * next_email);
    gblen += 5 * next_email;
    memcpy(brief_store, &save_game_buffer[gblen], 5 * next_brief);
    gblen += 5 * next_brief;
    memcpy(newmail_store, &save_game_buffer[gblen], 5 * new_mail);
    gblen += 5 * new_mail;

    {
        int i;
        memcpy(&ingame.MissionStatus, &save_game_buffer[gblen], sizeof(ingame.MissionStatus));
        gblen += sizeof(ingame.MissionStatus);
        i = sizeof(mission_open);
        assert(i == 100);
        memcpy(mission_open, &save_game_buffer[gblen], i);
        gblen += i;
        i = sizeof(mission_state);
        assert(i == 100);
        memcpy(mission_state, &save_game_buffer[gblen], i);
        gblen += i;
    }

    if (fmtver < 8)
    {
        // Deprecated data
        gblen += 32;
    }

    if (fmtver <= 11)
    {
        next_mission = save_game_buffer[gblen];
        gblen += 2;
    } else
    {
        memcpy(&next_mission, &save_game_buffer[gblen], sizeof(next_mission));
        gblen += 2;
    }


    {
        int i;
        for (i = 1; i < next_mission; i++)
        {
            mission_list[i].SpecialTrigger[0] = save_game_buffer[gblen];
            gblen++;
            mission_list[i].SpecialTrigger[1] = save_game_buffer[gblen];
            gblen++;
            mission_list[i].SpecialTrigger[2] = save_game_buffer[gblen];
            gblen++;
            if (fmtver > 1)
            {
                mission_list[i].Complete = save_game_buffer[gblen];
                gblen++;
            }
        }
    }

    if (fmtver <= 5)
    {
        if (login_name[0] == '\0')
          strcpy(login_name, "ANON");
    }
    else
    {
        int i;
        i = 16;
        memcpy(login_name, &save_game_buffer[gblen], i);
        gblen += i;
    }

    if (fmtver > 10)
    {
        int i;
        for (i = 0; i < num_cities; i++)
        {
            cities[i].Info = save_game_buffer[gblen];
            gblen++;
            recount_city_credit_reward(i);
        }
        ingame.fld_unk7DE = save_game_buffer[gblen];
        gblen++;
    }

#if 0
    // TODO MISSI Special fix for specific mission; why is it there?
    if (mission_list[28].SpecialTrigger[1]) {
        mission_list[28].SpecialTrigger[0] = 0;
        mission_list[28].SpecialTrigger[1] = 0;
        mission_list[28].SpecialTrigger[2] = 7;
    }
#endif

    read_user_settings();
    login_control__Money = ingame.Credits;
    ingame.CashAtStart = ingame.Credits;
    ingame.Expenditure = 0;
    return 0;
#endif
}

/** Re-enables a button from system menu, moving the following buttons down.
 */
void sysmnu_button_enable(int btnno, int count)
{
    int i;
    for (i = btnno+count; i < 6; i++) {
        sysmnu_buttons[i].Y += 30 * count;
    }
}

/** Disables a button from system menu, moving the buttons below to fill its space.
 */
void sysmnu_button_disable(int btnno, int count)
{
    int i;
    for (i = btnno+count; i < 6; i++) {
        sysmnu_buttons[i].Y -= 30 * count;
    }
}

ubyte load_game_slot(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_load_game_slot\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    char *ldname;
    int ldslot;
    int ret;

    if (login_control__State != 6) {
        return 0;
    }
    if (save_slot == -1) {
        show_alert = 1;
        sprintf(alert_text, "%s", gui_strings[564]);
        return 1;
    }
    if (save_slot != 0) {
        ldname = save_slot_names[save_slot - save_slot_base - 1];
        ldslot = save_slot;
    } else {
        ldname = save_active_desc;
        ldslot = 0;
    }

    ret = load_game(ldslot, ldname);
    if (ret == 1) {
        show_alert = 1;
        sprintf(alert_text, "%s", gui_strings[565]);
        return 1;
    } else if (ret == 2) {
        show_alert = 1;
        sprintf(alert_text, "%s", gui_strings[567]);
        return 1;
    }

    show_alert = 1;
    sprintf(alert_text, "%s", gui_strings[572]);

    // Reading the save might have caused campaign switch
    reload_background_flag = 1;
    load_objectives_text();
    init_weapon_text();

    unkn13_SYSTEM_button.Flags &= ~(0x8000|0x2000|0x0004);
    clear_someflags_controls_screen_boxes();
    clear_someflags_storage_screen_boxes();
    if (save_slot == 0)
    {
        ingame.Flags |= 0x10;
        sysmnu_button_disable(1,2);
    }
    unkn_city_no = -1;
    selected_agent = 0;
    word_1C6E0A = 0;
    word_1C6E08 = 0;
    screentype = SCRT_99;
    game_system_screen = 0;
    if (restore_savegame) {
        restore_savegame = 0;
        sysmnu_button_enable(0, 5);
    }
    return 1;
}

ubyte save_game_slot(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_save_game_slot\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte main_do_my_quit(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_main_do_my_quit\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte main_do_login_1(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_main_do_login_1\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte goto_savegame(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_goto_savegame\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

void my_preprocess_text(char *text)
{
    asm volatile ("call ASM_my_preprocess_text\n"
        :  : "a" (text));
}

/** Initializes the research data for a new game.
 */
void srm_reset_research(void)
{
#if 0
    asm volatile ("call ASM_srm_reset_research\n"
        :  :  : "eax" );
#else
    struct Campaign *p_campgn;
    int i;

    p_campgn = &campaigns[background_type];

    for (i = 0; i < 32; i++)
    {
        research.WeaponDaysDone[i] = 0;
        research.ModDaysDone[i] = 0;
        research.WeaponProgress[i][0] = 0;
        research.ModProgress[i][0] = 0;
    }
    research.WeaponsCompleted = p_campgn->StandardWeapons;
    research.WeaponsAllowed = p_campgn->ResearchWeapons;
    research.ModsCompleted = p_campgn->StandardMods;
    research.ModsAllowed = p_campgn->ResearchMods;
    research.CurrentWeapon = -1;
    research.CurrentMod = -1;
    research.Scientists = 0;
    research.NumBases = 0;
#endif
}

void research_unkn_func_006(ushort missi)
{
#if 0
    asm volatile ("call ASM_research_unkn_func_006\n"
        : : "a" (missi));
#endif
    struct Mission *p_missi;
    int i;

    p_missi = &mission_list[missi];

    if (p_missi->ResearchWeapons != 0) {
        research_weapon_flags_allow(p_missi->ResearchWeapons);
    }

    switch (p_missi->ExtraRewardType)
    {
    case MEReward_ResearchLab:
        research.NumBases++;
        // Fall through
    case MEReward_Scientists:
        research.Scientists += p_missi->ExtraRewardParam;
        break;
    case MEReward_CybModResearched:
        research_cymod_complete(p_missi->ExtraRewardParam);
        break;
    case MEReward_CybModSingle:
        for (i = 0; i < cryo_agents.NumAgents; i++)
        {
            TbBool added;

            added = add_mod_to_flags_no_replace(&cryo_agents.Mods[i], p_missi->ExtraRewardParam);
            if (!added)
                continue;

            if (i < 4) {
                PlayerInfo *p_locplayer;
                p_locplayer = &players[local_player_no];
                add_mod_to_flags(&p_locplayer->Mods[i], p_missi->ExtraRewardParam);
            }
            break;
        }
        break;
    case MEReward_WeaponResearched:
        research_weapon_complete(p_missi->ExtraRewardParam);
        break;
    case MEReward_WeaponSingle:
        for (i = 0; i < cryo_agents.NumAgents; i++)
        {
            TbBool added;

            added = weapons_add_one(&cryo_agents.Weapons[i], &cryo_agents.FourPacks[i], p_missi->ExtraRewardParam);
            if (!added)
                continue;

            /* TODO re-ebable when player->FourPacks is unified and in the same format as cryo_agents.FourPacks
            if (i < 4) {
                PlayerInfo *p_locplayer;
                p_locplayer = &players[local_player_no];
                weapons_add_one(&p_locplayer->Weapons[i], &p_player->FourPacks[i], p_missi->ExtraRewardParam);
            }*/
            break;
        }
        break;
    }
}

void mission_over_update_players(void)
{
    PlayerInfo *p_locplayer;

    p_locplayer = &players[local_player_no];
    cryo_update_agents_from_player(p_locplayer);
}

ulong mission_over_calculate_cash_gain_from_persuaded_crowd(ushort tgroup)
{
    ulong credits;
    struct Thing *p_person;
    short person;

    credits = 0;
    person = get_thing_same_type_head(TT_PERSON, -1);
    for (; person > 0; person = p_person->LinkSame)
    {
        p_person = &things[person];
        if ((p_person->Flag & TngF_Persuaded) == 0)
            continue;
        if (p_person->U.UPerson.EffectiveGroup != tgroup)
            continue;
        switch (p_person->SubType)
        {
        case SubTT_PERS_AGENT:
            credits += 1000;
            break;
        case SubTT_PERS_ZEALOT:
            credits += 1000;
            break;
        case SubTT_PERS_PUNK_M:
        case SubTT_PERS_PUNK_F:
            credits += 150;
            break;
        case SubTT_PERS_SCIENTIST:
            credits += 500;
            break;
        default:
            credits += 100;
            break;
        }
    }
    return credits;
}

void mission_over_gain_personnel_from_persuaded_crowd(void)
{
    struct Thing *p_person;
    short person;

    person = get_thing_same_type_head(TT_PERSON, -1);
    for (; person > 0; person = p_person->LinkSame)
    {
        p_person = &things[person];
        if ((p_person->Flag & TngF_Persuaded) == 0)
            continue;
        if (p_person->U.UPerson.EffectiveGroup != ingame.MyGroup)
            continue;
        switch (p_person->SubType)
        {
        case SubTT_PERS_AGENT:
            add_agent(p_person->U.UPerson.WeaponsCarried, p_person->U.UPerson.UMod.Mods);
            break;
        case SubTT_PERS_SCIENTIST:
            research.Scientists++;
            break;
        }
    }
}

void mission_over_gain_persuaded_crowd_rewards(void)
{
    ingame.Credits += mission_over_calculate_cash_gain_from_persuaded_crowd(ingame.MyGroup);
    mission_over_gain_personnel_from_persuaded_crowd();
}

ulong mission_over_calculate_player_cash_gain_from_items(void)
{
    ulong credits;
    short sthing;
    struct SimpleThing *p_sthing;

    credits = 0;
    for (sthing = sthings_used_head; sthing != 0; sthing = p_sthing->LinkChild)
    {
        p_sthing = &sthings[sthing];
        if (p_sthing->Type == SmTT_CARRIED_ITEM)
        {
            struct Thing *p_owntng;
            p_owntng = &things[p_sthing->U.UWeapon.Owner];
            // Item carried by non-persuaded player agents
            if (((p_owntng->Flag & TngF_PlayerAgent) != 0) &&
              ((p_owntng->Flag & TngF_Persuaded) == 0))
            {
                if (p_sthing->U.UWeapon.WeaponType == 0)
                    credits += 100 * p_sthing->U.UWeapon.Ammo;
                LOGSYNC("Carried item thing %hd picked by the player", sthing);
            }
            // Item carried by people persuaded by the player
            else if (((p_owntng->Flag & TngF_Persuaded) != 0) &&
              (p_owntng->U.UPerson.EffectiveGroup == ingame.MyGroup))
            {
                if (p_sthing->U.UWeapon.WeaponType == 0)
                    credits += 100 * p_sthing->U.UWeapon.Ammo;
                LOGSYNC("Carried item thing %hd picked by persuadee", sthing);
            }
            else
            {
                LOGSYNC("Carried item thing %hd not under player control", sthing);
            }
        }
    }
    return credits;
}

void update_player_cash(void)
{
#if 0
    asm volatile ("call ASM_update_player_cash\n"
        :  :  : "eax" );
#endif
    ulong credits;

    credits = mission_over_calculate_player_cash_gain_from_items();
    ingame.Credits += credits;
}

void init_agents(void)
{
    asm volatile ("call ASM_init_agents\n"
        :  :  : "eax" );
}

void do_start_triggers(short missi)
{
#if 0
    asm volatile (
      "call ASM_do_start_triggers\n"
        : : "a" (missi));
#endif
    short nxmissi, sptrig;
    int mslot;

    mslot = find_empty_mission_state_slot();
    if (mslot < 1) {
        LOGERR("No free slot found for mission %d", (int)missi);
        return;
    }

    for (nxmissi = missi; mslot < 50; mslot++)
    {
        if (true)
            sptrig = mission_list[nxmissi].SpecialTrigger[0];
        if (sptrig == 0)
            sptrig = mission_list[nxmissi].SpecialTrigger[1];
        if (sptrig == 0)
            break;
        nxmissi = sptrig;
        mission_open[mslot] = nxmissi;
        mission_state[mslot] = 0;
    }
}

void queue_up_new_mail(ubyte emtype, short missi)
{
#if 0
    asm volatile (
      "call ASM_queue_up_new_mail\n"
        : : "a" (emtype), "d" (missi));
#endif
    int i;

    if ((emtype == 1) && (mission_list[missi].SourceID == 0))
        return;
    if (missi < 0) {
        missi = -missi;
        emtype = 0;
    }
    i = new_mail;
    newmail_store[i].Mission = missi;
    newmail_store[i].RecvDay = global_date.Day;
    newmail_store[i].RecvMonth = global_date.Month;
    newmail_store[i].RecvYear = global_date.Year;
    if (emtype != 1)
        newmail_store[i].Flag = 2;
    else
        newmail_store[i].Flag = 1;
    new_mail++;
}

ushort open_new_mission(short missi)
{
    int mslot;

    if (mission_has_immediate_previous(missi)) {
        LOGSYNC("No slot needed for mission %d", (int)missi);
        return 0;
    }

    mslot = find_empty_mission_state_slot();
    if (mslot > 0) {
        mission_open[mslot] = missi;
        mission_state[mslot] = 0;
    } else {
        LOGERR("No free slot found for mission %d", (int)missi);
    }
    do_start_triggers(missi);
    queue_up_new_mail(1, missi);

    return 0;
}

TbBool check_mission_conds(ushort missi)
{
    int i;
    ushort cmissi;

    cmissi = missi;
    for (i = 0; i < 5; i++)
    {
        cmissi = mission_list[cmissi].MissionCond[i];
        if ((cmissi > 0) && (mission_list[cmissi].Complete != 1))
          return false;
    }
    return true;
}

TbBool mission_has_no_special_triggers(ushort missi)
{
    struct Mission *p_missi;
    int i;

    p_missi = &mission_list[missi];

    for (i = 0; i < 3; i++)
    {
        if (p_missi->SpecialTrigger[i] != 0)
          return false;
    }
    return true;
}

TbBool mission_special_triggers_2_is_self(ushort missi)
{
    struct Mission *p_missi;

    p_missi = &mission_list[missi];

    return (p_missi->SpecialTrigger[2] == missi);
}

void mission_reset_spec_triggers_2_chain(ushort missi)
{
    ushort next_missi;

    next_missi = missi;
    while (next_missi != 0)
    {
        ushort tmp_missi;

        LOGSYNC("Setting mission=%d owner to mission=%d", (int)next_missi, (int)missi);
        mission_list[next_missi].SpecialTrigger[2] = missi;

        // TODO Why only one these? If replacing, shouldn't we replace both?
        tmp_missi = mission_list[next_missi].SpecialTrigger[0];
        if (tmp_missi == 0)
            tmp_missi = mission_list[next_missi].SpecialTrigger[1];
        next_missi = tmp_missi;
    }
}

void mission_special_triggers_0_1_set_fail(ushort missi)
{
    ushort next_missi;

    next_missi = missi;
    while (next_missi > 0)
    {
        ushort tmp_missi;

        mission_list[next_missi].Complete = -1;

        // TODO Why only one these? If failing, shouldn't we fail both?
        tmp_missi = mission_list[next_missi].SpecialTrigger[0];
        if (tmp_missi == 0)
            tmp_missi = mission_list[next_missi].SpecialTrigger[1];
        next_missi = tmp_missi;
    }
}

void mission_copy_conds_and_succ_fail_triggers(ushort dst_missi, ushort src_missi)
{
    int i;

    LOGSYNC("Copying mission=%d props to mission=%d", (int)src_missi, (int)dst_missi);
    for (i = 0; i < 5; i++) {
        mission_list[dst_missi].MissionCond[i] = mission_list[src_missi].MissionCond[i];
    }
    for (i = 0; i < 3; i++) {
        mission_list[dst_missi].SuccessTrigger[i] = mission_list[src_missi].SuccessTrigger[i];
        mission_list[dst_missi].FailTrigger[i] = mission_list[src_missi].FailTrigger[i];
    }
}

ushort mission_fire_success_triggers(ushort missi)
{
    struct Mission *p_missi;
    ushort n;
    int i;

    p_missi = &mission_list[missi];

    n = 0;
    for (i = 0; i < 3; i++)
    {
        ushort new_missi;
        new_missi = p_missi->SuccessTrigger[i];
        if (new_missi != 0) {
            open_new_mission(new_missi);
            n++;
        }
    }
    return n;
}

ushort mission_fire_fail_triggers(ushort missi)
{
    struct Mission *p_missi;
    ushort n;
    int i;

    p_missi = &mission_list[missi];

    n = 0;
    for (i = 0; i < 3; i++)
    {
        ushort new_missi;
        new_missi = p_missi->FailTrigger[i];
        if (new_missi != 0) {
            open_new_mission(new_missi);
            n++;
        }
    }
    return n;
}

void delete_open_mission(ushort mslot, sbyte state)
{
#if 0
    asm volatile ("call ASM_delete_open_mission\n"
        : : "a" (mslot), "d" (state));
#else
    ushort missi;
    TbBool conds_met;

    missi = mission_open[mslot];
    if (state == 1) {
        mission_list[missi].Complete = state;
    } else if (mission_remain_until_success(missi)) {
          mission_list[missi].Complete = 0;
          mission_state[mslot] = 0;
    } else {
          mission_list[missi].Complete = state;
    }

    conds_met = check_mission_conds(missi);

    research_unkn_func_006(missi);

    if (mission_has_no_special_triggers(missi))
    {
        LOGSYNC("SpecialTriggers none, mission=%d, state=%d triggers=%d",
          (int)missi, (int)state, (int)conds_met);
        if (state == 1)
        {
            if (conds_met)
            {
                mission_fire_success_triggers(missi);
            }
            if (mission_has_immediate_next_on_success(ingame.CurrentMission))
            {
                LOGSYNC("Immediate start next (ST no), mission=%d, current=%d",
                  (int)missi, (int)ingame.CurrentMission);
                compound_mission_immediate_start_next();
                return;
            }
            play_smacker(MPly_MissiComplete);
        }
        else if (state == -1)
        {
            if (conds_met)
            {
                mission_fire_fail_triggers(missi);
            }
        }
    }
    else if (mission_special_triggers_2_is_self(missi))
    {
        ushort trg_missi;

        trg_missi = mission_list[missi].SpecialTrigger[0];
        if (trg_missi == 0)
            trg_missi = mission_list[missi].SpecialTrigger[1];
        if (trg_missi != 0)
        {
            LOGSYNC("SpecialTriggers self-owned and set, mission=%d, state=%d triggers=%d",
              (int)missi, (int)state, (int)conds_met);
            if (state == 1)
            {
                brief_store[open_brief - 1].Mission = trg_missi;

                mission_copy_conds_and_succ_fail_triggers(trg_missi, missi);
                mission_reset_spec_triggers_2_chain(trg_missi);

                ingame.MissionStatus = 0;
            }
            else if (state == -1)
            {
                if (mission_remain_until_success(missi))
                {
                    ingame.MissionStatus = 0;
                }
                else
                {
                    if (conds_met)
                    {
                        mission_fire_fail_triggers(missi);
                    }

                    mission_special_triggers_0_1_set_fail(trg_missi);
                }
            }
        }
        else
        {
            LOGSYNC("SpecialTriggers self-owned but unset, mission=%d, state=%d triggers=%d",
              (int)missi, (int)state, (int)conds_met);
            if (state == 1)
            {
                if (conds_met)
                {
                    mission_fire_success_triggers(missi);
                }
                if (mission_has_immediate_next_on_success(ingame.CurrentMission))
                {
                    LOGSYNC("Immediate start next (ST so), mission=%d, current=%d",
                      (int)missi, (int)ingame.CurrentMission);
                    compound_mission_immediate_start_next();
                    return;
                }
                play_smacker(MPly_MissiComplete);
            }
            else if (state == -1)
            {
                if (mission_remain_until_success(missi))
                {
                    ingame.MissionStatus = 0;
                }
                else
                {
                    if (conds_met)
                    {
                        mission_fire_fail_triggers(missi);
                    }
                }
            }
        }
    }
    else
    {
        ushort trg_missi;

        trg_missi = mission_list[missi].SpecialTrigger[2];

        LOGSYNC("SpecialTriggers owner=%d, mission=%d, state=%d", (int)trg_missi, (int)missi, (int)state);
        if (state == 1)
        {
            ushort tmp_missi, tmp2_missi, next_missi;

            next_missi = trg_missi;
            while (1)
            {
                tmp_missi = mission_list[next_missi].SpecialTrigger[0];
                if ((tmp_missi == missi) || (next_missi == 0))
                    break;
                next_missi = tmp_missi;
            }
            if (next_missi > 0)
            {
                tmp_missi = mission_list[next_missi].SpecialTrigger[0];
                tmp2_missi = mission_list[tmp_missi].SpecialTrigger[0];
                if (tmp2_missi > 0)
                    mission_list[next_missi].SpecialTrigger[0] = tmp2_missi;
                else
                    mission_list[next_missi].SpecialTrigger[0] = mission_list[tmp_missi].SpecialTrigger[1];
            }
            ingame.MissionStatus = 0;
        }
        else if (state == -1)
        {
            if (mission_remain_until_success(missi))
            {
                ingame.MissionStatus = 0;
            }
            else
            {
                if (conds_met)
                {
                    mission_fire_fail_triggers(trg_missi);
                }
                mission_special_triggers_0_1_set_fail(trg_missi);
            }
        }
    }

    if (!mission_remain_until_success(missi) || (state == 1))
    {
        remove_mission_state_slot(mslot);
    }
#endif
}

void mission_over(void)
{
#if 0
    asm volatile ("call ASM_mission_over\n"
        :  :  : "eax" );
#else
    ingame.DisplayMode = DpM_UNKN_37;
    LbMouseChangeSprite(0);
    StopCD();
    StopAllSamples();
    SetMusicVolume(100, 0);

    update_player_cash();
    mission_over_update_players();
    mission_over_gain_persuaded_crowd_rewards();
    players_sync_from_cryo();

    ushort mslot;
    ushort last_missi;
    short lstate;

    last_missi = ingame.CurrentMission;
    mslot = find_mission_state_slot(last_missi);
    if (mission_state[mslot] == 0)
        mission_state[mslot] = ingame.MissionStatus;

    if (mission_state[mslot] == 1)
    {
        if (mission_is_final_at_game_end(last_missi))
            init_outro();
    }

    lstate = 0;
    if (mission_state[mslot] == 1)
    {
        long cr_award;
        short email;
        ushort missi;

        lstate = 1;
        missi = mission_open[mslot];
        cr_award = 1000 * mission_list[missi].CashReward;
        ingame.fld_unkC57++;
        email = mission_list[missi].SuccessID;
        ingame.Credits += cr_award;
        if (email != 0)
            queue_up_new_mail(0, -email);
        delete_open_mission(mslot, 1);
    }
    else if (mission_state[mslot] == -1)
    {
        short email;
        ushort missi;

        lstate = -1;
        missi = mission_open[mslot];
        email = mission_list[missi].FailID;
        ingame.fld_unkC57++;
        if (email != 0)
            queue_up_new_mail(0, -email);
        delete_open_mission(mslot, -1);
        play_smacker(MPly_MissiFail);
    }

    ingame.GameOver = 0;
    if (lstate == -1)
    {
      if (!mission_remain_until_success(ingame.CurrentMission))
        ingame.GameOver = 1;
    }
    if (new_mail)
        ingame.GameOver = 0;
    if (cryo_agents.NumAgents == 0)
        ingame.GameOver = 1;
    LOGSYNC("Mission %d ended with state=%d gameover=%d",
      (int)last_missi, (int)lstate, (int)ingame.GameOver);
    if (ingame.GameOver)
        play_smacker(MPly_GameOver);
#endif
}

TbBool player_try_spend_money(long cost)
{
    if (ingame.Credits - cost < 0)
        return false;
    ingame.Credits -= cost;
    ingame.Expenditure += cost;
    return true;
}

void init_variables(void)
{
    asm volatile ("call ASM_init_variables\n"
        :  :  : "eax" );
}

ubyte main_do_map_editor(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_main_do_map_editor\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

int save_game_write(ubyte slot, char *desc)
{
    int ret;
    asm volatile ("call ASM_save_game_write\n"
        : "=r" (ret) : "a" (slot), "d" (desc));
    return ret;
}

ubyte ac_select_all_agents(ubyte click);
ubyte ac_alert_OK(ubyte click);
ubyte ac_do_sysmnu_button(ubyte click);
ubyte ac_main_do_my_quit(ubyte click);
ubyte ac_main_do_login_1(ubyte click);
ubyte ac_goto_savegame(ubyte click);
ubyte ac_main_do_map_editor(ubyte click);

void campaign_new_game_prepare(void)
{
    struct Campaign *p_campgn;

    p_campgn = &campaigns[background_type];

    screentype = 99;
    game_system_screen = 0;
    players[local_player_no].MissionAgents = 0x0F;
    init_weapon_text();
    load_city_data(0);
    load_city_txt();
    init_variables();
    init_agents();
    load_missions(background_type);
    load_objectives_text();
    srm_reset_research();

    {
        open_new_mission(p_campgn->FirstTrigger);
        update_brief_screen_netscan_button(p_campgn->NetscanTextId);
    }

    reload_background_flag = 1;
    edit_flag = 0;
}

ubyte do_storage_NEW_MORTAL(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_storage_NEW_MORTAL\n"
        : "=r" (ret) : "a" (click));
    return ret;
#else
    if (login_control__State != 6)
        return 0;

    if (strlen(login_name) == 0)
        strcpy(login_name, "ANON");
    read_user_settings();

    ingame.Flags |= GamF_Unkn0010;

    campaign_new_game_prepare();

    if( save_game_write(0, save_active_desc)) {
        show_alert = 1;
        sprintf(alert_text, "%s", gui_strings[566]);
    }

    if (restore_savegame)  {
        restore_savegame = 0;
        sysmnu_button_enable(0, 5);
    }

    if (true) {
        sysmnu_button_disable(1,2);
    }

    if (new_mail)
      play_sample_using_heap(0, 119 + (LbRandomAnyShort() % 3), 127, 64, 100, 0, 3u);

    return 1;
#endif
}

ubyte show_netgame_unkn1(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_netgame_unkn1\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_settings_controls_list(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_settings_controls_list\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte ac_show_netgame_unkn1(struct ScreenBox *box);
ubyte ac_show_settings_controls_list(struct ScreenBox *box);

void init_main_screen_boxes(void)
{
    init_screen_button(&main_map_editor_button, 260, 387,
      gui_strings[443], 6, med2_font, 1, 0);
    init_screen_button(&main_load_button, 260, 358,
      gui_strings[496], 6, med2_font, 1, 0);
    init_screen_button(&main_quit_button, 260, 329,
      gui_strings[445], 6, med2_font, 1, 0);
    init_screen_button(&main_login_button, 260, 300,
      gui_strings[444], 6, med2_font, 1, 0);
    main_map_editor_button.X = 319 - (main_map_editor_button.Width >> 1);
    main_login_button.X = 319 - (main_login_button.Width >> 1);
    main_quit_button.X = 319 - (main_quit_button.Width >> 1);
    main_load_button.X = 319 - (main_load_button.Width >> 1);
    main_map_editor_button.Border = 3;
    main_login_button.Border = 3;
    main_quit_button.Border = 3;
    main_load_button.Border = 3;
    main_map_editor_button.CallBackFn = ac_main_do_map_editor;
    main_login_button.CallBackFn = ac_main_do_login_1;
    main_quit_button.CallBackFn = ac_main_do_my_quit;
    main_load_button.CallBackFn = ac_goto_savegame;
    main_login_button.AccelKey = 28;
    main_quit_button.AccelKey = 1;
}

void set_flag01_main_screen_boxes(void)
{
        main_quit_button.Flags |= 0x0001;
        main_login_button.Flags |= 0x0001;
        main_load_button.Flags |= 0x0001;
        main_map_editor_button.Flags |= 0x0001;
}

void init_alert_screen_boxes(void)
{
    int w;

    init_screen_text_box(&loading_INITIATING_box, 210u, 230u, 220u, 20,
      6, med_font, 1);
    loading_INITIATING_box.Text = gui_strings[376];
    lbFontPtr = med_font;
    loading_INITIATING_box.Height = font_height(0x41u) + 8;
    w = my_string_width(gui_strings[376]);
    loading_INITIATING_box.Width = w + 9;
    loading_INITIATING_box.X = 319 - ((w + 9) >> 1);
    loading_INITIATING_box.Y = 219 - (loading_INITIATING_box.Height >> 1);

    init_screen_box(&alert_box, 219u, 189u, 200u, 100, 6);
    init_screen_button(&alert_OK_button, 10u, 269u,
      gui_strings[458], 6, med2_font, 1, 0);
    alert_OK_button.CallBackFn = ac_alert_OK;
    alert_OK_button.X = 319 - (alert_OK_button.Width >> 1);
    alert_OK_button.AccelKey = 28;
}

void init_system_menu_boxes(void)
{
    int i, h, val;

    init_screen_text_box(&heading_box, 7u, 25u, 626u, 38, 6, big_font, 1);
    heading_box.DrawTextFn = ac_show_title_box;
    heading_box.Text = options_title_text;

    init_screen_text_box(&unkn13_SYSTEM_button, 7u, 25u, 197u, 38, 6,
      big_font, 1);
    unkn13_SYSTEM_button.Text = gui_strings[366];
    unkn13_SYSTEM_button.DrawTextFn = ac_show_title_box;

    val = 0;
    h = 72;
    for (i = 0; i < 6; i++)
    {
        init_screen_button(&sysmnu_buttons[i], 7, h,
          gui_strings[378 + val], 6, med2_font, 1, 0);
        sysmnu_buttons[i].Width = 197;
        sysmnu_buttons[i].Height = 21;
        sysmnu_buttons[i].CallBackFn = ac_do_sysmnu_button;
        sysmnu_buttons[i].Flags |= 0x10;
        sysmnu_buttons[i].Border = 3;
        val++;
        h += 30;
    }
}

void reset_system_menu_boxes_flags(void)
{
    int i;

    unkn13_SYSTEM_button.Flags = 0x0001;

    for (i = 0; i < 6; i++) {
        sysmnu_buttons[i].Flags = 0x0011;
    }
}

void init_screen_boxes(void)
{
    init_alert_screen_boxes();
    init_main_screen_boxes();
    init_system_menu_boxes();
    init_brief_screen_boxes();
    init_debrief_screen_boxes();
    init_world_screen_boxes();
    init_options_screen_boxes();
    init_controls_screen_boxes();
    init_storage_screen_boxes();
    init_net_screen_boxes();
    init_login_screen_boxes();
    init_equip_screen_boxes();
    init_cryo_screen_boxes();
    init_research_screen_boxes();
}

void update_menus(void)
{
    asm volatile ("call ASM_update_menus\n"
        :  :  : "eax" );
}

void reload_background(void)
{
#if 0
    asm volatile ("call ASM_reload_background\n"
        :  :  : "eax" );
#else
    if (screentype == 6 || screentype == 10 || restore_savegame)
    {
        struct TbSprite *spr;
        ubyte *scr_bkp;
        int x, y;

        scr_bkp = lbDisplay.WScreen;
        lbDisplay.WScreen = back_buffer;
        spr = &sprites_Icons0_0[168];
        for (y = 0; y < 480; y += spr->SHeight)
        {
            for (x = 0; x < 640; x += spr->SWidth) {
                LbSpriteDraw(x, y, spr);
            }
        }
        lbDisplay.WScreen = scr_bkp;
    }
    else
    {
        struct Campaign *p_campgn;
        char str[52];
        const char *campgn_mark;
        const char *bkdata_dir;

        p_campgn = &campaigns[background_type];
        campgn_mark = p_campgn->ProjectorFnMk;
        bkdata_dir = "qdata";

        sprintf(str, "%s/%s-proj.dat", bkdata_dir, campgn_mark);
        LbFileLoadAt(str, back_buffer);
    }

    if (screentype == 5 && selected_weapon != -1)
    {
        init_weapon_anim(selected_weapon);
    }
    if (screentype == 4 && selected_mod != -1)
    {
        init_weapon_anim(selected_mod + 32);
    }
#endif
}

void players_init_control_mode(void)
{
    int player;
    for (player = 0; player < 8; player++) {
      players[player].UserInput[0].ControlMode = 1;
    }
}

void srm_scanner_reset(void)
{
    int i;

    ingame.Scanner.X1 = 8;
    ingame.Scanner.Y2 = 270;
    ingame.Scanner.MX = 127;
    ingame.Scanner.MZ = 127;
    ingame.Scanner.Angle = 0;
    ingame.Scanner.X2 = 325;
    ingame.Scanner.Zoom = 256;
    ingame.Scanner.Y1 = 73;

    for (i = 0; i + ingame.Scanner.Y1 <= ingame.Scanner.Y2; i++)
    {
        ingame.Scanner.Width[i] = 318;
    }
    for (i = 0; i != 16; i++)
    {
        ingame.Scanner.BigBlip[i].Period = 0;
    }
    SCANNER_width = ingame.Scanner.Width;
    SCANNER_init();
}

void move_camera(int x, int y, int z)
{
    asm volatile (
      "call ASM_move_camera\n"
        : : "a" (x), "d" (y), "b" (z));
}

void track_player(int thing)
{
    asm volatile ("call ASM_track_player\n"
        : : "a" (thing));
}

void track_angle(void)
{
    asm volatile ("call ASM_track_angle\n"
        :  :  : "eax" );
}

void track_y(int y)
{
    engn_yc += (y - engn_yc) >> 3;
}

void local_to_worldr(int *dx, int *dy, int *dz)
{
    asm volatile (
      "call ASM_local_to_worldr\n"
        : : "a" (dx), "d" (dy), "b" (dz));
}

ushort my_draw_text(short x, short y, const char *text, ushort startline)
{
    ushort ret;
    asm volatile ("call ASM_my_draw_text\n"
        : "=r" (ret) : "a" (x), "d" (y), "b" (text), "c" (startline));
    return ret;
}

ubyte sub_71694(int a1, int a2, char *text, int a4, ubyte a5, ubyte a6)
{
    ubyte ret;
    asm volatile (
      "push %6\n"
      "push %5\n"
      "call ASM_sub_71694\n"
        : "=r" (ret) : "a" (a1), "d" (a2), "b" (text), "c" (a4), "g" (a5), "g" (a6));
    return ret;
}

void do_scroll_map(void)
{
#if 0
    asm volatile ("call ASM_do_scroll_map\n"
        :  :  : "eax" );
#else
    PlayerInfo *p_locplayer;
    long engn_xc_orig, engn_zc_orig;
    ushort md;
    long abase, angle;
    int dx, dy;
    int dampr;

    dx = 0;
    dy = 0;
    dampr = 10;
    if (ingame.fld_unkCA6)
        track_angle();
    p_locplayer = &players[local_player_no];
    if (p_locplayer->State[0] == 1)
    {
        ushort bitx, bity;
        int dz;
        // TODO check if this makes sense
        bitx = (p_locplayer->UserInput[0].Bits >> 0);
        bity = (p_locplayer->UserInput[0].Bits >> 8);
        dx = (bitx & 0xFF) << 8;
        dz = (bity & 0xFF) << 8;
        local_to_worldr(&dx, &dy, &dz);
        engn_xc += dx;
        engn_zc += dz;
    }
    else
    {
        ulong dcthing;
        if (p_locplayer->DoubleMode)
        {
            if (byte_153198)
                dcthing = p_locplayer->DirectControl[byte_153198-1];
            else
                dcthing = p_locplayer->DirectControl[0];
            track_player(dcthing);
            return;
        }
        dcthing = p_locplayer->DirectControl[0];
        if (dcthing)
        {
            md = p_locplayer->UserInput[0].ControlMode & 0x1FFF;
            if (md == 1 || pktrec_mode == PktR_PLAYBACK)
                move_camera(ingame.TrackX, engn_yc, ingame.TrackZ);
            else
                track_player(dcthing);
        }
    }
    dy = 0;
    dx = 0;
    md = p_locplayer->UserInput[byte_153198-1].ControlMode & 0x1FFF;
    engn_xc_orig = engn_xc;
    engn_zc_orig = engn_zc;
    if (md == 1 || pktrec_mode == PktR_PLAYBACK)
    {
        if (!p_locplayer->PanelState[mouser])
        {
            long cumm_alt;
            int mv_border;

            // Define a move border, getting the mouse beyond it causes map scroll
            if (lbDisplay.GraphicsScreenWidth >= 640)
                mv_border = 3;
            else
                mv_border = 2;

            if (!p_locplayer->DoubleMode)
            {
                dx = (lbKeyOn[kbkeys[GKey_RIGHT]] & 1) - (lbKeyOn[kbkeys[GKey_LEFT]] & 1);
                dy = (lbKeyOn[kbkeys[GKey_DOWN]] & 1) - (lbKeyOn[kbkeys[GKey_UP]] & 1);
            }

            if (dx == 0)
            {
                int mx;
                mx = lbDisplay.MMouseX;
                if (mx >= lbDisplay.GraphicsScreenWidth - mv_border)
                    dx = 1;
                if (mx < mv_border)
                    dx = -1;
            }
            if (dy == 0)
            {
                int my;
                my = lbDisplay.MMouseY;
                if (my >= lbDisplay.GraphicsScreenHeight - mv_border)
                    dy = 1;
                if (my < mv_border)
                    dy = -1;
            }
            cumm_alt = alt_at_point(engn_xc, engn_zc) >> 8;
            cumm_alt += alt_at_point(engn_xc + 2048, engn_zc + 2048) >> 8;
            cumm_alt += alt_at_point(engn_xc + 2048, engn_zc - 2048) >> 8;
            cumm_alt += alt_at_point(engn_xc - 2048, engn_zc + 2048) >> 8;
            cumm_alt += alt_at_point(engn_xc - 2048, engn_zc - 2048) >> 8;
            track_y(cumm_alt / 5);
            dampr = 9;
        }
    }

    abase = -dword_176D58 >> 5;
    angle = -1;
    if (dx > 0)
        angle = (abase + 3583) & 0x7FF;
    if (dx < 0)
        angle = (abase + 2559) & 0x7FF;
    if (angle >= 0) {
        int wibl_x, wibl_y;
        wibl_x = dword_153194 * lbSinTable[angle] >> 8 >> dampr;
        wibl_y = dword_153194 * lbSinTable[angle + 512] >> 8 >> dampr;
        ingame.TrackX -= wibl_x;
        ingame.TrackZ += wibl_y;
        engn_xc -= wibl_x;
        engn_zc += wibl_y;
    }
    angle = -1;
    if (dy > 0)
        angle = (abase + 3071) & 0x7FF;
    if (dy < 0)
        angle = (abase + 2047) & 0x7FF;
    if (angle >= 0) {
        int wibl_x, wibl_y;
        wibl_x = dword_153194 * lbSinTable[angle] >> 8 >> dampr;
        wibl_y = dword_153194 * lbSinTable[angle + 512] >> 8 >> dampr;
        ingame.TrackX -= wibl_x;
        ingame.TrackZ += wibl_y;
        engn_xc -= wibl_x;
        engn_zc += wibl_y;
    }

    if (engn_xc < 0)
        engn_xc = 0;
    else if (engn_xc >= 0x8000)
        engn_xc = 0x7FFF;
    if (engn_zc < 0)
        engn_zc = 0;
    else if (engn_zc >= 0x8000)
        engn_zc = 0x7FFF;
    if (ingame.TrackX > 0xC000)
        ingame.TrackX = 0;
    else if (ingame.TrackX >= 0x8000)
        ingame.TrackX = 0x7FFF;
    if (ingame.TrackZ > 0xC000)
        ingame.TrackZ = 0;
    else if (ingame.TrackZ >= 0x8000)
        ingame.TrackZ = 0x7FFF;

    if (dx) {
        dword_153194 += 4 + ((dword_153194 - 252) >> 5);
        if (dword_153194 > 800)
            dword_153194 = 800;
    } else {
        dword_153194 -= (dword_153194 - 256) >> 2;
    }
    if ((engn_zc - engn_zc_orig) || (engn_xc - engn_xc_orig)) {
        dword_17710C = engn_xc - engn_xc_orig;
        dword_177110 = engn_zc - engn_zc_orig;
    } else {
        dword_17710C >>= 2;
        dword_177110 >>= 2;
        if (abs(dword_17710C) < 5)
            dword_17710C = 0;
        if (abs(dword_177110) < 5)
            dword_177110 = 0;
        engn_xc += dword_17710C;
        engn_zc += dword_177110;
    }
#endif
}

void do_rotate_map(void)
{
    asm volatile ("call ASM_do_rotate_map\n"
        :  :  : "eax" );
}

TbResult read_palette_file(void)
{
    TbResult ret;
    asm volatile ("call ASM_read_palette_file\n"
        : "=r" (ret) : );
    return ret;
}

TbBool pause_screen_handle(void)
{
#if 0
    TbBool ret;
    asm volatile ("call ASM_pause_screen_handle\n"
        : "=r" (ret) : );
    return ret;
#endif
    int w;
    const char *s;
    int x1, x2, y1;
    int ms_x, ms_y, i;
    TbBool is_unkn1;
    TbPixel colr1, colr2;

    if ((ingame.PanelPermutation != 2) && (ingame.PanelPermutation != -3))
        colr1 = 20;
    else
        colr1 = 40;
    snd_unkn1_volume_all_samples();
    person_func_unknown_310(2u);
    lbKeyOn[kbkeys[GKey_PAUSE]] = 0;
    while ((jskeys[GKey_PAUSE] != 0) &&
      (jskeys[GKey_PAUSE] != joy.Buttons[0]))
    {
        joy_func_065(&joy);
    }
    do_change_mouse(8);

    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    if (lbDisplay.ScreenMode == 1)
        LbDrawBox(43, 27, 208, 97, colr1);
    else
        LbDrawBox(86, 54, 416, 194, colr1);
    if (lbDisplay.ScreenMode == 1)
        LbDrawBox(251, 52, 25, 72, colr1);
    else
        LbDrawBox(502, 104, 50, 144, colr1);
    if (lbDisplay.ScreenMode == 1)
        LbDrawBox(68, 124, 208, 25, colr1);
    else
        LbDrawBox(136, 248, 416, 50, colr1);
    if (lbDisplay.ScreenMode == 1)
    {
        LbDrawTriangle(251, 27, 276, 52, 251, 52, colr1);
        LbDrawTriangle(44, 124, 68, 124, 68, 148, colr1);
    }
    else
    {
        LbDrawTriangle(502, 54, 552, 104, 502, 104, colr1);
        LbDrawTriangle(88, 248, 136, 248, 136, 296, colr1);
    }

    lbDisplay.DrawFlags = 0;
    if (lbDisplay.ScreenMode == 1)
        LbDrawLine(43, 27, 251, 27, colr1);
    else
        LbDrawLine(86, 54, 502, 54, colr1);
    if (lbDisplay.ScreenMode == 1)
        LbDrawLine(43, 27, 43, 123, colr1);
    else
        LbDrawLine(86, 54, 86, 246, colr1);
    if (lbDisplay.ScreenMode == 1)
        LbDrawLine(276, 52, 276, 148, colr1);
    else
        LbDrawLine(552, 104, 552, 296, colr1);
    if (lbDisplay.ScreenMode == 1)
        LbDrawLine(68, 148, 276, 148, colr1);
    else
        LbDrawLine(136, 296, 552, 296, colr1);
    if (lbDisplay.ScreenMode == 1)
    {
        LbDrawLine(251, 27, 276, 52, colr1);
        LbDrawLine(44, 124, 68, 148, colr1);
    }
    else
    {
        LbDrawLine(502, 54, 552, 104, colr1);
        LbDrawLine(88, 248, 136, 296, colr1);
    }

    if ((ingame.PanelPermutation != 2) && (ingame.PanelPermutation != -3))
        colr2 = 15;
    else
        colr2 = 35;
    lbFontPtr = small_font;
    my_set_text_window(0, 0, lbDisplay.PhysicalScreenWidth,
      lbDisplay.PhysicalScreenHeight);
    if ((ingame.PanelPermutation == 2) || (ingame.PanelPermutation == -3))
    {
        lbDisplay.DrawFlags |= 0x0040;
        lbDisplay.DrawColour = colr2;
    }
    if (lbDisplay.ScreenMode == 1)
    {
        s = mission_name;
        w = my_string_width(s);
        my_draw_text(161 - (w >> 1), 44, s, 0);
        s = gui_strings[419];
        w = my_string_width(s);
        my_draw_text(161 - (w >> 1), 54, s, 0);
        s = gui_strings[420];
        w = my_string_width(s);
        my_draw_text(161 - (w >> 1), 76, s, 0);
        s = gui_strings[516];
        w = my_string_width(s);
        my_draw_text(161 - (w >> 1), 98, s, 0);
    }
    else
    {
        s = mission_name;
        w = my_string_width(s);
        my_draw_text(322 - (w >> 1), 88, s, 0);
        s = gui_strings[419];
        w = my_string_width(s);
        my_draw_text(322 - (w >> 1), 108, s, 0);
        s = gui_strings[420];
        w = my_string_width(s);
        my_draw_text(322 - (w >> 1), 152, s, 0);
        s = gui_strings[516];
        w = my_string_width(s);
        my_draw_text(322 - (w >> 1), 196, s, 0);
    }
    if (ingame.PanelPermutation == 2 || ingame.PanelPermutation == -3)
        lbDisplay.DrawFlags &= ~0x0040;

    if (language_3str[0] == 'e')
    {
        if (lbDisplay.ScreenMode == 1)
        {
            LbSpriteDraw(89, 31, &pop1_sprites[102]);
            LbSpriteDraw(83, 120, &pop1_sprites[103]);
        }
        else
        {
            LbSpriteDraw(178, 62, &pop1_sprites[102]);
            w = pop1_sprites[102].SWidth;
            LbSpriteDraw(178 + w, 62, &pop1_sprites[104]);
            LbSpriteDraw(166, 240, &pop1_sprites[103]);
        }
    }
    else
    {
        if (lbDisplay.ScreenMode == 1)
        {
            s = gui_strings[606];
            w = my_string_width(s);
            my_draw_text(161 - (w >> 1), 31, s, 0);
            s = gui_strings[470];
            w = my_string_width(s);
            my_draw_text(103 - (w >> 1), 123, s, 0);
        }
        else
        {
            s = gui_strings[606];
            w = my_string_width(s);
            my_draw_text(322 - (w >> 1), 62, s, 0);
            s = gui_strings[470];
            w = my_string_width(s);
            my_draw_text(206 - (w >> 1), 246, s, 0);
        }
    }
    swap_wscreen();
    if (!ingame.fld_unk7DA)
        SetMusicVolume(100, 0x7F);

    is_unkn1 = 0;
    while (!lbKeyOn[kbkeys[GKey_PAUSE]]
        && (!jskeys[GKey_PAUSE] || jskeys[GKey_PAUSE] != joy.Buttons[0])
        && !is_unkn1)
    {
        game_update();
        joy_func_065(&joy);
        PlayCDTrack(ingame.CDTrack);

        x2 = 442;
        y1 = 122;
        x1 = 204;
        while (y1 < 140)
        {
            if (lbDisplay.ScreenMode == 1)
                LbDrawLine(x1 >> 1, y1 >> 1, x2 >> 1, y1 >> 1, colr2);
            else
                LbDrawLine(x1, y1, x2, y1, colr2);
            y1 += 2;
            x2 -= 2;
            x1 -= 2;
        }

        x2 = 442;
        y1 = 166;
        x1 = 204;
        while (y1 < 184)
        {
            if (lbDisplay.ScreenMode == 1)
                LbDrawLine(x1 >> 1, y1 >> 1, x2 >> 1, y1 >> 1, colr2);
            else
                LbDrawLine(x1, y1, x2, y1, colr2);
            y1 += 2;
            x2 -= 2;
            x1 -= 2;
        }

        x2 = 442;
        y1 = 210;
        x1 = 204;
        while (y1 < 228)
        {
            if (lbDisplay.ScreenMode == 1)
                LbDrawLine(x1 >> 1, y1 >> 1, x2 >> 1, y1 >> 1, colr2);
            else
                LbDrawLine(x1, y1, x2, y1, colr2);
            y1 += 2;
            x2 -= 2;
            x1 -= 2;
        }

        if (lbDisplay.ScreenMode != 1)
        {
            y1 = 123;
            x2 = 441;
            x1 = 203;
            while (y1 < 141)
            {
                if (lbDisplay.ScreenMode == 1)
                    LbDrawLine(x1 >> 1, y1 >> 1, x2 >> 1, y1 >> 1, colr2);
                else
                    LbDrawLine(x1, y1, x2, y1, colr2);
                y1 += 2;
                x2 -= 2;
                x1 -= 2;
            }

            x2 = 441;
            y1 = 167;
            x1 = 203;
            while (y1 < 185)
            {
                if (lbDisplay.ScreenMode == 1)
                    LbDrawLine(x1 >> 1, y1 >> 1, x2 >> 1, y1 >> 1, colr2);
                else
                    LbDrawLine(x1, y1, x2, y1, colr2);
                y1 += 2;
                x2 -= 2;
                x1 -= 2;
            }

            y1 = 211;
            x2 = 441;
            x1 = 203;
            while (y1 < 229)
            {
                if (lbDisplay.ScreenMode == 1)
                    LbDrawLine(x1 >> 1, y1 >> 1, x2 >> 1, y1 >> 1, colr2);
                else
                    LbDrawLine(x1, y1, x2, y1, colr2);
                y1 += 2;
                x2 -= 2;
                x1 -= 2;
            }
        }

        if (lbDisplay.ScreenMode == 1)
            ms_x = 2 * lbDisplay.MMouseX;
        else
            ms_x = lbDisplay.MMouseX;
        if (lbDisplay.ScreenMode == 1)
            ms_y = 2 * lbDisplay.MMouseY;
        else
            ms_y = lbDisplay.MMouseY;
        if ((ms_x >> 1 >= 88) && (ms_x >> 1 <= 92) && (ms_y >> 1 >= 62) && (ms_y >> 1 <= 70))
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDrawOneColour(88, 60, &pop1_sprites[100],
                  colour_lookup[1]);
            }
            else
            {
                LbSpriteDrawOneColour(176, 120, &pop1_sprites[100],
                  colour_lookup[1]);
            }
            if (lbDisplay.MLeftButton)
            {
                lbDisplay.LeftButton = 0;
                --startscr_samplevol;
                if (startscr_samplevol < 0)
                    startscr_samplevol = 0;
                SetSoundMasterVolume(127 * startscr_samplevol / 322);
                if (!IsSamplePlaying(0, 80, 0))
                    play_sample_using_heap(0, 80, 127, 64, 100, 0, 1u);
            }
        }
        else
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDraw(88, 60, &pop1_sprites[100]);
            }
            else
            {
                LbSpriteDraw(176, 120, &pop1_sprites[100]);
            }
        }

        if (lbDisplay.ScreenMode == 1)
            ms_x = 2 * lbDisplay.MMouseX;
        else
            ms_x = lbDisplay.MMouseX;
        if (lbDisplay.ScreenMode == 1)
            ms_y = 2 * lbDisplay.MMouseY;
        else
            ms_y = lbDisplay.MMouseY;
        if ((ms_x >> 1 >= 88) && (ms_x >> 1 <= 92) && (ms_y >> 1 >= 84) && (ms_y >> 1 <= 92))
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDrawOneColour(88, 82,
                  &pop1_sprites[100], colour_lookup[1]);
            }
            else
            {
                LbSpriteDrawOneColour(176, 164,
                  &pop1_sprites[100], colour_lookup[1]);
            }
            if (lbDisplay.MLeftButton)
            {
                lbDisplay.LeftButton = 0;
                --startscr_midivol;
                if (startscr_midivol < 0)
                    startscr_midivol = 0;
                SetMusicMasterVolume(127 * startscr_midivol / 322);
            }
        }
        else
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDraw(88, 82, &pop1_sprites[100]);
            }
            else
            {
                LbSpriteDraw(176, 164, &pop1_sprites[100]);
            }
        }

        if (lbDisplay.ScreenMode == 1)
            ms_x = 2 * lbDisplay.MMouseX;
        else
            ms_x = lbDisplay.MMouseX;
        if (lbDisplay.ScreenMode == 1)
            ms_y = 2 * lbDisplay.MMouseY;
        else
            ms_y = lbDisplay.MMouseY;
        if ((ms_x >> 1 >= 88) && (ms_x >> 1 <= 92) && (ms_y >> 1 >= 106) && (ms_y >> 1 <= 114))
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDrawOneColour(88, 104,
                  &pop1_sprites[100], colour_lookup[1]);
            }
            else
            {
                LbSpriteDrawOneColour(176, 208,
                  &pop1_sprites[100], colour_lookup[1]);
            }
            if (lbDisplay.MLeftButton)
            {
                lbDisplay.LeftButton = 0;
                --startscr_cdvolume;
                if (startscr_cdvolume < 0)
                    startscr_cdvolume = 0;
                SetCDVolume(70 * (127 * startscr_cdvolume / 322) / 100);
            }
        }
        else
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDraw(88, 104, &pop1_sprites[100]);
            }
            else
            {
                LbSpriteDraw(176, 208, &pop1_sprites[100]);
            }
        }

        if (lbDisplay.ScreenMode == 1)
            ms_x = 2 * lbDisplay.MMouseX;
        else
            ms_x = lbDisplay.MMouseX;
        if (lbDisplay.ScreenMode == 1)
            ms_y = 2 * lbDisplay.MMouseY;
        else
            ms_y = lbDisplay.MMouseY;
        if ((ms_x >> 1 >= 222) && (ms_x >> 1 <= 226) && (ms_y >> 1 >= 60) && (ms_y >> 1 <= 68))
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDrawOneColour(215, 60,
                  &pop1_sprites[101], colour_lookup[1]);
            }
            else
            {
                LbSpriteDrawOneColour(430, 120,
                  &pop1_sprites[101], colour_lookup[1]);
            }
            if (lbDisplay.MLeftButton)
            {
                lbDisplay.LeftButton = 0;
                ++startscr_samplevol;
                if (startscr_samplevol > 322)
                    startscr_samplevol = 322;
                SetSoundMasterVolume(127 * startscr_samplevol / 322);
                if (!IsSamplePlaying(0, 80, 0))
                    play_sample_using_heap(0, 80, 127, 64, 100, 0, 1u);
            }
        }
        else
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDraw(215, 60, &pop1_sprites[101]);
            }
            else
            {
                LbSpriteDraw(430, 120, &pop1_sprites[101]);
            }
        }

        if (lbDisplay.ScreenMode == 1)
            ms_x = 2 * lbDisplay.MMouseX;
        else
            ms_x = lbDisplay.MMouseX;
        if (lbDisplay.ScreenMode == 1)
            ms_y = 2 * lbDisplay.MMouseY;
        else
            ms_y = lbDisplay.MMouseY;
        if ((ms_x >> 1 >= 222) && (ms_x >> 1 <= 226) && (ms_y >> 1 >= 82) && (ms_y >> 1 <= 90))
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDrawOneColour(215, 82,
                  &pop1_sprites[101], colour_lookup[1]);
            }
            else
            {
                LbSpriteDrawOneColour(430, 164,
                  &pop1_sprites[101], colour_lookup[1]);
            }
            if (lbDisplay.MLeftButton)
            {
                lbDisplay.LeftButton = 0;
                ++startscr_midivol;
                if (startscr_midivol > 322)
                    startscr_midivol = 322;
                SetMusicMasterVolume(127 * startscr_midivol / 322);
            }
        }
        else
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDraw(215, 82, &pop1_sprites[101]);
            }
            else
            {
                LbSpriteDraw(430, 164, &pop1_sprites[101]);
            }
        }

        if (lbDisplay.ScreenMode == 1)
            ms_x = 2 * lbDisplay.MMouseX;
        else
            ms_x = lbDisplay.MMouseX;
        if (lbDisplay.ScreenMode == 1)
            ms_y = 2 * lbDisplay.MMouseY;
        else
            ms_y = lbDisplay.MMouseY;
        if ((ms_x >> 1 >= 222) && (ms_x >> 1 <= 226) && (ms_y >> 1 >= 104) && (ms_y >> 1 <= 112))
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDrawOneColour(215, 104, &pop1_sprites[101],
                  colour_lookup[1]);
            }
            else
            {
                LbSpriteDrawOneColour(430, 208, &pop1_sprites[101],
                  colour_lookup[1]);
            }
            if (lbDisplay.MLeftButton)
            {
                lbDisplay.LeftButton = 0;
                ++startscr_cdvolume;
                if (startscr_cdvolume > 322)
                    startscr_cdvolume = 322;
                SetCDVolume(70 * (127 * startscr_cdvolume / 322) / 100);
            }
        }
        else
        {
            if (lbDisplay.ScreenMode == 1)
            {
                LbSpriteDraw(215, 104, &pop1_sprites[101]);
            }
            else
            {
                LbSpriteDraw(430, 208, &pop1_sprites[101]);
            }
        }

        if (lbDisplay.MLeftButton)
        {
            ms_x = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
            ms_y = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
            if ((ms_y >> 1 >= 62) && (ms_y >> 1 <= 68))
            {
                int ms_delta;
                ms_delta = (ms_y >> 1) + (ms_x >> 1) - 62;
                if (ms_delta >= 103 && ms_delta <= 219)
                {
                    startscr_samplevol = 322 * (ms_delta - 103) / 116;
                    SetSoundMasterVolume(127 * startscr_samplevol / 322);
                    if (!IsSamplePlaying(0, 80, 0))
                        play_sample_using_heap(0, 80, 127, 64, 100, 0, 1u);
                }
            }
        }

        if (lbDisplay.MLeftButton)
        {
            ms_x = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
            ms_y = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
            if ((ms_y >> 1 >= 84) && (ms_y >> 1 <= 90))
            {
                int ms_delta;
                ms_delta = (ms_y >> 1) + (ms_x >> 1) - 84;
                if (ms_delta >= 103 && ms_delta <= 219)
                {
                    startscr_midivol = 322 * (ms_delta - 103) / 116;
                    SetMusicMasterVolume(127 * startscr_midivol / 322);
                }
            }
        }

        if (lbDisplay.MLeftButton)
        {
            ms_x = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
            ms_y = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
            if ((ms_y >> 1 >= 106) && (ms_y >> 1 <= 112))
            {
                int ms_delta;
                ms_delta = (ms_y >> 1) + (ms_x >> 1) - 106;
                if (ms_delta >= 103 && ms_delta <= 219)
                {
                    startscr_cdvolume = 322 * (ms_delta - 103) / 116;
                    SetCDVolume(70 * (127 * startscr_cdvolume / 322) / 100);
                }
            }
        }

        if (startscr_samplevol)
        {
            int val7;
            x1 = 206;
            y1 = 124;
            for (i = 0; i < 7; i++)
            {
                val7 = 103 - i;
                if (lbDisplay.ScreenMode == 1)
                    LbDrawLine(x1 >> 1, y1 >> 1,
                        (2 * (116 * startscr_samplevol / 322 + val7))
                            >> 1, y1 >> 1, colour_lookup[1]);
                else
                    LbDrawLine(x1, y1,
                        2 * (116 * startscr_samplevol / 322 + val7),
                        y1, colour_lookup[1]);
                x1 -= 2;
                y1 += 2;
            }
            if (lbDisplay.ScreenMode != 1)
            {
                x1 = 205;
                y1 = 125;
                for (i = 0; i < 7; i++)
                {
                    val7 = 103 - i;
                    if (lbDisplay.ScreenMode == 1)
                        LbDrawLine(x1 >> 1, y1 >> 1,
                            (2 * (116 * startscr_samplevol / 322 + val7)
                                - 1) >> 1, y1 >> 1, colour_lookup[1]);
                    else
                        LbDrawLine(x1, y1,
                            2 * (val7 + 116 * startscr_samplevol / 322)
                                - 1, y1, colour_lookup[1]);
                    x1 -= 2;
                    y1 += 2;
                }
            }
        }

        if (startscr_midivol)
        {
            int val7;
            x1 = 206;
            y1 = 168;
            for (i = 0; i < 7; i++)
            {
                val7 = 103 - i;
                if (lbDisplay.ScreenMode == 1)
                    LbDrawLine(x1 >> 1, y1 >> 1,
                        (2 * (116 * startscr_midivol / 322 + val7))
                            >> 1, y1 >> 1, colour_lookup[1]);
                else
                    LbDrawLine(x1, y1,
                        2 * (116 * startscr_midivol / 322 + val7),
                        y1, colour_lookup[1]);
                x1 -= 2;
                y1 += 2;
            }
            if (lbDisplay.ScreenMode != 1)
            {
                x1 = 205;
                y1 = 169;
                for (i = 0; i < 7; i++)
                {
                    val7 = 103 - i;
                    if (lbDisplay.ScreenMode == 1)
                        LbDrawLine(x1 >> 1, y1 >> 1,
                            (2
                                * (116 * startscr_midivol / 322
                                    + val7) - 1) >> 1, y1 >> 1,
                            colour_lookup[1]);
                    else
                        LbDrawLine(x1, y1,
                            2
                                * (116 * startscr_midivol / 322
                                    + val7) - 1, y1, colour_lookup[1]);
                    x1 -= 2;
                    y1 += 2;
                }
            }
        }

        if (startscr_cdvolume)
        {
            int val7;
            x1 = 206;
            y1 = 212;
            for (i = 0; i < 7; i++)
            {
                val7 = 103 - i;
                if (lbDisplay.ScreenMode == 1)
                    LbDrawLine(x1 >> 1, y1 >> 1,
                        (2 * (116 * startscr_cdvolume / 322 + val7))
                            >> 1, y1 >> 1, colour_lookup[1]);
                else
                    LbDrawLine(x1, y1,
                        2 * (116 * startscr_cdvolume / 322 + val7),
                        y1, colour_lookup[1]);
                x1 -= 2;
                y1 += 2;
            }
            if (lbDisplay.ScreenMode != 1)
            {
                x1 = 205;
                y1 = 213;
                for (i = 0; i < 7; i++)
                {
                    val7 = 103 - i;
                    if (lbDisplay.ScreenMode == 1)
                        LbDrawLine(x1 >> 1, y1 >> 1,
                            (2 * (val7 + 116 * startscr_cdvolume / 322)
                                - 1) >> 1, y1 >> 1, colour_lookup[1]);
                    else
                        LbDrawLine(x1, y1,
                            2 * (val7 + 116 * startscr_cdvolume / 322)
                                - 1, y1, colour_lookup[1]);
                    x1 -= 2;
                    y1 += 2;
                }
            }
        }

        if (sub_71694(180, 120, gui_strings[477], colr1, colr2, ingame.DetailLevel == 1))
        {
            ingame.DetailLevel = 1;
            bang_set_detail(0);
        }
        if (sub_71694(220, 120, gui_strings[475], colr1, colr2, ingame.DetailLevel == 0))
        {
            ingame.DetailLevel = 0;
            bang_set_detail(1);
        }
        if (sub_71694(140, 134, gui_strings[455], colr1, colr2, 0))
            is_unkn1 = 1;
        if (sub_71694(197, 134, gui_strings[445], colr1, colr2, 0))
        {
            swap_wscreen();
            SetMusicVolume(100, 0);
            StopAllSamples();
            StopCD();
            return 1;
        }
        lbDisplay.DrawFlags = 0;
        process_sound_heap();
        swap_wscreen();
    }

    while ((lbKeyOn[kbkeys[GKey_PAUSE]])
        || (jskeys[GKey_PAUSE] && jskeys[GKey_PAUSE] == joy.Buttons[0]))
    {
        game_update();
        joy_func_065(&joy);
        PlayCDTrack(ingame.CDTrack);
        swap_wscreen();
    }
    lbDisplay.RightButton = 0;
    lbDisplay.LeftButton = 0;
    if (!ingame.fld_unk7DA)
        SetMusicVolume(100, 0);
    return 0;
}

ubyte do_user_interface(void)
{
    PlayerInfo *p_locplayer;
    int n;

    p_locplayer = &players[local_player_no];
    person_func_unknown_310(0);
    do_scroll_map();
    do_rotate_map();
    if (in_network_game)
    {
        if (lbKeyOn[KC_RETURN])
        {
            lbKeyOn[KC_RETURN] = 0;
            if ((p_locplayer->PanelState[mouser] != 17) && (player_unkn0C9[local_player_no] <= 140))
            {
                p_locplayer->PanelState[mouser] = 17;
                reset_buffered_keys();
                player_unknCC9[local_player_no][0] = '\0';
                player_unkn0C9[local_player_no] = 0;
                scanner_unkn370 = 0;
                scanner_unkn3CC = 0;
            }
        }
    }
    if (p_locplayer->PanelState[mouser] == 17)
        return process_mouse_imputs() != 0;

    // screenshot
    if (lbKeyOn[KC_M])
        LbPngSaveScreen("synII", lbDisplay.WScreen, display_palette, 0);

    // TODO No idea what these are doing
    for (n = 0; n < 5; n++)
    {
        if (lbKeyOn[KC_F1+n])
        {
            if (lbShift & KMod_ALT)
            {
                lbKeyOn[KC_F1+n] = 0;
                my_build_packet(&packets[local_player_no], PAct_36, n, 0, 0, 0);
                return 1;
            }
            if (lbShift & KMod_SHIFT)
            {
                lbKeyOn[KC_F1+n] = 0;
                my_build_packet(&packets[local_player_no], PAct_35, n, 0, 0, 0);
                return 1;
            }
        }
    }

    // change panel style
    if (lbKeyOn[KC_F9] && (lbShift == KMod_NONE))
    {
        sbyte panperm;

        lbKeyOn[KC_F9] = 0;
        StopCD();
        if (ingame.PanelPermutation >= 0)
        {
            ingame.PanelPermutation++;
            if (ingame.PanelPermutation > 2)
                ingame.PanelPermutation = 0;
        }
        else
        {
            ingame.PanelPermutation--;
            if (ingame.PanelPermutation < -3)
                ingame.PanelPermutation = -1;
        }
        panperm = ingame.PanelPermutation;
        if ((panperm == 2) || (panperm == -3))
        {
            SCANNER_set_colour(1);
            SCANNER_fill_in();
        } else
        if ((panperm == 0) || (panperm == -1))
        {
            SCANNER_set_colour(2);
            SCANNER_fill_in();
        }
        if (lbDisplay.ScreenMode == 1)
            load_pop_sprites_lo();
        else
            load_pop_sprites_hi();
    }

    // change agents colours
    if (lbKeyOn[KC_F10] && (lbShift == KMod_NONE))
    {
        lbKeyOn[KC_F10] = 0;
        StopCD();
        if (++ingame.TrenchcoatPreference > 5)
            ingame.TrenchcoatPreference = 0;
        load_multicolor_sprites();
    }

    // adjust palette brightness
    if (lbKeyOn[KC_F11])
    {
        lbKeyOn[KC_F11] = 0;
        if (lbShift & KMod_CONTROL)
        {
            read_palette_file();
        }
        else if (lbShift & KMod_SHIFT)
        {
            change_brightness(-1);
            brightness--;
        }
        else if (lbShift == KMod_NONE)
        {
            change_brightness(1);
            brightness++;
        }
    }

    // TODO No idea what these are doing
    if (lbKeyOn[KC_F1] && (lbShift & KMod_CONTROL))
    {
        lbKeyOn[KC_F1] = 0;
        if (ingame.Flags & GamF_Unkn0001)
            ingame.Flags &= ~GamF_Unkn0001;
        else
            ingame.Flags |= GamF_Unkn0001;
    }
    if (lbKeyOn[KC_F2] && (lbShift & KMod_CONTROL))
    {
        lbKeyOn[KC_F2] = 0;
        if (ingame.Flags & GamF_Unkn0040)
            ingame.Flags &= ~GamF_Unkn0040;
        else
            ingame.Flags |= GamF_Unkn0040;
    }
    if (lbKeyOn[KC_F3] && (lbShift & KMod_CONTROL))
    {
        lbKeyOn[KC_F3] = 0;
        if (ingame.Flags & GamF_Unkn0080)
            ingame.Flags &= ~GamF_Unkn0080;
        else
            ingame.Flags |= GamF_Unkn0080;
    }
    if (lbKeyOn[KC_F4] && (lbShift & KMod_CONTROL))
    {
        lbKeyOn[KC_F4] = 0;
        if (ingame.Flags & GamF_Unkn0002)
            ingame.Flags &= ~GamF_Unkn0002;
        else
            ingame.Flags |= GamF_Unkn0002;
    }
    if (lbKeyOn[KC_F6] && (lbShift & KMod_CONTROL))
    {
        lbKeyOn[KC_F6] = 0;
        if (ingame.Flags & GamF_Unkn0400)
            ingame.Flags &= ~GamF_Unkn0400;
        else
            ingame.Flags |= GamF_Unkn0400;
    }
    if ( lbKeyOn[KC_F10] && (lbShift & KMod_CONTROL))
    {
        lbKeyOn[KC_F10] = 0;
        if (ingame.Flags & GamF_Unkn2000)
            ingame.Flags &= ~GamF_Unkn2000;
        else
            ingame.Flags |= GamF_Unkn2000;
    }

    // TODO Speed control - make it work, maybe?
    if (lbKeyOn[KC_EQUALS])
    {
        lbKeyOn[KC_EQUALS] = 0;
        game_speed -= 2500;
        if (game_speed < 0)
            game_speed = 0;
    }
    if (lbKeyOn[KC_MINUS])
    {
        lbKeyOn[KC_MINUS] = 0;
        game_speed += 2500;
    }
#if 0 // What was that supposed to do?
    for (n = 1; n < game_speed; n++)
    {
        ulong k;

        k = n;
        while (k > 1)
        {
            if (k & 1)
                k = 3 * k + 1;
            else
                k = k >> 1;
        }
    }
#endif

    // Toggle Scanner beep
    if (lbKeyOn[KC_S])
    {
        if (ingame.Flags & GamF_Unkn00200000)
            ingame.Flags &= ~GamF_Unkn00200000;
        else
            ingame.Flags |= GamF_Unkn00200000;
    }

    // Control map area to draw
    if (lbKeyOn[KC_E])
    {
        lbKeyOn[KC_E] = 0;

        if (lbShift & KMod_SHIFT)
            n = -2;
        else
            n = 2;
        render_area_a += n;

        if ( lbShift & KMod_SHIFT )
            n = -2;
        else
            n = 2;
        render_area_b += n;

        if (render_area_a < 2)
            render_area_a = 2;
        if (render_area_b < 2)
            render_area_b = 2;
        if (render_area_a > 28)
            render_area_a = 28;
        if (render_area_b > 28)
            render_area_b = 28;
    }

    // Entering pause screen
    if (!in_network_game)
    {
        if (lbKeyOn[kbkeys[GKey_PAUSE]] ||
            (jskeys[GKey_PAUSE] && jskeys[GKey_PAUSE] == joy.Buttons[0]))
        {
            if (pause_screen_handle()) {
                return -1;
            }
        }
    }

    // Scanner appearence control
    if (lbKeyOn[KC_NUMPAD6])
        ingame.Scanner.Brightness += 4;
    if (lbKeyOn[KC_NUMPAD4])
        ingame.Scanner.Brightness -= 4;
    if (ingame.Scanner.Brightness < 0)
        ingame.Scanner.Brightness = 0;
    if (ingame.Scanner.Brightness > 64)
        ingame.Scanner.Brightness = 64;

    // MIDI Music (tension) volume control
    if (lbKeyOn[KC_NUMPAD8])
    {
        if (lbShift & KMod_SHIFT)
            startscr_midivol += 1;
        else
            startscr_midivol += 10;
        if (startscr_midivol > 322)
            startscr_midivol = 322;
        SetMusicMasterVolume(127 * startscr_midivol / 322);
    }
    if (lbKeyOn[KC_NUMPAD2])
    {
        if (lbShift & KMod_SHIFT)
            startscr_midivol -= 1;
        else
            startscr_midivol -= 10;
        if (startscr_midivol < 0)
            startscr_midivol = 0;
        SetMusicMasterVolume(127 * startscr_midivol / 322);
    }

    // Sample volume control
    if (lbKeyOn[KC_NUMPAD7])
    {
        if (lbShift & KMod_SHIFT)
            startscr_samplevol += 1;
        else
            startscr_samplevol += 10;
        if (startscr_samplevol > 322)
            startscr_samplevol = 322;
        SetSoundMasterVolume(127 * startscr_samplevol / 322);
    }
    if (lbKeyOn[KC_NUMPAD1])
    {
        if (lbShift & KMod_SHIFT)
            startscr_samplevol -= 1;
        else
            startscr_samplevol -= 10;
        if (startscr_samplevol < 0)
            startscr_samplevol = 0;
        SetSoundMasterVolume(127 * startscr_samplevol / 322);
    }

    // CD Music volume control
    if (lbKeyOn[KC_NUMPAD9])
    {
        if (lbShift & KMod_SHIFT)
            startscr_cdvolume += 1;
        else
            startscr_cdvolume += 10;
        if (startscr_cdvolume > 322)
            startscr_cdvolume = 322;
        SetCDVolume(70 * (127 * startscr_cdvolume / 322) / 100);
    }
    if (lbKeyOn[KC_NUMPAD3])
    {
        if (lbShift & KMod_SHIFT)
            startscr_cdvolume -= 1;
        else
            startscr_cdvolume -= 10;
        if (startscr_cdvolume < 0)
            startscr_cdvolume = 0;
        SetCDVolume(70 * (127 * startscr_cdvolume / 322) / 100);
    }

    // Music track control
    if (lbKeyOn[KC_NUMPAD5])
    {
        lbKeyOn[KC_NUMPAD5] = 0;
        if (++ingame.CDTrack > 4)
            ingame.CDTrack = 2;
    }
    if (lbKeyOn[KC_NUMPAD0])
    {
        lbKeyOn[KC_NUMPAD0] = 0;
        ingame.DangerTrack = 2 - ingame.DangerTrack + 1;
    }

    // Restart level
    if (!in_network_game && !(ingame.Flags & GamF_Unkn0010))
    {
        if (lbKeyOn[KC_R])
        {
            lbKeyOn[KC_R] = 0;
            StopCD();
            test_missions(1);
            init_level_3d(1);

            restart_back_into_mission(ingame.CurrentMission);
        }
    }

    if (ingame.TrackThing)
    {
        build_packet(&packets[local_player_no], 0, p_locplayer->DirectControl[0], 0, 0, 0);
        return 1;
    }

    static ushort sel_agent_gkeys[] = {
        GKey_SEL_AGENT_1, GKey_SEL_AGENT_2, GKey_SEL_AGENT_3, GKey_SEL_AGENT_4
    };
    static ulong last_sel_agent_turn[4] = {0};
    for (n = 0; n < (int)(sizeof(sel_agent_gkeys)/sizeof(sel_agent_gkeys[0])); n++)
    {
        ulong gkey = sel_agent_gkeys[n];
        if (lbKeyOn[kbkeys[gkey]] && (lbShift == KMod_NONE))
        {
            struct Thing *p_agent;

            p_agent = p_locplayer->MyAgent[n];
            if (p_agent != NULL)
            {
                if (((p_agent->Flag & TngF_Unkn0002) == 0) && ((p_agent->Flag2 & 0x10) == 0) && (p_agent->State != PerSt_PERSON_BURNING))
                {
                  lbKeyOn[kbkeys[gkey]] = 0;
                  if (p_locplayer->DoubleMode)
                  {
                    byte_153198 = n+1;
                  }
                  else
                  {
                    my_build_packet(&packets[local_player_no], PAct_17,
                        p_locplayer->DirectControl[n], p_agent->ThingOffset, 0, 0);
                    p_locplayer->UserInput[0].ControlMode |= 0x8000u;
                    // Double tapping - center view on the agent
                    if (gameturn - last_sel_agent_turn[n] < 7)
                    {
                      p_agent = p_locplayer->MyAgent[n];
                      ingame.TrackX = p_agent->X >> 8;
                      engn_yc = p_agent->Y >> 8;
                      ingame.TrackZ = p_agent->Z >> 8;
                      build_packet(&packets[local_player_no], PAct_17,
                          p_locplayer->DirectControl[mouser], p_agent->ThingOffset, 0, 0);
                      if (p_agent->ThingOffset == p_locplayer->DirectControl[mouser])
                      {
                          engn_xc = p_agent->X >> 8;
                          engn_zc = p_agent->Z >> 8;
                      }
                    }
                    last_sel_agent_turn[n] = gameturn;
                  }
                  return 1;
                }
            }
        }
    }

    if (ingame.Flags & GamF_Unkn0100)
    {
        build_packet(&packets[local_player_no], 0, p_locplayer->DirectControl[0], 0, 0, 0);
        return 1;
    }

    // Resurrection and bes equipment cheat
    if (p_locplayer->DoubleMode && (ingame.Cheats & 0x04) && !in_network_game)
    {
        if (lbKeyOn[KC_Q] && ((lbShift == KMod_SHIFT) || (lbShift == KMod_NONE)))
        {
            lbKeyOn[KC_Q] = 0;
            beefup_all_agents(p_locplayer);
        }
    }

    // Final part of this function is left in ASM, for now. It requires updating
    // definition of SpecialUserInput for the rewrting to make sense.
    ubyte ret;
    asm volatile ("call ASM_do_user_interface\n"
        : "=r" (ret));
    return ret;
}

void show_main_screen(void)
{
    if ((game_projector_speed && (main_quit_button.Flags & 0x01)) ||
      (lbKeyOn[KC_SPACE] && !edit_flag))
    {
        lbKeyOn[KC_SPACE] = 0;
        main_quit_button.Flags |= 0x0002;
        main_load_button.Flags |= 0x0002;
        main_login_button.Flags |= 0x0002;
        main_map_editor_button.Flags |= 0x0002;
    }
    //main_quit_button.DrawFn(&main_quit_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&main_quit_button), "g" (main_quit_button.DrawFn));
    //main_load_button.DrawFn(&main_load_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&main_load_button), "g" (main_load_button.DrawFn));
    //main_login_button.DrawFn(&main_login_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&main_login_button), "g" (main_login_button.DrawFn));
}

void show_alert_box(void)
{
    asm volatile ("call ASM_show_alert_box\n"
        :  :  : "eax" );
}

void show_type11_screen(void)
{
    asm volatile ("call ASM_show_type11_screen\n"
        :  :  : "eax" );
}

void show_research_screen(void)
{
    asm volatile ("call ASM_show_research_screen\n"
        :  :  : "eax" );
}

void show_netgame_screen(void)
{
    asm volatile ("call ASM_show_netgame_screen\n"
        :  :  : "eax" );
}

void show_equipment_screen(void)
{
    asm volatile ("call ASM_show_equipment_screen\n"
        :  :  : "eax" );
}

void show_cryo_chamber_screen(void)
{
    asm volatile ("call ASM_show_cryo_chamber_screen\n"
        :  :  : "eax" );
}

void show_mission_screen(void)
{
    asm volatile ("call ASM_show_mission_screen\n"
        :  :  : "eax" );
}

void show_menu_screen_st0(void)
{
    debug_trace_place(16);

    lbInkeyToAscii[KC_OEM_102] = '\\';
    lbInkeyToAsciiShift[KC_OEM_102] = '|';

    players_init_control_mode();

    login_control__State = 6;
    sprintf(net_unkn2_text, "01234567890");

    {
        long pos = 0;
        hotspot_buffer = (struct StartScreenPoint *)((ubyte *)scratch_malloc_mem + pos);
        pos += hotspot_buffer_len;

        mission_briefing_text = (char *)scratch_malloc_mem + pos;
        pos += mission_briefing_text_len;

        netscan_text = (char *)scratch_malloc_mem + pos;
        pos += netscan_text_len;

        weapon_text = (char *)scratch_malloc_mem + pos;
        pos += weapon_text_len;

        memload = (ubyte *)scratch_malloc_mem + pos;
        pos += memload_len;

        purple_draw_list = (struct PurpleDrawItem *)((ubyte *)scratch_malloc_mem + pos);
    }

    init_read_all_sprite_files();
    ingame.Credits = 50000;

    debug_trace_place(17);
    LbColourTablesLoad(display_palette, "data/bgtables.dat");
    LbGhostTableGenerate(display_palette, 66, "data/startgho.dat");
    init_screen_boxes();
    update_menus();
    players[local_player_no].MissionAgents = 0x0f;
    load_city_data(0);
    load_city_txt();

    debug_trace_place(18);
    if ( in_network_game )
        screentype = SCRT_PAUSE;
    else
        screentype = SCRT_MAINMENU;
    data_1c498d = 1;

    debug_trace_place(19);
    LbFileLoadAt("data/s-proj.pal", display_palette);
    show_black_screen();
    show_black_screen();
    LbPaletteSet(display_palette);
    reload_background();

    global_date.Day = 2;
    global_date.Year = 74;
    global_date.Month = 6;

    srm_scanner_reset();

    save_game_buffer = unkn_buffer_05;

    net_system_init0();
}

void init_net_players(void)
{
    int i;
    for (i = 0; i < 5; i++) {
        LbMemorySet(&net_players[i], '\0', sizeof(struct NetPlayer2));
    }
}

void delete_mail(ushort mailnum, ubyte type)
{
    asm volatile ("call ASM_delete_mail\n"
        : : "a" (mailnum), "d" (type));
}

void draw_flic_purple_list(void (*fn)())
{
    asm volatile ("call ASM_draw_flic_purple_list\n"
        : : "a" (fn));
}

void update_mission_time(char a1)
{
    asm volatile ("call ASM_update_mission_time\n"
        : : "a" (a1));
}

void show_date_time(void)
{
    asm volatile ("call ASM_show_date_time\n"
        :  :  : "eax" );
}

void purple_unkn1_data_to_screen(void)
{
    memcpy(data_1c6de4, data_1c6de8, 0x5FA0u);
}

void research_unkn_func_002(void)
{
    asm volatile ("call ASM_research_unkn_func_002\n"
        :  :  : "eax" );
}

void unkn_research_func_006(void)
{
    asm volatile ("call ASM_unkn_research_func_006\n"
        :  :  : "eax" );
}

void net_unkn_func_29(short a1, short a2, ubyte a3, sbyte a4, ubyte a5)
{
    asm volatile (
      "push %4\n"
      "call ASM_net_unkn_func_29\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5));
}

void update_flic_mods(ubyte *mods)
{
    asm volatile ("call ASM_update_flic_mods\n"
        : : "a" (mods));
}

void net_new_game_prepare(void)
{
    switch_net_screen_boxes_to_initiate();
    login_control__State = 6;
    byte_15516D = -1;
    byte_15516C = -1;
    ingame.Credits = 50000;
    ingame.CashAtStart = 50000;
    login_control__TechLevel = 4;
    unkn_city_no = -1;
    login_control__City = -1;
    ingame.Expenditure = 0;
    unkn_flags_08 = 60;
    login_control__Money = starting_cash_amounts[0];
    init_agents();
    load_missions(background_type);
    load_objectives_text();
    srm_reset_research();
    init_net_players();
    draw_flic_purple_list(purple_unkn1_data_to_screen);
}


void agents_copy_fourpacks_netplayer_to_player(int plyr, struct NetworkPlayer *p_netplyr)
{
    short plagent, fp;

    for (plagent = 0; plagent < 4; plagent++)
    {
        for (fp = 0; fp < WFRPK_COUNT; fp++) {
            players[plyr].FourPacks[plagent][fp] = \
              p_netplyr->U.FourPacks.FourPacks[plagent][fp];
        }
    }
}

void agents_copy_fourpacks_netplayer_to_cryo(struct NetworkPlayer *p_netplyr)
{
    short plagent, fp;

    for (plagent = 0; plagent < 4; plagent++)
    {
        for (fp = 0; fp < WFRPK_COUNT; fp++) {
            cryo_agents.FourPacks[plagent].Amount[fp] = \
              p_netplyr->U.FourPacks.FourPacks[plagent][fp];
        }
    }
}

void net_unkn_func_33_sub1(int plyr, int netplyr)
{
    struct NetworkPlayer *p_netplyr;
    ushort nptype;
    int i;

    p_netplyr = &network_players[plyr];
    if (p_netplyr->Type == 17)
        return;

    nptype = p_netplyr->Type & 0x1F;
    if (nptype != 10 && nptype != 14 && nptype != 15)
    {
        group_types[plyr] = p_netplyr->U.Progress.val_181183;
        byte_1C5C28[plyr] = p_netplyr->U.Progress.val_181189;
        if (net_host_player_no == plyr)
        {
            if ((unkn_flags_08 & 0x02) == 0)
                login_control__TechLevel = p_netplyr->U.Progress.TechLevel;
            if ((unkn_flags_08 & 0x01) == 0) {
                login_control__Money = abs(p_netplyr->U.Progress.Credits);
                ingame.Credits = login_control__Money;
                ingame.CashAtStart = login_control__Money;
            }
            if ((unkn_flags_08 & 0x08) != 0)
            {
                long credits;

              credits = p_netplyr->U.Progress.Credits;
              if (credits >= 0) {
                  login_control__Money = credits;
                  ingame.CashAtStart = credits;
              } else {
                  ingame.Credits = -credits;
              }
              ingame.Expenditure = p_netplyr->U.Progress.Expenditure;
            }
        }
        for ( i = 0; i < 4; i++)
        {
            players[plyr].UserInput[i].ControlMode =
              network_players[0].U.Progress.ControlMode[i];
        }
        players[plyr].DoubleMode = p_netplyr->U.Progress.DoubleMode;
    }

    switch (p_netplyr->Type & 0x1F)
    {
    case 2:
        login_control__State = 8;
        LbNetworkShutDownListeners();
        LbMemorySet(unkstruct04_arr, 0, 20 * sizeof(struct TbNetworkSessionList));
        byte_1C6D48 = 0;
        break;
    case 3:
        draw_flic_purple_list(purple_unkn1_data_to_screen);
        break;
    case 4:
        login_control__TechLevel = p_netplyr->U.Progress.TechLevel;
        break;
    case 5:
        i = p_netplyr->U.Progress.Credits;
        ingame.Expenditure = 0;
        login_control__Money = i;
        ingame.Credits = i;
        ingame.CashAtStart = i;
        break;
    case 6:
        refresh_equip_list = 1;
        unkn_flags_08 = p_netplyr->U.Progress.val_flags_08;
        break;
    case 9:
        login_control__City = p_netplyr->U.Progress.SelectedCity;
        break;
    case 10:
        // Free last net_players[] slot
        {
            struct NetPlayer2 *p_nplyr1;
            struct NetPlayer2 *p_nplyr2;
            p_nplyr2 = &net_players[1];
            p_nplyr1 = &net_players[0];
            for (i = 0; i < 3; i++)
            {
                byte_1C6DDC[i] = byte_1C6DDC[i+1];
                strcpy(p_nplyr1->field_0, p_nplyr2->field_0);
                p_nplyr1++;
                p_nplyr2++;
            }
        }
        // Fill the slot from packet
        {
            struct NetPlayer2 *p_nplyr1;
            const char *p_text;
            byte_1C6DDC[4] = plyr;
            p_nplyr1 = &net_players[4];
            p_text = p_netplyr->U.Text;
            strcpy(p_nplyr1->field_0, p_text);
        }
        break;
    case 11:
        net_unkn_func_29(
          p_netplyr->U.Progress.npfield_8,
          p_netplyr->U.Progress.npfield_A,
          p_netplyr->U.Progress.npfield_12,
            1, i);
        break;
    case 12:
        byte_15516D = -1;
        reset_net_screen_EJECT_flags();
        LbNetworkSessionStop();
        if (word_1811AE == 1)
        {
            if (p_netplyr->U.Progress.val_15516D == netplyr)
            {
                net_new_game_prepare();
                if (screentype == 4)
                {
                    update_flic_mods(flic_mods);
                    for (i = 0; i < 4; i++) {
                        if (flic_mods[i] != old_flic_mods[i])
                            mod_draw_states[i] |= 0x08;
                    }
                }
            }
        } else {
            if (p_netplyr->U.Progress.val_15516D != netplyr)
                LbNetworkSessionStop();
            net_new_game_prepare();
            if (byte_1C4A6F)
                LbNetworkHangUp();
            LbNetworkReset();
            byte_1C4A7C = 0;
        }
        break;
    case 13:
        LbNetworkSessionStop();
        if (word_1811AE == 1)
        {
            if (plyr == netplyr || net_host_player_no == plyr)
            {
                net_new_game_prepare();
                memset(unkstruct04_arr, 0, 0x1108u);
                byte_1C6D48 = 0;
                for (i = 0; i < 8; i++) {
                    unkn2_names[i][0] = '\0';
                }
            }
        }
        else
        {
            net_new_game_prepare();
            for (i = 0; i < 8; i++) {
                unkn2_names[i][0] = '\0';
            }
            if ( byte_1C4A6F )
              LbNetworkHangUp();
            LbNetworkReset();
            byte_1C4A7C = 0;
        }
        if (screentype == 4)
        {
            update_flic_mods(flic_mods);
            for (i = 0; i < 4; i++) {
                if (flic_mods[i] != old_flic_mods[i])
                    mod_draw_states[i] |= 0x08;
            }
        }
        break;
    case 14:
        if ((net_host_player_no == plyr) && ((unkn_flags_08 & 0x08) != 0))
        {
            for (i = 0; i < 8; i++)
            {
                int k;
                if (unkn2_names[i][0] == '\0')
                    continue;
                for (k = 0; k < 4; k++) {
                    players[i].Weapons[k] = p_netplyr->U.WepMod.Weapons[k];
                    players[i].Mods[k] = p_netplyr->U.WepMod.Mods[k];
                }
            }
            {
                int k;
                for (k = 0; k < 4; k++) {
                    cryo_agents.Weapons[k] = p_netplyr->U.WepMod.Weapons[k];
                    cryo_agents.Mods[k] = p_netplyr->U.WepMod.Mods[k];
                }
            }
            if (net_host_player_no != netplyr)
            {
                update_flic_mods(flic_mods);
                for (i = 0; i < 4; i++) {
                    if (flic_mods[i] != old_flic_mods[i])
                        mod_draw_states[i] |= 0x08;
                }
            }
        }
        else if ((unkn_flags_08 & 0x08) == 0)
        {
            for (i = 0; i != 4; i++) {
                players[plyr].Weapons[i] = p_netplyr->U.WepMod.Weapons[i];
                players[plyr].Mods[i] = p_netplyr->U.WepMod.Mods[i];
            }
        }
        break;
    case 15:
        if ((net_host_player_no == plyr) && ((unkn_flags_08 & 0x08) != 0))
        {
            for (i = 0; i < 8; i++)
            {
                if (unkn2_names[i][0] == '\0')
                    continue;
                agents_copy_fourpacks_netplayer_to_player(i, p_netplyr);
            }
            agents_copy_fourpacks_netplayer_to_cryo(p_netplyr);
        }
        else if ((unkn_flags_08 & 0x08) == 0)
        {
            agents_copy_fourpacks_netplayer_to_player(plyr, p_netplyr);
        }
        break;
    case 16:
        net_unkn_func_29(
          p_netplyr->U.Progress.npfield_8,
          p_netplyr->U.Progress.npfield_A,
          p_netplyr->U.Progress.npfield_12,
          0, i);
        break;
    case 18:
        net_unkn_func_29(
          p_netplyr->U.Progress.npfield_8,
          p_netplyr->U.Progress.npfield_A,
          p_netplyr->U.Progress.npfield_12,
          2, i);
        break;
    default:
        break;
    }
}

void net_unkn_func_33(void)
{
#if 0
    asm volatile ("call ASM_net_unkn_func_33\n"
        :  :  : "eax" );
#else
    struct NetworkPlayer *p_netplyr;
    int player;
    int i;

    player = LbNetworkPlayerNumber();
    net_players_num = LbNetworkSessionNumberPlayers();
    net_host_player_no = LbNetworkHostPlayerNumber();
    p_netplyr = &network_players[player];

    switch (p_netplyr->Type)
    {
    case 14:
        for (i = 0; i < 4; i++)
        {
            p_netplyr->U.WepMod.Weapons[i] = cryo_agents.Weapons[i];
            p_netplyr->U.WepMod.Mods[i] = cryo_agents.Mods[i];
        }
        break;
    case 15:
        for (i = 0; i < 4; i++)
        {
            int k;
            for (k = 0; k < 5; k++) {
                p_netplyr->U.FourPacks.FourPacks[i][k] =
                  cryo_agents.FourPacks[i].Amount[k];
            }
        }
        break;
    case 10:
        break;
    case 17:
        p_netplyr->Type = 0;
        // Fall through
    default:
        p_netplyr->U.Progress.SelectedCity = login_control__City;
        if (((gameturn & 1) == 0) && (unkn_flags_08 & 8))
            p_netplyr->U.Progress.Credits = -ingame.Credits;
        else
            p_netplyr->U.Progress.Credits = login_control__Money;
        p_netplyr->U.Progress.TechLevel = login_control__TechLevel;
        p_netplyr->U.Progress.val_flags_08 = unkn_flags_08;
        p_netplyr->U.Progress.val_181189 = byte_181189;
        p_netplyr->U.Progress.val_181183 = byte_181183;
        p_netplyr->U.Progress.val_15516D = byte_15516D;
        p_netplyr->U.Progress.Expenditure = ingame.Expenditure;

        for (i = 0; i < 4; i++)
        {
            p_netplyr->U.Progress.ControlMode[i] =
              players[player].UserInput[i].ControlMode;
        }
        p_netplyr->U.Progress.DoubleMode = players[player].DoubleMode;
        break;
    }

    // TODO VERIFY why are we exchanging first packet, not the one we filled?
    if (LbNetworkExchange(&network_players[0], sizeof(struct NetworkPlayer)) != 1)
    {
        LbNetworkSessionStop();
        net_new_game_prepare();
        if (word_1811AE != 1)
        {
            if (byte_1C4A6F)
                LbNetworkHangUp();
            LbNetworkReset();
            byte_1C4A7C = 0;
        }
    }

    for (i = 0; i < 8; i++)
    {
        net_unkn_func_33_sub1(i, player);
    }

    if (byte_1C6D4A)
    {
        p_netplyr = &network_players[net_host_player_no];
        if (p_netplyr->Type != 10 && p_netplyr->Type != 14 && p_netplyr->Type != 15)
        {
            login_control__TechLevel = p_netplyr->U.Progress.TechLevel;
            unkn_flags_08 = p_netplyr->U.Progress.val_flags_08;
            login_control__City = p_netplyr->U.Progress.SelectedCity;
            ingame.Expenditure = p_netplyr->U.Progress.Expenditure;
            login_control__Money = abs(p_netplyr->U.Progress.Credits);
            ingame.Credits = login_control__Money;
            ingame.CashAtStart = login_control__Money;
        }
        byte_1C6D4A = 0;
    }

    for (i = 0; i < 8; i++)
    {
        network_players[i].Type = 17;
    }
#endif
}

void forward_research_progress(int num_days)
{
    new_mods_researched = 0;
    new_weapons_researched = 0;
    int i;
    for (i = 0; i < num_days; i++)
    {
        int prev;

        prev = research.CurrentWeapon;
        scientists_lost = research_daily_progress_for_type(0);
        if (research.CurrentWeapon != prev)
            new_weapons_researched |= 1 << prev;

        prev = research.CurrentMod;
        scientists_lost += research_daily_progress_for_type(1);
        if (research.CurrentMod != prev)
            new_mods_researched |= 1 << prev;
    }
    research_unkn_func_002();
}

void draw_purple_screen(void)
{
    asm volatile ("call ASM_draw_purple_screen\n"
        :  :  : "eax" );
}

void clear_mission_status(ulong id)
{
    mission_status[id].CivsKilled = 0;
    mission_status[id].EnemiesKilled = 0;
    mission_status[id].CivsPersuaded = 0;
    mission_status[id].SecurityPersuaded = 0;
    mission_status[id].EnemiesPersuaded = 0;
    mission_status[id].AgentsGained = 0;
    mission_status[id].AgentsLost = 0;
    mission_status[id].SecurityKilled = 0;
    mission_status[id].CityDays = 0;
    mission_status[id].CityHours = 0;
}

void show_menu_screen_st2(void)
{
    if ( in_network_game )
    {
        local_player_no = 0;
        net_new_game_prepare();
        memset(unkstruct04_arr, 0, 20 * sizeof(struct TbNetworkSessionList)); //clear 4360 bytes
        byte_1C6D48 = 0;
        selected_mod = -1;
        selected_weapon = -1;
        scientists_lost = 0;
        update_mission_time(0);
        in_network_game = 0;
        screentype = SCRT_B;
        redraw_screen_flag = 1;
        heading_box.Text = gui_strings[374];
    }
    else
    {
      update_mission_time(0);
      city_id = -1;
      byte_1C4AA3 = brief_store[open_brief - 1].RefNum;
      if ((ingame.MissionStatus != 0) && (ingame.MissionStatus != 2))
      {
            memcpy(&mission_status[0], &mission_status[open_brief],
              sizeof(struct MissionStatus));
            delete_mail(open_brief - 1, 1u);
            open_brief = 0;
            old_mission_brief = 0;
            cities[unkn_city_no].Info = 0;
      }
      else
      {
            old_mission_brief = open_brief;
      }
      if (ingame.GameOver)
      {
            screentype = SCRT_MAINMENU;
            if (ingame.Flags & GamF_Unkn0010) {
                char fname[52];
                get_saved_game_fname(fname, 0);
                LbFileDelete(fname);
            }
            ingame.GameOver = 0;
      }
      else
      {
            forward_research_progress(mission_status[open_brief].CityDays);
            if (ingame.Flags & GamF_Unkn0010)
                save_game_write(0, save_active_desc);
            screentype = SCRT_9;
            heading_box.Text = gui_strings[374];
            redraw_screen_flag = 1;
      }
    }

    LbColourTablesLoad(display_palette, "data/bgtables.dat");
    LbGhostTableGenerate(display_palette, 66, "data/startgho.dat");
    init_read_all_sprite_files();
    init_weapon_text();
    load_city_txt();
    data_1c498d = 1;
    LbMouseChangeSpriteOffset(0, 0);
    LbFileLoadAt("data/s-proj.pal", display_palette);

    update_options_screen_state();

    show_black_screen();
    show_black_screen();
    LbPaletteSet(display_palette);
    reload_background();

    srm_scanner_reset();

    if (new_mail)
        play_sample_using_heap(0, 119 + (LbRandomAnyShort() % 3), 127, 64, 100, 0, 3u);

    net_system_init2();
}

void init_random_seed(void)
{
    if (in_network_game)
    {
        struct NetworkPlayer *p_netplyr;

        mission_open[1] = ingame.CurrentMission;
        mission_state[1] = 0;
        mission_open[2] = 0;
        mission_state[2] = 0;

        p_netplyr = &network_players[net_host_player_no];
        if (is_unkn_current_player())
        {
            lbSeed = time(0);
            p_netplyr->U.RandInit.Seed = lbSeed;
            LbNetworkExchange(network_players, sizeof(struct NetworkPlayer));
        } else {
            LbNetworkExchange(network_players, sizeof(struct NetworkPlayer));
            lbSeed = p_netplyr->U.RandInit.Seed;
        }
    }
    else
    {
        lbSeed = time(0);
    }
    srand(lbSeed);
}

ushort find_mission_with_mapid(short mapID, short mission_limit)
{
    ushort i;
    for (i = 1; i < mission_limit; i++)
    {
        if (mission_list[i].MapNo == mapID) {
            return i;
        }
    }
    return 0;
}

/** Searches for mission taking place in given city, within mission chain specified by the brief.
 */
ushort find_mission_for_city_in_brief(short brief, sbyte city_no)
{
    ushort missi;

    for (missi = brief_store[brief].Mission; missi != 0;
      missi = mission_list[missi].SpecialTrigger[0])
    {
        if (mission_list[missi].MapNo == cities[city_no].MapID)
            break;
    }
    return missi;
}

void update_open_brief(void)
{
    short brief;
    open_brief = 0;
    for (brief = 0; brief < next_brief; brief++)
    {
        ushort missi;

        missi = find_mission_for_city_in_brief(brief, unkn_city_no);
        if (missi != 0) {
            open_brief = brief + 1;
            break;
        }
    }
}

/** Macro for returning given array of elements in random order.
 *
 * Provided input and output arrays, type of element and count of elements,
 * this routine will randomize the order of elements.
 */
#define array_elements_in_random_order(out_nubers, in_nubers, elem_type, count) \
{ \
    ushort pos, remain, next; \
    elem_type nums[count]; \
    memcpy(nums, in_nubers, sizeof(elem_type)*count); \
    remain = count; \
    for (pos = 0; pos < count; pos++) \
    { \
        elem_type nbak; \
        next = LbRandomAnyShort() % remain; \
        remain--; \
        out_nubers[pos] = nums[next]; \
        nbak = nums[remain]; \
        nums[remain] = nums[next]; \
        nums[next] = nbak; \
    } \
}

void randomize_playable_groups_order(void)
{
    static long incrementing_nubers[] = {0, 1, 2, 3, 4, 5, 6, 7,};
    array_elements_in_random_order(level_def.PlayableGroups, incrementing_nubers, long, 8);
}

void wait_for_sound_sample_finish(ushort smpl_id)
{
    TbClockMSec last_loop_time = LbTimerClock();
    while (IsSamplePlaying(0, smpl_id, NULL)) {
        game_update();
        TbClockMSec sleep_end = last_loop_time + 1000/GAME_FPS;
        LbSleepUntil(sleep_end);
        last_loop_time = LbTimerClock();
    }
}

void copy_to_screen_ani(ubyte *buf)
{
    int y;
    const ubyte *inp;
    ubyte *o;

    inp = buf;
    o = lbDisplay.WScreen;
    for (y = 0; y < 256; y++)
    {
        memcpy(o, inp, 256);
        inp += 256;
        o += lbDisplay.GraphicsScreenWidth;
    }
}

void frame_unkn_func_06(void)
{
    asm volatile ("call ASM_frame_unkn_func_06\n"
        :  :  : "eax" );
}

void show_mission_loading_screen(void)
{
    LbMouseChangeSprite(0);
    reload_background();
    play_sample_using_heap(0, 118, 127, 64, 100, 0, 3);

    DwBool stop;
    TbClockMSec last_loop_time = LbTimerClock();
    do
    {
        memcpy(lbDisplay.WScreen, back_buffer, lbDisplay.GraphicsScreenWidth * lbDisplay.GraphicsScreenHeight);
        text_buf_pos = lbDisplay.GraphicsScreenWidth * lbDisplay.GraphicsScreenHeight;
        if ((0 != game_projector_speed && (loading_INITIATING_box.Flags & 0x0001))
          || (0 != lbKeyOn[KC_SPACE] && 0 == edit_flag))
            loading_INITIATING_box.Flags |= 0x0002;
        //loading_INITIATING_box.DrawFn(&loading_INITIATING_box); -- incompatible calling convention
        asm volatile ("call *%1\n"
            : : "a" (&loading_INITIATING_box), "g" (loading_INITIATING_box.DrawFn));
        stop = loading_INITIATING_box.Flags & 0x1000;
        draw_purple_screen();
        swap_wscreen();

        game_update();
        TbClockMSec sleep_end = last_loop_time + 1000/GAME_FPS;
        LbSleepUntil(sleep_end);
        last_loop_time = LbTimerClock();
    }
    while (!stop);

    loading_INITIATING_box.Flags = 0x0001;
    wait_for_sound_sample_finish(118);
}

void show_load_and_prep_mission(void)
{
    init_random_seed();
    show_mission_loading_screen();

    if ( start_into_mission )
    {
        ushort missi, next_mapno;
        short next_level;

        if (!in_network_game) {
            update_open_brief();
        }

        ingame.DisplayMode = DpM_UNKN_32;
        debug_trace_place(6);
        if ( in_network_game )
        {
            ingame.MissionNo = 1;
            missi = find_mission_with_mapid(cities[login_control__City].MapID, next_mission);
            if (missi > 0) {
                ingame.MissionNo = missi;
                ingame.CurrentMission = missi;
            }

            missi = ingame.MissionNo;
            next_mapno = mission_list[missi].MapNo;
            next_level = mission_list[missi].LevelNo;
        }
        else
        {
            missi = find_mission_for_city_in_brief(open_brief - 1, unkn_city_no);
            next_mapno = cities[unkn_city_no].MapID;
            next_level = cities[unkn_city_no].Level;
            load_mission_name_text(missi);
        }
        LOGSYNC("Init %s mission %hu on map %hu level %hd", in_network_game ? "MP" : "SP",
          missi, next_mapno, next_level);
        change_current_map(next_mapno);
        debug_trace_place(7);
        // The file name is formatted in original code, but doesn't seem to be used
        //sprintf(fname, "maps/map%03d.scn", next_mapno);
        ingame.fld_unkC4F = 0;
        data_19ec6f = 1;
        execute_commands = 1;
        if (next_level != 0)
            load_level_pc(-next_level, missi, 0);
        else
            LOGWARN("Requested level %hd; load skipped", next_level);
        if (in_network_game) {
            randomize_playable_groups_order();
        }
        debug_trace_place(8);
    }
    else
    {
        LbMouseChangeSprite(&pointer_sprites[1]);
        ingame.DisplayMode = DpM_UNKN_1;
    }

    debug_trace_place(9);
    data_1c498d = 2;
    reload_background_flag = 1;
    debug_trace_place(10);
    {
        screen_buffer_fill_black();
        frame_unkn_func_06();
        screen_buffer_fill_black();
        show_black_screen();
        swap_wscreen();
    }
    LbFileLoadAt("qdata/pal.pal", display_palette);
    LbPaletteSet(display_palette);
    debug_trace_place(11);
    if (game_high_resolution)
    {
        if (lbDisplay.ScreenMode != screen_mode_game_hi)
            setup_screen_mode(screen_mode_game_hi);
    }
    else
    {
        if (lbDisplay.ScreenMode != screen_mode_game_lo)
            setup_screen_mode(screen_mode_game_lo);
    }

    debug_trace_place(12);
    LbColourTablesLoad(display_palette, "data/tables.dat");
    LbGhostTableLoad(display_palette, 50, "data/synghost.tab");
    debug_trace_place(13);

    // Update game progress and prepare level to play
    if ( start_into_mission )
    {
        if ( in_network_game )
        {
            update_mission_time(1);
            // why clear only 0x140 bytes?? the array is much larger
            memset(mission_status, 0, sizeof(struct MissionStatus) * 8);
            gameturn = 0;
        }
        else
        {
            int i;
            for (i = 0; i < 8; i++) {
                unkn2_names[i][0] = 0;
            }
            strncpy(unkn2_names[0], login_name, 16);

            clear_mission_status(open_brief);
            update_mission_time(1);
            cities[unkn_city_no].Info = 0;
            debug_trace_place(14);

            i = find_mission_for_city_in_brief(open_brief - 1, unkn_city_no);
            ingame.CurrentMission = i;
            mission_result = 0;
            debug_trace_place(15);
        }
        init_player();
    }

    // Set up remaining graphics data and controls
    if ( start_into_mission )
    {
        load_multicolor_sprites();
        adjust_mission_engine_to_video_mode();

        if (ingame.GameMode == GamM_None)
            ingame.GameMode = GamM_Unkn2;

        flic_unkn03(1);
        func_6edb8(1);

        if ( in_network_game )
        {
            if (word_1811AE != 1)
                ingame.InNetGame_UNSURE = 3;
            ingame.DetailLevel = 0;
            bang_set_detail(ingame.DetailLevel == 0);
        }

        lbDisplay.MLeftButton = 0;
        lbDisplay.LeftButton = 0;
    }

    map_editor = 0;
    start_into_mission = 0;
    reload_background_flag = 0;
    net_system_reset();
    stop_sample_using_heap(0, 122);
}

void setup_menu_screen_mode(void)
{
    TbScreenModeInfo *mdinfo;

    game_high_resolution = 0;
    LbMouseReset();
    screen_buffer_fill_black();
    mdinfo = LbScreenGetModeInfo(screen_mode_menu);
    if (mdinfo->Width == 0) return;
    LbScreenSetup(screen_mode_menu, mdinfo->Width, mdinfo->Height, display_palette);
    LbMouseSetup(&pointer_sprites[1], 1, 1);
    setup_vecs(lbDisplay.WScreen, vec_tmap, lbDisplay.PhysicalScreenWidth,
        lbDisplay.PhysicalScreenWidth, lbDisplay.PhysicalScreenHeight);
}

void show_menu_screen(void)
{
    switch (data_1c498d)
    {
    case 2:
        show_menu_screen_st2();
        play_sample_using_heap(0, 122, 127, 64, 100, -1, 3);
        break;
    case 0:
        show_menu_screen_st0();
        play_sample_using_heap(0, 122, 127, 64, 100, -1, 3);
        break;
    default:
        break;
    }
    if (lbDisplay.ScreenMode != screen_mode_menu)
    {
        setup_menu_screen_mode();
        reload_background();
        my_set_text_window(0, 0, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);
    }

    if (screentype == SCRT_MAINMENU)
    {
        replay_intro_timer++;
        if (replay_intro_timer > 1100)
        {
            replay_intro();
            replay_intro_timer = 0;
            data_1c498d = 0;
            return;
          }
    }

    if (screentype != SCRT_MAINMENU) {
        replay_intro_timer = 0;
    }
    text_buf_pos = lbDisplay.GraphicsScreenWidth * lbDisplay.GraphicsScreenHeight;

    if ( !joy.Buttons[0] || net_unkn_pos_02 )
    {
        if ( data_1c4991 )
            data_1c4991 = 0;
    }
    else if ( !data_1c4991 )
    {
        int i;
        data_1c4991 = 1;
        lbDisplay.LeftButton = 1;
        // Scale mouse position in high resolutions
        if (lbDisplay.GraphicsScreenWidth > 320)
            i = lbDisplay.MMouseX * lbDisplay.GraphicsScreenWidth / 320;
        else
            i = lbDisplay.MMouseX;
        lbDisplay.MouseX = i;
        if (lbDisplay.GraphicsScreenHeight > 200)
            i = lbDisplay.MMouseY * lbDisplay.GraphicsScreenHeight / 200;
        else
            i = lbDisplay.MMouseY;
        lbDisplay.MouseY = i;
    }

    data_1c498f = lbDisplay.LeftButton;
    data_1c4990 = lbDisplay.RightButton;
    show_date_time();
    if ((screentype != SCRT_MAINMENU) && (screentype != SCRT_PAUSE) && !restore_savegame)
          unkn_research_func_006();
    if ((screentype == SCRT_9 || screentype == SCRT_B) && change_screen == 7)
    {
        screentype = SCRT_MISSION;
        brief_load_mission_info();
        redraw_screen_flag = 1;
        edit_flag = 0;
        change_screen = 0;
    }

    switch (screentype)
    {
    case SCRT_MISSION:
        show_mission_screen();
        break;
    case SCRT_WORLDMAP:
        show_worldmap_screen();
        break;
    case SCRT_CRYO:
        show_cryo_chamber_screen();
        break;
    case SCRT_EQUIP:
        show_equipment_screen();
        break;
    case SCRT_MAINMENU:
        show_main_screen();
        break;
    case SCRT_NETGAME:
        show_netgame_screen();
        break;
    case SCRT_RESEARCH:
        show_research_screen();
        break;
    case SCRT_9:
        show_type11_screen();
        break;
    case SCRT_PAUSE:
        show_login_screen();
        break;
    case SCRT_B:
        show_type11_screen();
        break;
    case SCRT_ALERTBOX:
        show_alert_box();
        break;
    }

    if (login_control__State == 5)
    {
        net_unkn_func_33();
    }
    else if (login_control__State == 8)
    {
        start_into_mission = 1;
        in_network_game = 1;
        redraw_screen_flag = 1;
        local_player_no = LbNetworkPlayerNumber();
        net_players_num = LbNetworkSessionNumberPlayers();
        switch_net_screen_boxes_to_initiate();
        network_players[local_player_no].Type = 14;
        net_unkn_func_33();
        network_players[local_player_no].Type = 15;
        net_unkn_func_33();
        init_net_players();
    }
    if (data_1c498f && lbDisplay.LeftButton)
    {
        data_1c498f = 0;
        lbDisplay.LeftButton = 0;
    }
    if (data_1c4990 && lbDisplay.RightButton)
    {
        data_1c4990 = 0;
        lbDisplay.RightButton = 0;
    }
    memcpy(lbDisplay.WScreen, back_buffer, lbDisplay.GraphicsScreenWidth * lbDisplay.GraphicsScreenHeight);
    draw_purple_screen();

    if ( screentype != SCRT_MAINMENU && screentype != SCRT_PAUSE && !restore_savegame )
    {
        if ( lbKeyOn[KC_F1] && screentype != SCRT_ALERTBOX && !net_unkn_pos_01b)
        {
            lbKeyOn[KC_F1] = 0;
            change_screen = 1;
        }
        if ( lbKeyOn[KC_F2] && screentype != SCRT_ALERTBOX && !net_unkn_pos_01b)
        {
            lbKeyOn[KC_F2] = 0;
            change_screen = 3;
        }
        if ( lbKeyOn[KC_F3] && screentype != SCRT_ALERTBOX && !net_unkn_pos_01b)
        {
            lbKeyOn[KC_F3] = 0;
            change_screen = 4;
        }
        if ( lbKeyOn[KC_F4] && screentype != SCRT_ALERTBOX && !net_unkn_pos_01b)
        {
            lbKeyOn[KC_F4] = 0;
            change_screen = 5;
        }
        if ( lbKeyOn[KC_F5] && screentype != SCRT_ALERTBOX && !net_unkn_pos_01b)
        {
            lbKeyOn[KC_F5] = 0;
            if (research.NumBases > 0)
                change_screen = 6;
        }
        if ( lbKeyOn[KC_F6] && screentype != SCRT_ALERTBOX && !net_unkn_pos_01b)
        {
            lbKeyOn[KC_F6] = 0;
            if (open_brief != 0)
                change_screen = 7;
        }
    }
    if (change_screen == 1)
    {
        screentype = SCRT_NETGAME;
        redraw_screen_flag = 1;
        heading_box.Text = "";
        edit_flag = 0;
        change_screen = 0;
    }
    if (change_screen == 2)
    {
        screentype = SCRT_2;
        redraw_screen_flag = 1;
        heading_box.Text = gui_strings[367];
        edit_flag = 0;
        change_screen = 0;
    }
    if (change_screen == 3)
    {
        heading_box.Text = gui_strings[368];
        redraw_screen_flag = 1;
        edit_flag = 0;
        change_screen = 0;
        screentype = SCRT_WORLDMAP;
        if (city_id != -1)
          unkn_city_no = city_id;
    }
    if (change_screen == 4)
    {
        screentype = SCRT_CRYO;
        switch_shared_equip_screen_buttons_to_cybmod();

        update_cybmod_cost_text();
        redraw_screen_flag = 1;
        int i;
        for (i = 0; i < 4; i++)
        {
            mod_draw_states[i] = 0;
            if (0 != flic_mods[i])
                mod_draw_states[i] = 8;
        }
        current_drawing_mod = 0;
        new_current_drawing_mod = 0;
        edit_flag = 0;
        change_screen = 0;
    }
    if (change_screen == 5)
    {
        screentype = SCRT_EQUIP;
        switch_shared_equip_screen_buttons_to_equip();
        update_equip_cost_text();
        redraw_screen_flag = 1;
        edit_flag = 0;
        change_screen = 0;
    }
    if (change_screen == 6)
    {
        screentype = SCRT_RESEARCH;
        heading_box.Text = gui_strings[371];
        clear_research_screen();
        edit_flag = 0;
        change_screen = 0;
        redraw_screen_flag = 1;
    }
    if (change_screen == 7)
    {
        city_id = -1;
        screentype = SCRT_MISSION;
        brief_load_mission_info();
        redraw_screen_flag = 1;
        edit_flag = 0;
        change_screen = 0;
    }
    if (show_alert)
    {
        alert_box.Flags = 1;
        edit_flag = 0;
        old_screentype = screentype;
        show_alert = 0;
        screentype = SCRT_ALERTBOX;
        alert_OK_button.Flags |= 0x0001;
        redraw_screen_flag = 1;
    }

    if (redraw_screen_flag && !edit_flag)
    {
        mo_weapon = -1;
        redraw_screen_flag = 0;
        reload_background_flag = 1;
        if (screentype == SCRT_WORLDMAP)
        {
            open_brief = old_mission_brief;
            activate_cities(0);
        }
        else if (screentype == SCRT_MISSION)
        {
            activate_cities(open_brief);
        }

        reset_brief_screen_boxes_flags();

        heading_box.Flags = 0x0001;
        set_flag01_debrief_screen_boxes();

        reset_net_screen_boxes_flags();
        reset_world_screen_boxes_flags();
        reset_login_screen_boxes_flags();
        reset_controls_screen_boxes_flags();
        reset_storage_screen_boxes_flags();
        reset_cryo_screen_boxes_flags();
        reset_equip_screen_boxes_flags();
        reset_research_screen_boxes_flags();
        reset_system_menu_boxes_flags();

        int i;
        for (i = 0; i < 5; i++) {
            unk11_menu[i].Flags = 0x0001;
        }
        reset_options_screen_boxes_flags();

        set_flag01_storage_screen_boxes();
        set_flag01_login_screen_boxes();
        set_flag01_main_screen_boxes();
        set_flag01_cryo_screen_boxes();
        set_flag01_research_screen_boxes();

        set_flag01_net_screen_boxes();
        set_flag01_equip_screen_boxes();
        set_flag01_controls_screen_boxes();
        set_flag01_brief_screen_boxes();
        set_flag01_world_screen_boxes();

        if (!game_projector_speed && screentype != SCRT_99)
            play_sample_using_heap(0, 113, 127, 64, 100, 0, 3u);
    }

    if (gameturn & 1)
    {
      if (++data_1c498e > 7)
          data_1c498e = 0;
      LbMouseChangeSprite(&unk3_sprites[data_1c498e + 1]);
    }

    if (lbKeyOn[KC_F12]) {
        lbKeyOn[KC_F12] = 0;
        LbPngSaveScreen("synII", lbDisplay.WScreen, display_palette, 0);
    }

    if ( start_into_mission || map_editor )
    {
        show_load_and_prep_mission();
    }
    else if (reload_background_flag)
    {
        reload_background();
        reload_background_flag = 0;
    }

    if (exit_game)
    {
        wait_for_sound_sample_finish(111);
        stop_sample_using_heap(0, 122);
    }
}

void draw_game(void)
{
#if 0
    asm volatile ("call ASM_draw_game\n"
        :  :  : "eax" );
    return;
#endif
    switch (ingame.DisplayMode)
    {
    case DpM_UNKN_1:
        // No action
        break;
    case DpM_UNKN_32:
        PlayCDTrack(ingame.CDTrack);
        if ( !(ingame.Flags & GamF_Unkn0020) || !(gameturn & 0xF) )
        {
            show_game_engine();
            if ((ingame.Flags & GamF_Unkn0800) != 0)
              gproc3_unknsub2();
            BAT_play();
            if ( execute_commands )
            {
                long tmp;
                gamep_unknval_16 = nav_stats__ThisTurn;
                nav_stats__ThisTurn = 0;
                ++gamep_unknval_12;
                gamep_unknval_10 += gamep_unknval_16;
                gamep_unknval_15 = gamep_unknval_14;
                tmp = gamep_unknval_14 + gamep_unknval_11;
                gamep_unknval_14 = 0;
                gamep_unknval_11 = tmp;
            }
        }
        break;
    case DpM_UNKN_37:
        show_menu_screen();
        break;
    case DpM_UNKN_3A:
        gproc3_unknsub3(0);
        break;
    default:
        LOGERR("DisplayMode %d empty\n", (int)ingame.DisplayMode);
        break;
    }
}

void load_packet(void)
{
    asm volatile ("call ASM_load_packet\n"
        :  :  : "eax" );
}

void process_packets(void)
{
    asm volatile ("call ASM_process_packets\n"
        :  :  : "eax" );
}

void joy_input(void)
{
    asm volatile ("call ASM_joy_input\n"
        :  :  : "eax" );
}

void update_unkn_changing_colors(void)
{
#if 0
    asm volatile ("call ASM_update_unkn_changing_colors\n"
        :  :  : "eax" );
#else
    ubyte col1, col2;

    unkn_changing_color_counter1++;
    if (unkn_changing_color_counter1 & 0x01)
        col1 = colour_lookup[5];
    else
        col1 = colour_lookup[0];
    unkn_changing_color_1 = col1;

    if (unkn_changing_color_counter1 & 0x01)
        col2 = colour_lookup[2];
    else
        col2 = colour_lookup[1];
    unkn_changing_color_2 = col2;
#endif
}

/** Orbital station explosion code.
 */
void game_process_orbital_station_explode(void)
{
    if (unkn01_downcount > 0)
    {
        unkn01_downcount--;
        LOGDBG("unkn01_downcount = %ld", unkn01_downcount);
        if ( unkn01_downcount == 40 ) {
            mapwho_unkn01(unkn01_pos_x, unkn01_pos_y);
        }
        else if (unkn01_downcount < 40) {
            ushort stl_x, stl_y;

            stl_y = unkn01_pos_y + (LbRandomAnyShort() & 0xF) - 7;
            stl_x = unkn01_pos_x + (LbRandomAnyShort() & 0xF) - 7;
            new_bang_3(stl_x << 16, 0, stl_y << 16, 95);
            stl_y = unkn01_pos_y + (LbRandomAnyShort() & 0xF) - 7;
            stl_x = unkn01_pos_x + (LbRandomAnyShort() & 0xF) - 7;
            new_bang_3(stl_x << 16, 0, stl_y << 16, 95);
        }
    }
}

void game_process(void)
{
    debug_m_sprite(193);
    LOGDBG("WSCREEN 0x%lx", (ulong)lbDisplay.WScreen);
    while ( !exit_game )
    {
      process_sound_heap();
      // TODO Why resolution control here? Isn't it already handled in game_graphics_inputs()?
      if ( lbKeyOn[KC_F8] && (lbShift & KMod_CONTROL))
      {
          lbKeyOn[KC_F8] = 0;
          LbScreenSetup(lbDisplay.ScreenMode, 640, 480, display_palette);
      }
      navi2_unkn_counter -= 2;
      if (navi2_unkn_counter < 0)
          navi2_unkn_counter = 0;
      if (navi2_unkn_counter > navi2_unkn_counter_max)
          navi2_unkn_counter_max = navi2_unkn_counter;
      if (keyboard_mode_direct)
          input_char = LbKeyboard();
      if (ingame.DisplayMode == DpM_UNKN_37) {
          LOGDBG("id=%d  trial alloc = %d turn %lu", 0, triangulation, gameturn);
      }
      if (!LbScreenIsLocked()) {
          while (LbScreenLock() != Lb_SUCCESS)
              ;
      }
      input();
      game_process_sub01();
      draw_game();
      debug_trace_turn_bound(gameturn + 100);
      load_packet();
      if ( ((active_flags_general_unkn01 & 0x8000) != 0) !=
        ((ingame.Flags & GamF_Unkn8000) != 0) )
          LbPaletteSet(display_palette);
      active_flags_general_unkn01 = ingame.Flags;
      if ((ingame.DisplayMode == DpM_UNKN_32) ||
          (ingame.DisplayMode == DpM_UNKN_1) ||
          (ingame.DisplayMode == DpM_UNKN_3B))
          process_things();
      if (ingame.DisplayMode != DpM_UNKN_37)
          process_packets();
      joy_input();
      if (ingame.DisplayMode == DpM_UNKN_37) {
          swap_wscreen();
      }
      else if ( !(ingame.Flags & GamF_Unkn0020) || ((gameturn & 0xF) == 0) ) {
          LbScreenSwapClear(0);
      }
      update_unkn_changing_colors();
      game_process_orbital_station_explode();
      gameturn++;
      game_process_sub09();
    }
    PacketRecord_Close();
    LbPaletteFade(NULL, 0x10u, 1);
}

void
game_quit(void)
{
    host_reset();
    LbBaseReset();
    exit(0);
}

void
game_transform_path_full(const char *file_name, char *buffer, size_t size)
{
    if (strncasecmp (file_name, SAVEGAME_PATH,
             sizeof (SAVEGAME_PATH) - 1) == 0)
    {
        snprintf (buffer, size, "%s" FS_SEP_STR "%s", GetDirectoryUser(),
          file_name + sizeof (SAVEGAME_PATH) - 1);
        return;
    }

    /* abort on absolute paths */
    if (file_name[0] == '\\' || file_name[0] == '/'
        || (strlen (file_name) >= 2 && file_name[1] == ':'))
    {
        snprintf (buffer, size, "%s", file_name);
        return;
    }

    snprintf (buffer, size, "%s" FS_SEP_STR "%s", GetDirectoryHdd(), file_name);
}

void
game_transform_path(const char *file_name, char *result)
{
    game_transform_path_full (file_name, result, FILENAME_MAX);
}

static void
game_update_full(bool wait)
{
    static TbClockMSec last_loop_time = 0;

    display_unlock();

    game_handle_sdl_events();

    if (wait)
    {
        TbClockMSec curr_time = LbTimerClock();
        TbClockMSec sleep_end = last_loop_time + 1000/GAME_FPS;
        // If we missed the normal sleep target (ie. there was a slowdown), reset the value and do not sleep
        if ((sleep_end < curr_time) || (sleep_end > curr_time + 1000/GAME_FPS)) {
            LOGNO("missed FPS target, last frame time %ld too far from current %ld", (ulong)sleep_end, (ulong)curr_time);
            sleep_end = curr_time;
        }
        LbSleepUntil(sleep_end);
        last_loop_time = sleep_end;
    }

    display_lock();
}

int game_hacky_update(void)
{
    game_update_full(false);
    return 1;
}

void
game_update(void)
{
    game_update_full(true);
}

void engine_reset(void)
{
  LbMemoryFree(engine_mem_alloc_ptr);
}

void host_reset(void)
{
#if 0
    asm volatile ("call ASM_host_reset\n"
        :  :  : "eax" );
#else
    StopCD();
    setup_heaps(1);
    FreeAudio();
    engine_reset();
    LbSpriteReset(m_sprites, m_sprites_end, m_spr_data);
    LbSpriteReset(pointer_sprites, pointer_sprites_end, pointer_data);
    LbMouseReset();
    LbKeyboardClose();
    LbScreenReset();
    LbNetworkReset();
#endif
}

void
game_reset(void)
{
    host_reset();
    free_texturemaps();
    LbDataFreeAll(unk02_load_files);
}

