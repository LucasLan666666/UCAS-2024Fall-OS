TEXFILE = main
OUTFILE = hw10

all: clean $(OUTFILE).pdf

$(OUTFILE).pdf: $(TEXFILE).tex
	xelatex -jobname=$(OUTFILE) $(TEXFILE).tex
	xelatex -jobname=$(OUTFILE) $(TEXFILE).tex

clean:
	rm -f $(OUTFILE).pdf $(OUTFILE).aux $(OUTFILE).bbl $(OUTFILE).blg \
	$(OUTFILE).log $(OUTFILE).out $(OUTFILE).toc $(OUTFILE).lof $(OUTFILE).lot

.PHONY: all clean
