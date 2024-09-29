TEXFILE = hw1

all: $(TEXFILE).pdf

$(TEXFILE).pdf: $(TEXFILE).tex
	xelatex $(TEXFILE).tex
	xelatex $(TEXFILE).tex

clean:
	rm -f $(TEXFILE).pdf $(TEXFILE).aux $(TEXFILE).bbl $(TEXFILE).blg $(TEXFILE).log $(TEXFILE).out $(TEXFILE).toc $(TEXFILE).lof $(TEXFILE).lot

.PHONY: all clean