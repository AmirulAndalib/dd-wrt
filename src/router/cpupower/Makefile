# SPDX-License-Identifier: GPL-2.0-only
# Makefile for cpupower
#
# Copyright (C) 2005,2006 Dominik Brodowski <linux@dominikbrodowski.net>
#
# Based largely on the Makefile for udev by:
#
# Copyright (C) 2003,2004 Greg Kroah-Hartman <greg@kroah.com>
#
OUTPUT=./
ifeq ("$(origin O)", "command line")
	OUTPUT := $(O)/
endif

ifneq ($(OUTPUT),)
# check that the output directory actually exists
OUTDIR := $(shell cd $(OUTPUT) && pwd)
$(if $(OUTDIR),, $(error output directory "$(OUTPUT)" does not exist))
endif


# --- CONFIGURATION BEGIN ---

# Set the following to `true' to make a unstripped, unoptimized
# binary. Leave this set to `false' for production use.
DEBUG ?=	false

# make the build silent. Set this to something else to make it noisy again.
V ?=		true

# Internationalization support (output in different languages).
# Requires gettext.
NLS ?=		true

# Set the following to 'true' to build/install the
# cpufreq-bench benchmarking tool
CPUFREQ_BENCH ?= true

# Do not build libraries, but build the code in statically
# Libraries are still built, otherwise the Makefile code would
# be rather ugly.
export STATIC ?= true

# Prefix to the directories we're installing to
DESTDIR ?= $(INSTALLDIR)

# --- CONFIGURATION END ---



# Package-related definitions. Distributions can modify the version
# and _should_ modify the PACKAGE_BUGREPORT definition

VERSION:=			$(shell ./utils/version-gen.sh)
LIB_MAJ=			0.0.1
LIB_MIN=			1

PACKAGE =			cpupower
PACKAGE_BUGREPORT =		linux-pm@vger.kernel.org
LANGUAGES = 			de fr it cs pt ka


# Directory definitions. These are default and most probably
# do not need to be changed. Please note that DESTDIR is
# added in front of any of them

bindir ?=	/usr/bin
sbindir ?=	/usr/sbin
mandir ?=	/usr/man
includedir ?=	/usr/include
localedir ?=	/usr/share/locale
docdir ?=       /usr/share/doc/packages/cpupower
confdir ?=      /etc/
bash_completion_dir ?= /usr/share/bash-completion/completions

# Toolchain: what tools do we use, and what options do they need:

CP = cp -fpR
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA  = ${INSTALL} -m 644
#bash completion scripts get sourced and so they should be rw only.
INSTALL_SCRIPT = ${INSTALL} -m 644

# If you are running a cross compiler, you may want to set this
# to something more interesting, like "arm-linux-".  If you want
# to compile vs uClibc, that can be done here as well.
#CROSS = #/usr/i386-linux-uclibc/usr/bin/i386-uclibc-
#CC = $(CROSS)gcc
#LD = $(CROSS)gcc
#AR = $(CROSS)ar
#STRIP = $(CROSS)strip
#RANLIB = $(CROSS)ranlib
HOSTCC = gcc
MKDIR = mkdir

libdir =	/usr/lib

# Now we set up the build system
#

GMO_FILES = ${shell for HLANG in ${LANGUAGES}; do echo $(OUTPUT)po/$$HLANG.gmo; done;}

export CROSS CC AR STRIP RANLIB CFLAGS LDFLAGS LIB_OBJS

# check if compiler option is supported
cc-supports = ${shell if $(CC) ${1} -S -o /dev/null -x c /dev/null > /dev/null 2>&1; then echo "$(1)"; fi;}

# use '-Os' optimization if available, else use -O2
OPTIMIZATION := -O2

WARNINGS := -Wall -Wchar-subscripts -Wpointer-arith -Wsign-compare
WARNINGS += $(call cc-supports,-Wno-pointer-sign)
WARNINGS += $(call cc-supports,-Wdeclaration-after-statement)
WARNINGS += -Wshadow

override CFLAGS += -DVERSION=\"$(VERSION)\" -DPACKAGE=\"$(PACKAGE)\" \
		-DPACKAGE_BUGREPORT=\"$(PACKAGE_BUGREPORT)\" -D_GNU_SOURCE -DNEED_PRINTF $(COPTS) $(MIPS16_OPT) $(LTO)  $(OPTIMIZATION)

UTIL_OBJS =  utils/helpers/amd.o utils/helpers/msr.o \
	utils/helpers/sysfs.o utils/helpers/misc.o utils/helpers/cpuid.o \
	utils/helpers/pci.o utils/helpers/bitmask.o \
	utils/idle_monitor/nhm_idle.o utils/idle_monitor/snb_idle.o \
	utils/idle_monitor/hsw_ext_idle.o \
	utils/idle_monitor/amd_fam14h_idle.o utils/idle_monitor/cpuidle_sysfs.o \
	utils/idle_monitor/mperf_monitor.o utils/idle_monitor/cpupower-monitor.o \
	utils/idle_monitor/rapl_monitor.o \
	utils/cpupower.o utils/cpufreq-info.o utils/cpufreq-set.o \
	utils/cpupower-set.o utils/cpupower-info.o utils/cpuidle-info.o \
	utils/cpuidle-set.o utils/powercap-info.o

