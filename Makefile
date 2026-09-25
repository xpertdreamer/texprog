DOX ?= 0
DEBUG ?= 0

define run_doxygen
	@if [ "$(DOX)" = "1" ]; then \
		echo "Generating Doxygen documentation..."; \
		doxygen; \
	fi
endef

clean:
	rm -rf build/
	rm -rf docs/html/

lab1: lab_1/main.cpp lab_1/counter.cpp lab_1/cmd.cpp lab_1/stl.cpp
	@mkdir -p build/lab_1
	g++ -Wall -Wextra lab_1/main.cpp lab_1/counter.cpp lab_1/cmd.cpp lab_1/stl.cpp -o build/lab_1/lab1
	$(call run_doxygen)

lab2: lab_2/lab2.cpp lab_2/parser.cpp lab_2/uploader.cpp lab_2/validator.cpp
	@mkdir -p build/lab_2
	g++ -Wall -Wextra $^ -o build/lab_2/lab2
	$(call run_doxygen)

lab3s: lab_2/parser.cpp lab_2/uploader.cpp lab_2/validator.cpp
	@mkdir -p build/lab_3
	g++ -Wall -Wextra -c lab_2/parser.cpp lab_2/uploader.cpp lab_2/validator.cpp -DDEBUG_MODE=$(DEBUG)
	ar rvs build/lab_3/lab_2.a parser.o uploader.o validator.o
	rm -f parser.o uploader.o validator.o
	cp lab_2/parser.h lab_2/uploader.h lab_2/validator.h lab_3
	cp build/lab_3/lab_2.a lab_3
	g++ -Wall -Wextra lab_3/test.cpp -Llab_3 -l:lab_2.a -o build/lab_3/lab3
