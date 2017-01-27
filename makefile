CXX=g++

all:
	mpic++ -std=c++14 master.cpp -o master
	mpic++ -std=c++14 Communicator.cc spawn.cpp -o spawn
	mv spawn spawn_dir/.
	$(CXX) -std=c++14 printer.cpp -o printer
