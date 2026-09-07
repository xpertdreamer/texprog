clean:
	rm -rf build/

lab_1: lab_1/main.cpp
	@mkdir -p build/lab_1
	g++ -Wall -Wextra lab_1/main.cpp -o build/lab_1/lab_1
