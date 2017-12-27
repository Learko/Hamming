CXX = g++-7
CXXFLAGS = -Wall --pedantic -O3 -std=c++1z

all: hamming noise


main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

encode.o: encode.cpp
	$(CXX) $(CXXFLAGS) -c encode.cpp

decode.o: decode.cpp
	$(CXX) $(CXXFLAGS) -c decode.cpp

noise.o: noise.cpp
	$(CXX) $(CXXFLAGS) -c noise.cpp


hamming: main.o encode.o decode.o
	$(CXX) main.o encode.o decode.o -o hamming

noise: noise.o
	$(CXX) noise.o -o noise

clean:
	rm *.o
