bin_PROGRAMS = swars
bindir = $(prefix)/$(PACKAGE)

MKW = $(PYTHON) $(top_srcdir)/util/mkwrappers

swars_SOURCES = \
	app_gengh.c \
	app_gentab.h \
	app_spr_init.c \
	ap_gspr_map.c \
	ap_spr_smap.c \
	app_sprite.h \
	bflib_joyst.c \
	bflib_joyst.h \
	bflib_joyst_s.sx \
	bflib_render_gpoly.c \
	bflib_render_plin_s.sx \
	bflib_render_drspr.c \
	bflib_render_drspr.h \
	bflib_render_drspr_s.sx \
	bflib_snd_mss_s.sx \
	bflib_vidraw.c \
	bflib_vidraw.h \
	bflib_vidraw_s.sx \
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
	../bfsmacklib/include/smack.h \
	../bfsmacklib/include/smack2ail.h \
	../bfsmacklib/src/smack2ail.c \
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
	engindrwlstm.c \
	engindrwlstm.h \
	engindrwlstx.c \
	engindrwlstx.h \
	enginfexpl.c \
	enginfexpl.h \
	enginfloor.c \
	enginfloor.h \
	enginlights.c \
	enginlights.h \
	enginpriobjs.c \
	enginpriobjs.h \
	enginpritxtr.c \
	enginpritxtr.h \
	enginshrapn.c \
	enginshrapn.h \
	enginsngobjs.c \
	enginsngobjs.h \
	enginsngtxtr.c \
	enginsngtxtr.h \
	enginshadws.c \
	enginshadws.h \
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
	game_save.c \
	game_save.h \
	game_speed.c \
	game_speed.h \
	game_sprani.c \
	game_sprani.h \
	game_sprts.c \
	game_sprts.h \
	game.h \
	globals.h \
	hud_panecfg.c \
	hud_panecfg.h \
	hud_panel.c \
	hud_panel.h \
	hud_target.c \
	hud_target.h \
	joy_modul1.sx \
	joy_modul2.sx \
	keyboard.c \
	keyboard.h \
	lvdraw3d.c \
	lvdraw3d.h \
	lvobjctv.c \
	lvobjctv.h \
	lvfiles.c \
	lvfiles.h \
	main.c \
	matrix.c \
	matrix.h \
	misstat.c \
	misstat.h \
	mouse.c \
	mouse.h \
	mydraw.c \
	mydraw.h \
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
	plyr_packet.c \
	plyr_packet.h \
	plyr_usrinp.c \
	plyr_usrinp.h \
	purpldrw.c \
	purpldrw.h \
	purpldrwlst.c \
	purpldrwlst.h \
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
	thing_search.c \
	thing_search.h \
	thing_debug.c \
	thing_debug_s.sx \
	tngcolisn.c \
	tngcolisn.h \
	tngobjdrw.c \
	tngobjdrw.h \
	timer.c \
	timer.h \
	osunix.c \
	osunix.h \
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
	oswindws.c \
	oswindws.h \
	wrcities.c \
	wrcities.h

swars_CPPFLAGS = \
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

# Libraries included with the project
swars_PROJECT_LIBS = ../lib/libbullfrog.a ../lib/libbfsound.a

$(swars_OBJECTS) $(bin_PROGRAMS): $(swars_PROJECT_LIBS)

swars_LDADD = \
  $(swars_WRAPPERS) $(swars_RESRCS)

../lib/libbullfrog.a: ../bflibrary/libbullfrog.a
	$(MAKE) -C $(<D) DESTDIR="../" prefix="" install

../lib/libbfsound.a: ../bfsoundlib/libbfsound.a
	$(MAKE) -C $(<D) DESTDIR="../" prefix="" install

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
