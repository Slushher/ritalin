.PHONY: cmake compile multi example run clean

cmake:
	cmake -B cmake

compile:
	cmake --build cmake

multi:
	$(MAKE) compile -j 16

example:
	cmake -B cmake -DBUILD_EXAMPLE=ON

run:
	./bin/ritalin

clean:
	rm cmake -rf
	rm -f bin/ritalin
	rm -f bin/ritalin_example