clean:
	rm -rf build/
	rm -rf docs/html/

lab1: lab_1/main.cpp lab_1/counter.cpp lab_1/cmd.cpp lab_1/stl.cpp
	@mkdir -p build/lab_1
	g++ -Wall -Wextra lab_1/main.cpp lab_1/counter.cpp lab_1/cmd.cpp lab_1/stl.cpp -o build/lab_1/lab1
	doxygen