UTIL_SRC := $(UTIL_OBJS:.o=.c)

UTIL_OBJS := $(addprefix $(OUTPUT),$(UTIL_OBJS))

UTIL_HEADERS = utils/helpers/helpers.h utils/idle_monitor/cpupower-monitor.h \
	utils/helpers/bitmask.h \
	utils/idle_monitor/idle_monitors.h utils/idle_monitor/idle_monitors.def

LIB_HEADERS = 	lib/cpufreq.h lib/cpupower.h lib/cpuidle.h lib/acpi_cppc.h \
	lib/powercap.h
LIB_SRC = 	lib/cpufreq.c lib/cpupower.c lib/cpuidle.c lib/acpi_cppc.c \
	lib/powercap.c
LIB_OBJS = 	lib/cpufreq.o lib/cpupower.o lib/cpuidle.o lib/acpi_cppc.o \
	lib/powercap.o
LIB_OBJS :=	$(addprefix $(OUTPUT),$(LIB_OBJS))

override CFLAGS +=	-pipe

ifeq ($(strip $(NLS)),true)
	INSTALL_NLS += install-gmo
	COMPILE_NLS += create-gmo
	override CFLAGS += -DNLS
endif

ifeq ($(strip $(CPUFREQ_BENCH)),true)
	INSTALL_BENCH += install-bench
	COMPILE_BENCH += compile-bench
endif

ifeq ($(strip $(STATIC)),true)
        UTIL_OBJS += $(LIB_OBJS)
        UTIL_HEADERS += $(LIB_HEADERS)
        UTIL_SRC += $(LIB_SRC)
endif

override CFLAGS += $(WARNINGS)

ifeq ($(strip $(V)),false)
	QUIET=@
	ECHO=@echo
else
	QUIET=
	ECHO=@\#
endif
export QUIET ECHO

# if DEBUG is enabled, then we do not strip or optimize
ifeq ($(strip $(DEBUG)),true)
	override CFLAGS += -O1 -g -DDEBUG
	STRIPCMD = /bin/true -Since_we_are_debugging
else
	override CFLAGS += $(OPTIMIZATION) -fomit-frame-pointer
	STRIPCMD = $(STRIP) -s --remove-section=.note --remove-section=.comment
endif


# the actual make rules

all: $(OUTPUT)cpupower $(COMPILE_NLS) $(COMPILE_BENCH)

$(OUTPUT)lib/%.o: $(LIB_SRC) $(LIB_HEADERS)
	$(ECHO) "  CC      " $@
	$(QUIET) $(CC) $(CFLAGS) -fPIC -o $@ -c lib/$*.c


# Let all .o files depend on its .c file and all headers
# Might be worth to put this into utils/Makefile at some point of time
$(UTIL_OBJS): $(UTIL_HEADERS)

$(OUTPUT)%.o: %.c
	$(ECHO) "  CC      " $@
	$(QUIET) $(CC) $(CFLAGS) -I./lib -I ./utils -I$(TOP)/pciutils/lib -o $@ -c $*.c

$(OUTPUT)cpupower: $(UTIL_OBJS)
	$(ECHO) "  CC      " $@
ifeq ($(strip $(STATIC)),true)
	$(QUIET) $(CC) $(CFLAGS) $(LDFLAGS) $(UTIL_OBJS) -L$(TOP)/pciutils/lib -lrt -lpci -L$(OUTPUT) -o $@
else
	$(QUIET) $(CC) $(CFLAGS) $(LDFLAGS) $(UTIL_OBJS) -lcpupower -lrt -lpci -L$(OUTPUT) -o $@
endif
	$(QUIET) $(STRIPCMD) $@

$(OUTPUT)po/$(PACKAGE).pot: $(UTIL_SRC)
	$(ECHO) "  GETTEXT " $@
	$(QUIET) xgettext --default-domain=$(PACKAGE) --add-comments \
		--keyword=_ --keyword=N_ $(UTIL_SRC) -p $(@D) -o $(@F)

$(OUTPUT)po/%.gmo: po/%.po
	$(ECHO) "  MSGFMT  " $@
	$(QUIET) msgfmt -o $@ po/$*.po

create-gmo: ${GMO_FILES}

update-po: $(OUTPUT)po/$(PACKAGE).pot
	$(ECHO) "  MSGMRG  " $@
	$(QUIET) @for HLANG in $(LANGUAGES); do \
		echo -n "Updating $$HLANG "; \
		if msgmerge po/$$HLANG.po $< -o \
		   $(OUTPUT)po/$$HLANG.new.po; then \
			mv -f $(OUTPUT)po/$$HLANG.new.po $(OUTPUT)po/$$HLANG.po; \
		else \
			echo "msgmerge for $$HLANG failed!"; \
			rm -f $(OUTPUT)po/$$HLANG.new.po; \
		fi; \
	done;

