bin_PROGRAMS = swars
bindir = $(prefix)/$(PACKAGE)

MKW = $(PYTHON) $(top_srcdir)/util/mkwrappers

swars_SOURCES = \
	bflib_basics.h \
	bflib_joyst.c \
	bflib_joyst.h \
	bflib_joyst_s.sx \
	bflib_test_poly.c \
	bflib_render_gpoly.c \
	bflib_render_gpoly_s.sx \
	bflib_render_plin_s.sx \
	bflib_render_drspr_s.sx \
	bflib_sndlib_s.sx \
	bflib_snd_mss_s.sx \
	bflib_snd_sys_s.sx \
	bflib_vidraw.c \
	bflib_vidraw.h \
	bflib_vidraw_s.sx \
	../bfsoundlib/include/ail.h \
	../bfsoundlib/src/ail.c \
	../bfsoundlib/include/ailss.h \
	../bfsoundlib/src/ailss.c \
	../bfsoundlib/src/ailvoc.c \
	../bfsoundlib/src/ailwav.c \
	../bfsoundlib/include/aila.h \
	../bfsoundlib/src/aila.c \
	../bfsoundlib/include/aildebug.h \
	../bfsoundlib/src/aildebug.c \
	../bfsoundlib/include/mssdig.h \
	../bfsoundlib/src/mssdig.c \
	../bfsoundlib/include/msssys.h \
	../bfsoundlib/src/msssys.c \
	../bfsoundlib/include/mssxmidi.h \
	../bfsoundlib/src/mssxmidi.c \
	../bfsoundlib/include/mssxdig.h \
	../bfsoundlib/src/mssxdig.c \
	../bfsoundlib/include/bfaudio.h \
	../bfsoundlib/src/bfaudio.c \
	../bfsoundlib/include/bfsvaribl.h \
	../bfsoundlib/src/variable.c \
	../bfsoundlib/src/dpmi.c \
	../bfsoundlib/include/dpmi.h \
	../bfsoundlib/src/sb16.c \
	../bfsoundlib/include/sb16.h \
	../bfsoundlib/src/awe32.c \
	../bfsoundlib/src/awe32use.c \
	../bfsoundlib/include/awe32.h \
	../bfsoundlib/include/ssampfad.h \
	../bfsoundlib/src/ssampfad.c \
	../bfsoundlib/include/ssamplst.h \
	../bfsoundlib/src/ssamplst.c \
	../bfsoundlib/include/ssampply.h \
	../bfsoundlib/src/ssampply.c \
	../bfsoundlib/include/dllload.h \
	../bfsoundlib/src/dllload.c \
	../bfsoundlib/include/memfile.h \
	../bfsoundlib/src/memfile.c \
	../bfsoundlib/include/miscutil.h \
	../bfsoundlib/src/miscutil.c \
	../bfsoundlib/include/snderr.h \
	../bfsoundlib/src/snderr.c \
	../bfsoundlib/include/bfmusic.h \
	../bfsoundlib/src/init_mus.c \
	../bfsoundlib/src/mseqnply.c \
	../bfsoundlib/src/mseqnfad.c \
	../bfsoundlib/src/mseqnlst.c \
	../bfsoundlib/include/bfsound.h \
	../bfsoundlib/src/init_snd.c \
	../bfsoundlib/include/bfscd.h \
	../bfsoundlib/src/cd.c \
	../bfsoundlib/include/streamfx.h \
	../bfsoundlib/src/streamfx.c \
	../bfsoundlib/include/sndtimer.h \
	../bfsoundlib/src/sndtimer.c \
	../bfsoundlib/include/drv_oal.h \
	../bfsoundlib/src/drv_oal.c \
	../bfsoundlib/include/oggvorbis.h \
	../bfsoundlib/src/oggvorbis.c \
	../bfsoundlib/src/ailssa.sx \
	../bfariadne/include/trfringe.h \
	../bfariadne/src/trfringe.c \
	../bfariadne/include/triangls.h \
	../bfariadne/src/triangls.c \
	../bfariadne/include/tringops.h \
	../bfariadne/src/tringops.c \
	../bfariadne/include/trpath.h \
	../bfariadne/include/trpoints.h \
	../bfariadne/src/trpoints.c \
	../bfariadne/src/trfind8.c \
	../bfariadne/include/trfind8.h \
	../bfariadne/src/trstate.c \
	../bfariadne/include/trstate.h \
	../bfariadne/src/delaunay.c \
	../bfariadne/include/delaunay.h \
	../bfsmacklib/include/bfsmack.h \
	../bfsmacklib/src/bfsmack.c \
	../bfsmacklib/src/bfsmacklib_s.sx \
	linksmk.h \
	linksmk.c \
	config.h \
	bmbang.c \
	bmbang.h \
	bat.sx \
	bigmap.c \
	bigmap.h \
	building.c \
	building.h \
	campaign.c \
	campaign.h \
	command.c \
	command.h \
	cybmod.c \
	cybmod.h \
	display.c \
	display.h \
	dos.c \
	dos.h \
	drawtext.c \
	drawtext.h \
	enginbckt.c \
	enginbckt.h \
	engindrwlst.c \
	engindrwlst.h \
	enginlights.c \
	enginlights.h \
	enginpriobjs.c \
	enginpriobjs.h \
	enginpritxtr.c \
	enginpritxtr.h \
	enginsngobjs.c \
	enginsngobjs.h \
	enginsngtxtr.c \
	enginsngtxtr.h \
	enginpeff.c \
	enginpeff.h \
	engintrns.c \
	engintrns.h \
	enginzoom.c \
	enginzoom.h \
	febrief.c \
	febrief.h \
	fedebrief.c \
	fedebrief.h \
	fecntrls.c \
	fecntrls.h \
	fecryo.c \
	fecryo.h \
	feequip.c \
	feequip.h \
	feoptions.c \
	feoptions.h \
	fepanet.c \
	fepanet.h \
	felogin.c \
	felogin.h \
	femail.c \
	femail.h \
	femain.c \
	femain.h \
	fenet.c \
	fenet.h \
	fepause.c \
	fepause.h \
	feresearch.c \
	feresearch.h \
	festorage.c \
	festorage.h \
	feworld.c \
	feworld.h \
	guiboxes.c \
	guiboxes.h \
	guigraph.c \
	guigraph.h \
	guitext.c \
	guitext.h \
	game.c \
	game_bstype.h \
	game_data.c \
	game_data.h \
	game_speed.c \
	game_speed.h \
	game.h \
	globals.h \
	hud_panel.c \
	hud_panel.h \
	hud_target.c \
	hud_target.h \
	joy_modul1.sx \
	joy_modul2.sx \
	keyboard.c \
	keyboard.h \
	lvobjctv.c \
	lvobjctv.h \
	lvfiles.c \
	lvfiles.h \
	main.c \
	matrix.c \
	matrix.h \
	mouse.c \
	mouse.h \
	netipx1.sx \
	netser1.sx \
	network.c \
	network.h \
	pathtrig.c \
	pathtrig_debug.c \
	pathtrig.h \
	packet.c \
	packet.h \
	people.c \
	people.h \
	pepgroup.c \
	pepgroup.h \
	player.c \
	player.h \
	radica1.sx \
	radica2.sx \
	radica3.sx \
	research.c \
	research.h \
	rules.c \
	rules.h \
	scanner.c \
	scanner.h \
	scandraw.c \
	scandraw.h \
	sound.c \
	sound.h \
	specblit.c \
	specblit.h \
	swars.sx \
	swlog.h \
	thing.c \
	thing.h \
	thing_debug.c \
	thing_debug_s.sx \
	timer.c \
	timer.h \
	unix.c \
	unix.h \
	util.c \
	util.h \
	vehicle.c \
	vehicle.h \
	vehtraffic.c \
	vehtraffic.h \
	wadfile.c \
	wadfile.h \
	weapon.c \
	weapon.h \
	windows.c \
	windows.h \
	wrcities.c \
	wrcities.h

