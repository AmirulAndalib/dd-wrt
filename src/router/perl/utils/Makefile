# -*- buffer-read-only: t -*-
# !!!!!!!   DO NOT EDIT THIS FILE   !!!!!!!
# This file is built by utils/Makefile.PL.
# Any changes made here will be lost!

PERL = ../miniperl
REALPERL = ../perl
RUN =  # Used mainly cross-compilation setups.


# Files to be built with variable substitution after miniperl is
# available.  Dependencies handled manually below (for now).

pl = corelist.PL cpan.PL h2ph.PL h2xs.PL instmodsh.PL json_pp.PL perlbug.PL perldoc.PL perlivp.PL pl2pm.PL prove.PL ptar.PL ptardiff.PL ptargrep.PL shasum.PL splain.PL libnetcfg.PL piconv.PL enc2xs.PL encguess.PL xsubpp.PL pod2html.PL zipdetails.PL
plextract = corelist cpan h2ph h2xs instmodsh json_pp perlbug perldoc perlivp pl2pm prove ptar ptardiff ptargrep shasum splain libnetcfg piconv enc2xs encguess xsubpp pod2html zipdetails
plextractexe = ./corelist ./cpan ./h2ph ./h2xs ./json_pp ./instmodsh ./perlbug ./perldoc ./perlivp ./pl2pm ./prove ./ptar ./ptardiff ./ptargrep ./shasum ./splain ./libnetcfg ./piconv ./enc2xs ./encguess ./xsubpp ./pod2html ./zipdetails

all: $(plextract) 

$(plextract):
	$(RUN) $(PERL) -I../lib $@.PL

cpan:		cpan.PL ../config.sh

corelist:	corelist.PL ../config.sh

h2ph:		h2ph.PL ../config.sh

h2xs:		h2xs.PL ../config.sh

instmodsh:	instmodsh.PL ../config.sh

json_pp:	json_pp.PL ../config.sh

perlbug:	perlbug.PL ../config.sh ../patchlevel.h

perldoc:	perldoc.PL ../config.sh

perlivp:	perlivp.PL ../config.sh

prove:		prove.PL ../config.sh

ptar:		ptar.PL ../config.sh

ptardiff:	ptardiff.PL ../config.sh

ptargrep:	ptargrep.PL ../config.sh

pl2pm:		pl2pm.PL ../config.sh

shasum:		shasum.PL ../config.sh

splain:		splain.PL ../config.sh ../lib/diagnostics.pm

libnetcfg:	libnetcfg.PL ../config.sh

piconv:		piconv.PL ../config.sh

enc2xs:		enc2xs.PL ../config.sh

enc2xs:		encguess.PL ../config.sh

xsubpp:		xsubpp.PL ../config.sh

zipdetails:	zipdetails.PL ../config.sh

pod2html:	pod2html.PL ../config.sh ../ext/Pod-Html/bin/pod2html

clean:

realclean:
	rm -rf $(plextract) $(plextractexe)
	rm -f ../t/_h2ph_pre.ph

clobber:	realclean

distclean:	clobber

veryclean:	distclean
	-rm -f *~ *.org

# ex: set ro:
