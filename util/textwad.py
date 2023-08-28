#!/usr/bin/python3
# -*- coding: utf-8 -*-
# vim:sw=4

""" Text WAD/IDX conversion tool for Syndicate Wars Port.

Prepares WAD/IDX files accepted by game engine.
"""

# Copyright (C) 2023 Mefistotelis <mefistotelis@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

__version__ = "0.0.1"
__author__ = "Mefistotelis"
__license__ = "GPL"

import sys
import argparse
import os
import re
import io
import json

from ctypes import c_char, c_int, c_ubyte, c_ushort, c_uint, c_ulonglong, c_float
from ctypes import memmove, addressof, sizeof, Array, LittleEndianStructure


class WADIndexEntry(LittleEndianStructure):
    _pack_ = 1
    _fields_ = [
      ('Filename', c_char * 12),
      ('Offset', c_uint),
      ('Length', c_uint),
    ]


class POEntry:
    # Translator Comment is something usually added by the translator
    comment_trans = ""
    # Extracted Comment come from the code, so are written by the programmer
    comment_extr = ""
    # String position reference, usually in form <file_name>:<line>
    references = None
    # Flags determining condition of the translation and its special handling
    flags = None
    # Context for the message - ie. general area where the message is displayed
    msgctxt = None
    # Untranslated string
    msgid = ""
    # Translated string
    msgstr = None
    def __init__(self, mstr = "", mctxt = ""):
        self.msgstr = mstr
        self.msgctxt = mctxt
        self.references = []
        self.flags = []
        pass


campaign_names = ["SYNDCT", "CHURCH", "PUNKS", "COMM"]


def pofile_store_entry(po, pofh, e):
    pofh.write("\n")
    if len(e.comment_trans) > 0:
        pofh.write("# " + e.comment_trans + "\n")
    if len(e.comment_extr) > 0:
        pofh.write("#. " + e.comment_extr + "\n")
    if len(e.references) > 0:
        pofh.write("#: " + " ".join(e.references) + "\n")
    if len(e.flags) > 0:
        pofh.write("#, " + " ".join(e.flags) + "\n")
    if len(e.msgctxt) > 0:
        pofh.write("msgctxt \"" + e.msgctxt + "\"\n")
    pofh.write("msgid \"" + e.msgid + "\"\n")
    pofh.write("msgstr \"" + e.msgstr + "\"\n")
    return


def pofile_store(po, pofname, polist, lang):
    if lang == "eng":
        polang = "en_GB"
    else:
        polang = "UNKNOWN"
    with open(pofname, "w", encoding="utf-8") as pofh:
        pofh.write("# *****************************************************************************\n")
        pofh.write("#  Syndicate Wars Port, source port of the classic strategy game from Bullfrog.\n")
        pofh.write("# *****************************************************************************\n")
        pofh.write("#   @file " + pofname + "\n")
        pofh.write("#      Menu text for SW Port translation file\n")
        pofh.write("#  @par Purpose:\n")
        pofh.write("#      Contains translation of the national text in the game.\n")
        pofh.write("#  @par Comment:\n")
        pofh.write("#      Use this file to improve the translation for specific language.\n")
        pofh.write("#  @author   Syndicate Wars Fans\n")
        pofh.write("#  @date     20 Aug 2023 - 02 Sep 2023\n")
        pofh.write("#  @par  Copying and copyrights:\n")
        pofh.write("#      This program is free software; you can redistribute it and/or modify\n")
        pofh.write("#      it under the terms of the GNU General Public License as published by\n")
        pofh.write("#      the Free Software Foundation; either version 2 of the License, or\n")
        pofh.write("#      (at your option) any later version.\n")
        pofh.write("#\n")
        pofh.write("# *****************************************************************************\n")
        pofh.write("msgid \"\"\n")
        pofh.write("msgstr \"\"\n")
        pofh.write("\"Project-Id-Version: Menu text for SW Port\\n\"\n")
        pofh.write("\"Report-Msgid-Bugs-To: https://github.com/swfans/swars/issues\\n\"\n")
        pofh.write("\"POT-Creation-Date: 2023-08-20 01:12+0200\\n\"\n")
        pofh.write("\"PO-Revision-Date: 2023-09-02 12:00+0100\\n\"\n")
        pofh.write("\"Last-Translator: Mefistotelis <mefistotelis@gmail.com>\\n\"\n")
        pofh.write("\"Language-Team: Syndicate Wars Fans <github.com/swfans>\\n\"\n")
        pofh.write("\"Language: "+ polang + "\\n\"\n")
        pofh.write("\"MIME-Version: 1.0\\n\"\n")
        pofh.write("\"Content-Type: text/plain; charset=UTF-8\\n\"\n")
        pofh.write("\"Content-Transfer-Encoding: 8bit\\n\"\n")
        pofh.write("\"X-Poedit-SourceCharset: utf-8\\n\"\n")
        pofh.write("\"X-Generator: Poedit 1.5.7\\n\"\n")
        for e in polist:
            pofile_store_entry(po, pofh, e)
    return


