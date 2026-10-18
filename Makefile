DOX ?= 0
DEBUG ?= 0
LAB ?= lab_4

ifeq ($(LAB), lab_4)
	TEST_INCS = -Ilab_4
endif

define run_doxygen
	@if [ "$(DOX)" = "1" ]; then \
		echo "Generating Doxygen documentation..."; \
		doxygen; \
	fi
endef

clean:
	rm -rf build/
	rm -rf docs/html/

test:
	@if [ ! -f "tests/test_$(LAB).cpp" ]; then \
		echo "Error: Test file tests/test_$(LAB).cpp not found!"; \
		exit 1; \
	fi
	@mkdir -p build/tests
	g++ -Wall -Wextra $(TEST_INCS) -Itests tests/test_$(LAB).cpp $(TEST_SRCS) -o build/tests/run_tests_$(LAB)
	./build/tests/run_tests_$(LAB)

lab1: lab_1/main.cpp lab_1/counter.cpp lab_1/cmd.cpp lab_1/stl.cpp
	@mkdir -p build/lab_1
	g++ -Wall -Wextra lab_1/main.cpp lab_1/counter.cpp lab_1/cmd.cpp lab_1/stl.cpp -o build/lab_1/lab1
	$(call run_doxygen)

lab2: lab_2/lab2.cpp lab_2/parser.cpp lab_2/uploader.cpp lab_2/validator.cpp
	@mkdir -p build/lab_2
	g++ -Wall -Wextra $^ -o build/lab_2/lab2 -Ofast -march=native
	$(call run_doxygen)

lab3s: lab_2/parser.cpp lab_2/uploader.cpp lab_2/validator.cpp lab_3/static/test.cpp
	@mkdir -p build/lab_3/obj

	g++ -Wall -Wextra -Ilab_2 -DDEBUG_MODE=$(DEBUG) \
		-c lab_2/parser.cpp    -o build/lab_3/obj/parser.o
	g++ -Wall -Wextra -Ilab_2 -DDEBUG_MODE=$(DEBUG) \
		-c lab_2/uploader.cpp  -o build/lab_3/obj/uploader.o
	g++ -Wall -Wextra -Ilab_2 -DDEBUG_MODE=$(DEBUG) \
		-c lab_2/validator.cpp -o build/lab_3/obj/validator.o

	ar rvs build/lab_3/liblab2.a \
		build/lab_3/obj/parser.o \
		build/lab_3/obj/uploader.o \
		build/lab_3/obj/validator.o

	g++ -Wall -Wextra -Ilab_2 -g -DDEBUG_MODE=$(DEBUG) \
		lab_3/static/test.cpp \
		-Lbuild/lab_3 -l:liblab2.a \
		-o build/lab_3/lab3s

lab3d: lab_2/parser.cpp lab_2/uploader.cpp lab_2/validator.cpp lab_3/dynamic/test.cpp
	@mkdir -p build/lab_3
	g++ -Wall -Wextra -Ilab_2 -DDEBUG_MODE=$(DEBUG) \
		-fPIC -shared \
		lab_2/parser.cpp lab_2/uploader.cpp lab_2/validator.cpp \
		-o build/lab_3/liblab2.so

		g++ -Wall -Wextra -Ilab_2 -DDEBUG_MODE=$(DEBUG) \
		lab_3/dynamic/test.cpp \
		-Lbuild/lab_3 -l:liblab2.so \
		-Wl,-rpath,'$$ORIGIN' \
		-o build/lab_3/lab3d

lab3r: lab_2/parser.cpp lab_2/uploader.cpp lab_3/runtime/api.cpp lab_3/runtime/test.cpp
	@mkdir -p build/lab_3
	g++ -Wall -Wextra -Ilab_2 -fPIC -shared -std=c++20 \
		lab_2/parser.cpp lab_2/uploader.cpp lab_3/runtime/api.cpp \
		-o build/lab_3/liblab2.so
	g++ -Wall -Wextra \
		lab_3/runtime/test.cpp -ldl \
		-o build/lab_3/lab3r

lab4: lab_4/lab4.cpp
	@mkdir -p build/lab_4
	g++ -Wall -Wextra -Ilab_4  $^ -o build/lab_4/lab4
	$(call run_doxygen)
