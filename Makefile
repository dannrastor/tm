tm: src/*.cpp include/*.h
	g++ -std=c++1z -o tm main.cpp src/*.cpp -I include
