tsort : main.cpp
	g++ -O2 -std=c++20 -o $@ $<

run : tsort
	./tsort

.PHONY : run