swars_CPPFLAGS = \
  -I"$(top_srcdir)/bflibrary/include" \
  -I"$(top_srcdir)/bfsoundlib/include" \
  -I"$(top_srcdir)/bfsmacklib/include" \
  -I"$(top_srcdir)/bfariadne/include"

swars_WRAPPERS = wrappers_dos.o wrappers_game.o wrappers_libc.o wrappers_util.o

swars_RCFLAGS = \
  -I"$(top_srcdir)/src" -I"$(builddir)" \
  $(RCFLAGS)

if HAS_WINDRES
swars_RESRCS = swars_stdres.res
else
swars_RESRCS =
endif

# Pretending to contain c++ source so that Automake select c++ linker
nodist_EXTRA_swars_SOURCES = dummy.cxx

swars_LDADD = \
  $(swars_WRAPPERS) $(swars_RESRCS) \
  -L$(builddir)/../bflibrary -lbullfrog

$(swars_WRAPPERS:.o=.sx): %.sx: $(top_srcdir)/conf/%.conf
	$(AM_V_GEN)$(MKW) $(MKWFLAGS) -o $@ $<

.sx.o:
	$(AM_V_CC)$(CPP) $(CPPFLAGS) $< $(ASFILTER) \
		| $(CCAS) -x assembler -c $(CCASFLAGS) -o $@ -

if HAS_WINDRES
%.res: $(top_srcdir)/res/%.rc
	$(WINDRES) $(swars_RCFLAGS) -i $< -J rc -o $@ -O coff
endif

# Include dynamic libraries in the package
if TARGET_WINDOWS
install-exec-hook:
	mkdir -p "$(DESTDIR)${prefix}/$(PACKAGE)"
	## Find main executable dependencies
	$(eval lib_SHARED_INSTALL := $(shell objdump -p swars$(EXEEXT) | \
		sed -n 's/\s*\(DLL Name:\|NEEDED\)\s*\(.*\)$$/\2/p' | \
		xargs -I {} find $(shell dirname $(shell which ${CXX})) -name '{}'))
	## Find sub-dependencies for the previous dependencies
	$(eval lib_SHARED_INSTALL += $(shell echo -n '$(lib_SHARED_INSTALL)' | \
        xargs -d ' ' -I {} objdump -p '{}' | \
		sed -n 's/\s*\(DLL Name:\|NEEDED\)\s*\(.*\)$$/\2/p' | \
		xargs -I {} find $(shell dirname $(shell which ${CXX})) -name '{}'))
	## Remove duplicates
	$(eval lib_SHARED_INSTALL := $(sort $(lib_SHARED_INSTALL)))
	cp $(lib_SHARED_INSTALL) $(DESTDIR)${prefix}/$(PACKAGE)
endif

CLEANFILES = wrappers_*.sx
DISTCLEANFILES = *~
