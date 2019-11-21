SrcFile :=../src
OutDir :=../temp
MakeDirCommand := mkdir -p

CXX :=g++
CXXFLAGS := -g --std=c++11
CC	:=gcc
CFLAGS :=
LDFLAGS :=
LIBS	:= -latomic
OPENCVFLAGS	:= `pkg-config --cflags opencv4`
OPENCVLIBS	:= `pkg-config --libs opencv4`


OBJS	:= $(OutDir)/face_detecter.o $(OutDir)/face_predict.o $(OutDir)/main.o

.PHONY: all clean

all : $(OBJS)
	@$(MakeDirCommand) $(OutDir)
	$(CXX) $(LDFLAGS) $(OBJS) -o main $(LIBS) $(OPENCVLIBS)
	@echo "Success Build"

$(OutDir)/face_detecter.o : $(SrcFile)/face_detecter.cpp
		$(CXX) $(CXXFLAGS) $(OPENCVFLAGS) -c $(SrcFile)/face_detecter.cpp -o $(OutDir)/face_detecter.o

$(OutDir)/face_predict.o : $(SrcFile)/face_predict.cpp
		$(CXX) $(CXXFLAGS) $(OPENCVFLAGS) -c $(SrcFile)/face_predict.cpp -o $(OutDir)/face_predict.o

$(OutDir)/main.o : $(SrcFile)/main.cpp
		$(CXX) $(CXXFLAGS) $(OPENCVFLAGS) -c $(SrcFile)/main.cpp -o $(OutDir)/main.o

clean:
	@echo "***clean***"
	-rm main $(OBJS)