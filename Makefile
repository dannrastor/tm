tm: src/*.cpp include/*.h main.cpp
	g++ -std=c++1z -o tm main.cpp src/*.cpp -I include

archive.tar: src/*.cpp include/*.h main.cpp
	mkdir tmp
	cp src/*.cpp tmp
	cp include/*.h tmp
	cp main.cpp tmp	
	tar -cf archive.tar tmp/* 
	rm -rf tmp
