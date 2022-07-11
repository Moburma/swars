/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file giff.c
 *     Implementation of Interchange File Format image format read/write.
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
#include "bfpng.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <png.h>

#include "bftypes.h"
#include "bffnuniq.h"
#include "bfmemory.h"
#include "bffile.h"
#include "bfscreen.h"
#include "privbflog.h"

static void palette_to_png(png_color *pngpal, const ubyte *pal, ushort num)
{
    int i;
    for (i = 0; i < num; i++) {
        pngpal[i].red   = pal[3 * i + 0] << 2;
        pngpal[i].green = pal[3 * i + 1] << 2;
        pngpal[i].blue  = pal[3 * i + 2] << 2;
    }
}

static void palette_from_png(ubyte *pal, const png_color *pngpal, ushort num)
{
    int i;
    for (i = 0; i < num; i++) {
        pal[3 * i + 0] = pngpal[i].red >> 2;
        pal[3 * i + 1] = pngpal[i].green >> 2;
        pal[3 * i + 2] = pngpal[i].blue >> 2;
    }
}

TbResult LbPngRead(FILE *img_fh, ubyte *out_buffer,
  ulong *width, ulong *height, ubyte *pal)
{
    png_byte  header[8]; // 8 is the maximum size that can be checked
    png_structp png = NULL;
    png_infop info = NULL;
    png_byte **rows = NULL;
    png_byte color_type;
    png_byte bit_depth;
    size_t w, h, i;
    char *action;

    action = "verify PNG magic identifier";
    fread(header, 1, 8, img_fh);
    if (png_sig_cmp(header, 0, 8))
        goto err;

    // initialize stuff
    action = "create read struct";
    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL)
        goto err;

    action = "create info struct";
    info = png_create_info_struct(png);
    if (info == NULL)
        goto err;

    action = "perform init_io";
    if (setjmp(png_jmpbuf(png)))
        goto err;

    png_init_io(png, img_fh);
    png_set_sig_bytes(png, 8);

    png_read_info(png, info);

    *width = w = png_get_image_width(png, info);
    *height = h = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth = png_get_bit_depth(png, info);

    action = "read colour palette";
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        // If the file uses colour palette, get it
        png_colorp palette;
        int num;

        png_get_PLTE(png, info, &palette, &num);
        palette_to_png(palette, pal, num);
    } else {
        // Other file formats are not supported
        goto err;
    }

    int number_of_passes;
    number_of_passes = png_set_interlace_handling(png);
    png_read_update_info(png, info);

    action = "read image data";
    if (setjmp(png_jmpbuf(png)))
        goto err;

    // libPNG geves us access to individual rows of the image
    rows = (png_byte**)LbMemoryAlloc(sizeof(png_byte*) * h);
    // we need to provide pointers to the start of each row
    for (i = 0; i < h; i++)
        rows[i] = (png_byte *) out_buffer + i * w;

    png_read_image(png, rows);

    // finalise reading
    png_read_end(png, info);
    // cleanup heap allocation
    png_destroy_read_struct(&png, &info, NULL);
    LbMemoryFree(rows);
    return Lb_SUCCESS;

err: // handle error and cleanup heap allocation
    LOGERR("could not %s", action);
    if (png != NULL && info != NULL)
        png_destroy_read_struct(&png, &info, NULL);
    else if (png != NULL)
        png_destroy_read_struct(&png, (png_infopp) NULL, NULL);
    LbMemoryFree(rows);
    return Lb_FAIL;
}

TbResult LbPngLoad(const char *fname, ubyte *out_buffer,
  ubyte *pal)
{
    TbFileHandle handle;
    TbResult ret;

    handle = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    if (handle == INVALID_FILE) {
        return Lb_FAIL;
    }

    LbFileClose(handle);
    return ret;
}

TbResult LbPngWrite(FILE *img_fh, const ubyte *inp_buffer,
  ulong width, ulong height, const ubyte *pal)
{
    png_structp png = NULL;
    png_infop info = NULL;
    png_color palette[256];
    png_byte **rows;
    size_t w, h, i;
    char *action;

    // get parameters
    w = width;
    h = height;
    rows = (png_byte**) LbMemoryAlloc(sizeof (png_byte*) * h);

    // set image rows for png processing
    for (i = 0; i < h; i++)
        rows[i] = (png_byte *) inp_buffer + i * w;

    // initialise
    action = "create png_create_write_struct";
    png = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (png == NULL)
        goto err;

    action = "create png_create_info_struct";
    info = png_create_info_struct(png);
    if (info == NULL)
        goto err;

    png_init_io(png, img_fh);

    // write header
    png_set_IHDR(png, info, w, h, 8,
                  PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_ADAM7,
                  PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    palette_to_png(palette, pal, 256);
    png_set_PLTE(png, info, palette, 256);
    png_write_info(png, info);

    // write bytes
    png_write_image(png, rows);

    // finalise writing
    png_write_end(png, info);
    // cleanup heap allocation
    png_destroy_write_struct(&png, &info);
    LbMemoryFree(rows);
    return Lb_SUCCESS;

err: // handle error and cleanup heap allocation
    LOGERR("could not %s", action);
    if (png != NULL && info != NULL)
        png_destroy_write_struct(&png, &info);
    else if (png != NULL)
        png_destroy_write_struct(&png, (png_infopp) NULL);
    LbMemoryFree(rows);
    return Lb_FAIL;
}

TbResult LbPngSave(const char *fname, const TbPixel *inp_buffer,
  ulong width, ulong height, const ubyte *pal, TbBool force_fname)
{
    char full_fname[FILENAME_MAX];
    FILE *img_fh;
    TbResult ret;

    if (force_fname) {
        sprintf(full_fname, "%s", fname);
    } else {
        LbPrepareImageFilename(full_fname, fname, "png");
    }
    img_fh = fopen(full_fname, "wb");
    if (!img_fh) {
        LOGERR("%s: cannot open: %s", full_fname, strerror(errno));
        return 0;
    }
    ret = LbPngWrite(img_fh, inp_buffer, width, height, pal);
    fclose(img_fh);
    return ret;
}

TbResult LbPngSaveScreen(const char *fname, const TbPixel *inp_buffer,
  const ubyte *pal, TbBool force_fname)
{
    return LbPngSave(fname, inp_buffer, lbDisplay.PhysicalScreenWidth,
      lbDisplay.PhysicalScreenHeight, pal, force_fname);
}

/******************************************************************************/
