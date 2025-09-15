CC = g++-14
CCFLAGS = --std=c++23 -O3

all: setup main_simple main_ranges main_ranges_not_stored

setup:
	mkdir bin obj

main_simple: obj/main_simple.o
	$(CC) $(CCFLAGS) -o bin/$@.out $^

obj/main_simple.o: src/main_simple.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

main_ranges: obj/main_ranges.o
	$(CC) $(CCFLAGS) -o bin/$@.out $^

obj/main_ranges.o: src/main_ranges.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

main_ranges_not_stored: obj/main_ranges_not_stored.o
	$(CC) $(CCFLAGS) -o bin/$@.out $^

obj/main_ranges_not_stored.o: src/main_ranges_not_stored.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -f bin/* obj/*