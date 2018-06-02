SRC = help.md\
      wizard.md\
	  main.md\
	  questionnaire.md\
	  logfile.md\
	  about.md
VERSION = 0.9.4
FLAGS = --self-contained --toc  --section-divs
META = --metadata=title:"EffortLog v$(VERSION)"
HTML_OUT = doc.html
PDF_OUT = doc.pdf


all : html pdf
html : 
	pandoc $(FLAGS) $(META) --to html5 --css doc.css -o $(HTML_OUT) -s $(SRC)
pdf : 
	pandoc $(FLAGS) $(META) -o $(PDF_OUT) $(SRC)
clean: 
	rm doc.html doc.pdf

