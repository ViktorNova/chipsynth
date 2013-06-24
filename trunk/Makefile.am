## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = \
src/libs/README \
src/gui/chipsynth.ui \
src/gui/configDialog.ui \
src/gui/resources.qrc \
src/gui/icons/chipsynth.svg

src_chipsynth_CPPFLAGS = \
@debug_flags@ \
@rt_flags@ \
$(QT4_CFLAGS) \
$(KDE_CFLAGS) \
$(ALSA_CFLAGS) \
$(JACK_CFLAGS) \
$(PULSE_CFLAGS) \
-I$(top_srcdir)/src \
-I$(top_srcdir)/src/engine \
-I$(top_srcdir)/src/libs \
-I$(top_srcdir)/src/gui \
-I$(top_srcdir)/src/sound \
-I$(top_builddir)/src/gui

src_chipsynth_LDFLAGS = \
$(QT4_LIBS) \
$(KDE_LIBS) \
$(ALSA_LIBS) \
$(JACK_LIBS) \
$(PULSE_LIBS) \
$(CORE_LIBS) \
$(MM_LIBS) \
$(DS_LIBS) \
$(KS_LIBS)

bin_PROGRAMS = src/chipsynth

src_chipsynth_SOURCES = \
src/main.cpp \
src/gui/chipsynth.cpp \
src/gui/chipsynth.h \
src/gui/configDialog.cpp \
src/gui/configDialog.h \
src/sound/csaudio.cpp \
src/sound/csaudio.h \
src/sound/csmidi.cpp \
src/sound/csmidi.h \
src/engine/envelope.cpp \
src/engine/filter.cpp \
src/engine/wavegen.cpp \
src/engine/engine.cpp \
src/engine/engine.h \
src/engine/envelope.h \
src/engine/filter.h \
src/engine/wavegen.h \
src/engine/delay.hpp \
src/engine/echo.hpp \
src/engine/chorus.hpp \
src/engine/oscillator.hpp \
src/engine/filter_module.hpp \
src/engine/wavegen_module.hpp \
src/engine/oscillator_module.hpp \
src/libs/RtError.h \
src/libs/RtAudio.cpp \
src/libs/RtAudio.h \
src/libs/RtMidi.cpp \
src/libs/RtMidi.h


nodist_src_chipsynth_SOURCES = \
src/gui/resources_rcc.cpp \
src/gui/chipsynth_ui.h \
src/gui/chipsynth_moc.cpp \
src/gui/configDialog_ui.h \
src/gui/configDialog_moc.cpp

BUILT_SOURCES = \
src/gui/chipsynth_ui.h \
src/gui/configDialog_ui.h

if !HAVE_KDE
src_chipsynth_SOURCES += \
src/gui/kled.cpp \
src/gui/kled.h
nodist_src_chipsynth_SOURCES += \
src/gui/moc_kled.cpp
endif

CLEANFILES = \
src/gui/resources_rcc.cpp \
src/gui/chipsynth_ui.h \
src/gui/chipsynth_moc.cpp \
src/gui/configDialog_ui.h \
src/gui/configDialog_moc.cpp

%_ui.h: %.ui
	uic $< -o $@

%_moc.cpp: %.h
	moc $< -o $@

%_rcc.cpp: %.qrc
	rcc $< -o $@
