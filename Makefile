# build an executable named myprog from myprog.c
all: csv-reader.cpp
	g++ -o csv-reader util.cpp config.cpp structs.cpp operators.cpp csv-reader.cpp
	g++ -o csv-reader-test util.cpp config.cpp structs.cpp operators.cpp test.cpp csv-reader-test.cpp

reader: csv-reader.cpp
	g++ -o csv-reader util.cpp config.cpp structs.cpp operators.cpp csv-reader.cpp

test: csv-reader-test.cpp
	g++ -o csv-reader-test util.cpp config.cpp structs.cpp operators.cpp test.cpp csv-reader-test.cpp

clean:
	$(RM) csv-reader csv-reader-test
