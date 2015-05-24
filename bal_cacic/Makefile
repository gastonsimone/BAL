PDFLATEX = pdflatex -file-line-error
RM = rm -f
TARGET = bal_cacic

.PHONY: pdf zip gzip clean

pdf: $(TARGET).tex
	$(PDFLATEX) $<
	$(PDFLATEX) $<

zip: pdf
	zip $(TARGET).pdf.zip $(TARGET).pdf

gzip: pdf
	gzip -c $(TARGET).pdf > $(TARGET).pdf.gz

clean:
	$(RM) *.log *.out *.aux *.dvi *.ps *.pdf *.zip *.gz