compile-bench:
	@V=$(V) confdir=$(confdir) $(MAKE) -C bench O=$(OUTPUT)

# we compile into subdirectories. if the target directory is not the
# source directory, they might not exists. So we depend the various
# files onto their directories.
DIRECTORY_DEPS = $(LIB_OBJS) $(UTIL_OBJS) $(GMO_FILES)
$(DIRECTORY_DEPS): | $(sort $(dir $(DIRECTORY_DEPS)))

# In the second step, we make a rule to actually create these directories
$(sort $(dir $(DIRECTORY_DEPS))):
	$(ECHO) "  MKDIR      " $@
	$(QUIET) $(MKDIR) -p $@ 2>/dev/null

clean:
	-find $(OUTPUT) \( -not -type d \) -and \( -name '*~' -o -name '*.[oas]' \) -type f -print \
	 | xargs rm -f
	-rm -f $(OUTPUT)cpupower
	-rm -rf $(OUTPUT)po/*.gmo
	-rm -rf $(OUTPUT)po/*.pot
	$(MAKE) -C bench O=$(OUTPUT) clean


install-tools: $(OUTPUT)cpupower
	$(INSTALL) -d $(DESTDIR)${bindir}
	$(INSTALL_PROGRAM) $(OUTPUT)cpupower $(DESTDIR)${bindir}
	$(INSTALL) -d $(DESTDIR)${bash_completion_dir}
	$(INSTALL_SCRIPT) cpupower-completion.sh '$(DESTDIR)${bash_completion_dir}/cpupower'

install-man:
	$(INSTALL_DATA) -D man/cpupower.1 $(DESTDIR)${mandir}/man1/cpupower.1
	$(INSTALL_DATA) -D man/cpupower-frequency-set.1 $(DESTDIR)${mandir}/man1/cpupower-frequency-set.1
	$(INSTALL_DATA) -D man/cpupower-frequency-info.1 $(DESTDIR)${mandir}/man1/cpupower-frequency-info.1
	$(INSTALL_DATA) -D man/cpupower-idle-set.1 $(DESTDIR)${mandir}/man1/cpupower-idle-set.1
	$(INSTALL_DATA) -D man/cpupower-idle-info.1 $(DESTDIR)${mandir}/man1/cpupower-idle-info.1
	$(INSTALL_DATA) -D man/cpupower-set.1 $(DESTDIR)${mandir}/man1/cpupower-set.1
	$(INSTALL_DATA) -D man/cpupower-info.1 $(DESTDIR)${mandir}/man1/cpupower-info.1
	$(INSTALL_DATA) -D man/cpupower-monitor.1 $(DESTDIR)${mandir}/man1/cpupower-monitor.1
	$(INSTALL_DATA) -D man/cpupower-powercap-info.1 $(DESTDIR)${mandir}/man1/cpupower-powercap-info.1

install-gmo: create-gmo
	$(INSTALL) -d $(DESTDIR)${localedir}
	for HLANG in $(LANGUAGES); do \
		echo '$(INSTALL_DATA) -D $(OUTPUT)po/$$HLANG.gmo $(DESTDIR)${localedir}/$$HLANG/LC_MESSAGES/cpupower.mo'; \
		$(INSTALL_DATA) -D $(OUTPUT)po/$$HLANG.gmo $(DESTDIR)${localedir}/$$HLANG/LC_MESSAGES/cpupower.mo; \
	done;

install-bench: compile-bench
	@#DESTDIR must be set from outside to survive
	@sbindir=$(sbindir) bindir=$(bindir) docdir=$(docdir) confdir=$(confdir) $(MAKE) -C bench O=$(OUTPUT) DESTDIR=$(INSTALLDIR) install

ifeq ($(strip $(STATIC)),true)
install: all install-tools $(INSTALL_BENCH)
else
install: all install-lib install-tools install-man $(INSTALL_NLS) $(INSTALL_BENCH)
endif

uninstall:
	- rm -f $(DESTDIR)${includedir}/cpufreq.h
	- rm -f $(DESTDIR)${includedir}/cpuidle.h
	- rm -f $(DESTDIR)${bindir}/utils/cpupower
	- rm -f $(DESTDIR)${mandir}/man1/cpupower.1
	- rm -f $(DESTDIR)${mandir}/man1/cpupower-frequency-set.1
	- rm -f $(DESTDIR)${mandir}/man1/cpupower-frequency-info.1
	- rm -f $(DESTDIR)${mandir}/man1/cpupower-set.1
	- rm -f $(DESTDIR)${mandir}/man1/cpupower-info.1
	- rm -f $(DESTDIR)${mandir}/man1/cpupower-monitor.1
	- rm -f $(DESTDIR)${mandir}/man1/cpupower-powercap-info.1
	- for HLANG in $(LANGUAGES); do \
		rm -f $(DESTDIR)${localedir}/$$HLANG/LC_MESSAGES/cpupower.mo; \
	  done;

.PHONY: all utils update-po create-gmo install-lib install-tools install-man install-gmo install uninstall clean