def prep_po_entries_city(po, podict, lines):
    city_prop_comments = [
        "name",
        "population size",
        "landscape",
        "language",
        "industry",
        "toxicity",
    ]

    polist = []
    # Read initial comments
    n = 0
    comment = ""
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        # Special case for translation which is commented out in original files
        if ln == "#UTOPIA":
            e = POEntry(ln[1:], comment)
            e.references.append("mapscreen.title")
            podict["COMM"].append(e)
            n += 1
            continue
        if ln[0] == "#":
            comment = ln[1:]
            n += 1
            continue
        break
    # Read names of city properties - entries for Synd and Church
    k = 0
    entryno = 0
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        if ln[0] == "#":
            comment = ln[1:]
            n += 1
            k = 0
            entryno += 1
            continue
        if entryno >= 2:
            break
        if k < 6:
            e = POEntry(ln, comment + " - " + city_prop_comments[k])
            campgn = campaign_names[entryno]
            e.references.append(f"mapscreen.heading:{k}")
            podict[campgn].append(e)
            n += 1
            k += 1
            continue
        break
    # Read the properties of cities - 6 per city
    k = 0
    entryno = 0
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        if ln[0] == "#":
            comment = ln[1:]
            n += 1
            continue
        if k >= 6:
            k = 0
            entryno += 1
        if True:
            if comment == "city data":
                e = POEntry(ln, "map screen location - " + city_prop_comments[k])
            else:
                e = POEntry(ln, comment)
            e.references.append(f"mapscreen.city{entryno}:{k}")
            podict["COMM"].append(e)
            n += 1
            k += 1
            continue
        break
    return polist


def prep_po_entries_per_line(po, podict, lines, refstart, comment):
    # Read initial comments
    n = 0
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        if True:
            text = ln[0].upper() + ln[1:].lower()
            e = POEntry(text, comment)
            e.references.append(f"{refstart}:{n}")
            podict["COMM"].append(e)
            n += 1
            continue
        break
    return


def textwad_extract_po(po, podict, txtfname, lines):
    match = None
    if not match:
        match = re.match(r'^(city)[.]txt$', txtfname)
        if match:
            prep_po_entries_city(po, podict, lines)
    if not match:
        match = re.match(r'^(lost)[.]txt$', txtfname)
        if match:
            prep_po_entries_per_line(po, podict, lines, "sci.death.reason", "scientists death reason")
    if not match:
        match = re.match(r'^(miss([0-9]+))[.]txt$', txtfname)
        if match:
            pass # TODO
    if not match:
        match = re.match(r'^(names)[.]txt$', txtfname)
        if match:
            pass # TODO
    if not match:
        match = re.match(r'^(netscan)[.]txt$', txtfname)
        if match:
            pass # TODO
    if not match:
        match = re.match(r'^(obj)[.]txt$', txtfname)
        if match:
            prep_po_entries_per_line(po, podict, lines, "scanner.objective", "objective text shown on scanner")
    if not match:
        match = re.match(r'^(outtro)[.]txt$', txtfname)
        if match:
            pass # TODO
    if not match:
        match = re.match(r'^(wms)[.]txt$', txtfname)
        if match:
            pass # TODO
    if not match:
        pass # unrecognized file type
    return


