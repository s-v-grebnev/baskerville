CPPOPTS = -O3 -m64 -g -std=c++11 -Wall
CPP = g++
DEFS = 
INCLUDES = -I. -I/usr/include/x86_64-linux-gnu/c++/4.9 -I/usr/include
CCFLAGS = $(CCOPTS) $(DEFS) $(INCLUDES) 
CPPFLAGS = $(CPPOPTS) $(DEFS) $(INCLUDES) 
LIBS = -lgrpc++ -lprotobuf -lssl -lcrypto 
LDFLAGS = $(LIBS)
CPPOBJECTS = src/base64.o src/basket.grpc.pb.o src/basket.pb.o src/coptions.o src/rsa.o src/serverfileop.o src/soptions.o
EXEC = client server
DOCS = Doc/xanalysis.tex

OBJECTS = $(CPPOBJECTS) 
CLEANOBJECTS = $(TARGET) $(OBJECTS)  $(EXEC)

all: $(OBJECTS) $(EXEC)

release: $(OBJECTS) $(EXEC)
	strip $(EXEC)

client: src/client.cpp
	$(CPP) $(CPPFLAGS) src/client.cpp $(CPPOBJECTS) $(LDFLAGS) -o client  

server: src/server.cpp
	$(CPP) $(CPPFLAGS) src/server.cpp $(CPPOBJECTS) $(LDFLAGS) -o server  

.cpp.o:
	@$(CPP) $(CPPFLAGS) -c $*.cpp -o ./$*.o

.cc.o:
	@$(CPP) $(CPPFLAGS) -c $*.cc -o ./$*.o

doc: $(DOCS)
	xelatex Doc/xanalysis.tex
	rm -f xanalysis.aux xanalysis.log

clean:
	rm -f $(CLEANOBJECTS) 
	rm -f Doc/xanalysis.aux xanalysis.log
	rm -f xanalysis.aux xanalysis.log
	rm *.log