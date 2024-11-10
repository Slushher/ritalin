.PHONY: cmake compile multi example run clean

cmake:
	cmake -B cmake

compile:
	cmake --build cmake

multi:
	$(MAKE) compile -j 16

example: cmake_example compile

cmake_example:
	cmake -B cmake -DBUILD_EXAMPLE=ON

run:
	./bin/ritalin

clean:
	rm -rf cmake
	rm -f bin/ritalin
