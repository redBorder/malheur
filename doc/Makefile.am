# MALHEUR - Automatic Analysis of Malware Behavior
# Copyright (c) 2009-2015 Konrad Rieck (konrad@mlsec.org)
# University of Goettingen, Berlin Institute of Technology
#

EXTRA_DIST = example.cfg malheur.pod malheur.man malheur.txt malheur.pdf \
             doxygen.cfg modules latex-pre.inc.in latex-post.inc

doc_DATA = example.cfg
man1_MANS = malheur.man 

malheur.man: malheur.pod
	pod2man -c "User Manual" -s 1 -r "Malheur $(VERSION)" malheur.pod > $@

malheur.txt: malheur.pod
	pod2text -w 70 malheur.pod > $@

malheur.pdf: malheur.pod latex-pre.inc latex-post.inc
	pod2latex -prefile $(top_builddir)/doc/latex-pre.inc \
	          -postfile $(top_srcdir)/doc/latex-post.inc \
	          -full -out $(top_builddir)/doc/malheur.tex \
	          $(top_srcdir)/doc/malheur.pod
	pdflatex $(top_builddir)/doc/malheur.tex
	pdflatex $(top_builddir)/doc/malheur.tex	
	pdflatex $(top_builddir)/doc/malheur.tex	
	rm -f *.tex *.log *.aux *.toc

latex-pre.inc: latex-pre.inc.in
	sed -e 's|__VERSION__|$(VERSION)|' \
        	$(top_srcdir)/doc/latex-pre.inc.in \
        	> $(top_builddir)/doc/latex-pre.inc

distclean-local:
	rm -f malheur.pdf malheur.man malheur.txt latex-pre.inc

dist-hook: distclean-local malheur.pdf malheur.man malheur.txt