def textwad_extract_raw(po, wadfh, idxfh):
    e = WADIndexEntry()
    while idxfh.readinto(e) == sizeof(e):
        txtfname = e.Filename.decode("utf-8").lower()
        n = 0
        wadfh.seek(e.Offset, os.SEEK_SET)
        with open(txtfname, 'wb') as txtfh:
            copy_buffer = wadfh.read(e.Length)
            n += len(copy_buffer)
            txtfh.write(copy_buffer)
    return


def textwad_extract(po, wadfh, idxfh):
    lang = "eng"
    # Prepare empty dict
    podict = {}
    for campgn in campaign_names:
        podict[campgn] = []
    # Fill with files from WAD
    e = WADIndexEntry()
    while idxfh.readinto(e) == sizeof(e):
        txtfname = e.Filename.decode("utf-8").lower()
        wadfh.seek(e.Offset, os.SEEK_SET)
        txt_buffer = wadfh.read(e.Length)
        lines = txt_buffer.split(b'\n')
        # TODO support SW code page
        lines = [ln.decode("utf-8").rstrip() for ln in lines]
        textwad_extract_po(po, podict, txtfname, lines)
        basename = os.path.splitext(os.path.basename(txtfname))[0]
    if lang == "eng":
        poext = "pot"
        for campgn in campaign_names:
            for e in podict[campgn]:
                e.msgid = e.msgstr
                e.msgstr = ""
    else:
        poext = "po"
    for campgn in campaign_names:
        pofname = "text_" + campgn.lower() + "_" + lang + "." + poext
        pofile_store(po, pofname, podict[campgn], lang)
    return


def textwad_create(po, wadfh, idxfh):
    idxfh.write((c_ubyte * sizeof(e)).from_buffer_copy(e))


def main():
    """ Main executable function.

    Its task is to parse command line options and call a function which performs requested command.
    """
    parser = argparse.ArgumentParser(description=__doc__.split('.')[0])

    parser.add_argument('-w', '--wadfile', type=str, required=True,
          help="Name for WAD/IDX files")

    parser.add_argument('-v', '--verbose', action='count', default=0,
          help="Increases verbosity level; max level is set by -vvv")

    subparser = parser.add_mutually_exclusive_group(required=True)

    subparser.add_argument('-x', '--extract', action='store_true',
          help="Extract to text files")

    subparser.add_argument('-c', '--create', action='store_true',
          help="Create WAD/IDX from text files")

    subparser.add_argument('--version', action='version', version="%(prog)s {version} by {author}"
            .format(version=__version__, author=__author__),
          help="Display version information and exit")

    po = parser.parse_args()

    po.wadbase = os.path.splitext(os.path.basename(po.wadfile))[0]
    assert len(po.wadbase) > 0, "Provided WAD file name base is too short"
    po.wadfile = po.wadbase + ".wad"
    po.idxfile = po.wadbase + ".idx"

    if po.extract:
        if (po.verbose > 0):
            print("{}: Opening for extract".format(po.wadfile))
        with open(po.wadfile, 'rb') as wadfh:
            with open(po.idxfile, 'rb') as idxfh:
                textwad_extract(po, wadfh, idxfh)

    elif po.create:
        if (po.verbose > 0):
            print("{}: Opening for creation".format(po.wadfile))
        raise NotImplementedError("Unsupported command.")
        with open(po.wadfile, 'wb') as wadfh:
            with open(po.idxfile, 'wb') as idxfh:
                TODO(po, wadfh)

    else:
        raise NotImplementedError("Unsupported command.")


if __name__ == '__main__':
    try:
        main()
    except Exception as ex:
        print("Error: "+str(ex), file=sys.stderr)
        if 1: raise
        sys.exit(10)
