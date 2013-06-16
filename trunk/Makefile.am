## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign

src_chipsynth_CPPFLAGS = \
$(QT4_CFLAGS) \
$(KDE_CFLAGS) \
$(ALSA_CFLAGS) \
-I$(top_srcdir)/src \
-I$(top_srcdir)/src/engine \
-I$(top_srcdir)/src/gui \
-I$(top_srcdir)/src/sound \
-I$(top_builddir)/src/gui

src_chipsynth_LDFLAGS = \
$(QT4_LIBS) \
$(KDE_LIBS) \
$(ALSA_LIBS) \
-lpthread

bin_PROGRAMS = src/chipsynth

src_chipsynth_SOURCES = \
src/main.cpp \
src/gui/chipsynth.cpp \
src/gui/chipsynth.h \
src/sound/alsa_sound.cpp \
src/sound/alsa_sound.h \
src/sound/sound_thread.cpp \
src/sound/sound_thread.h \
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
src/engine/oscillator_module.hpp

nodist_src_chipsynth_SOURCES = \
src/gui/chipsynth_ui.h \
src/gui/chipsynth_moc.cpp \
src/sound/sound_thread_moc.cpp

BUILT_SOURCES = \
src/gui/chipsynth_ui.h

EXTRA_DIST = \
src/gui/chipsynth.ui

if !HAVE_KDE
src_chipsynth_SOURCES += \
src/gui/kled.cpp \
src/gui/kled.h
nodist_src_chipsynth_SOURCES += \
src/gui/moc_kled.cpp
endif

CLEANFILES = \
src/gui/chipsynth_ui.h \
src/gui/chipsynth_moc.cpp \
src/sound/sound_thread_moc.cpp

%_ui.h: %.ui
	uic $< -o $@

%_moc.cpp: %.h
	moc $< -o $@
