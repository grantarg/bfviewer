CFLAGS = -Wall -Wextra -O -O2 -O3 -O4
RM = rm -f

default: all

all: bfviewer tests graph

bfviewer:
	g++-13 $(CFLAGS) -o bfviewer src/bfviewer.cpp

tests: bfviewer
	./bfviewer -m logs/LOG00001.01.csv > motors.jgr

graph: tests
	$(RM) *.jpg
	./jgraph -P motors.jgr | ps2pdf - motors.pdf
#convert -density 300 - -quality 100 motors.jpg

clean:
	$(RM) bfviewer