CFLAGS = -Wall -Wextra -O -O2 -O3
RM = rm -f

default: all

all: bfviewer tests graph

bfviewer:
	g++ $(CFLAGS) -o bfviewer src/bfviewer.cpp

tests: bfviewer
	$(RM) *.pdf
	$(RM) *.jgr
	./bfviewer -m logs/LOG00001.01.csv > motors.jgr
	./bfviewer -s logs/LOG00001.01.csv > signal.jgr
	./bfviewer -a logs/LOG00001.01.csv > amperage.jgr
	./bfviewer -v logs/LOG00001.01.csv > vbat.jgr
	./bfviewer -e logs/LOG00001.01.csv > mahtotal.jgr

graph: tests
	jgraph -P motors.jgr | ps2pdf - motors.pdf
	jgraph -P signal.jgr | ps2pdf - signal.pdf
	jgraph -P amperage.jgr | ps2pdf - amperage.pdf
	jgraph -P vbat.jgr | ps2pdf - vbat.pdf
	jgraph -P mahtotal.jgr | ps2pdf - mahtotal.pdf

clean:
	$(RM) bfviewer