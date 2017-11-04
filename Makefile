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
DOCS = doc/xanalysis.pdf

OBJECTS = $(CPPOBJECTS) 
CLEANOBJECTS = $(TARGET) $(OBJECTS) src/client src/server

all: $(OBJECTS) $(EXEC)

release: $(OBJECTS) $(EXEC)
	strip $(EXEC)

client: src/client.cpp
	$(CPP) $(CPPFLAGS) src/client.cpp $(CPPOBJECTS) $(LDFLAGS) -o src/client  

server: src/server.cpp
	$(CPP) $(CPPFLAGS) src/server.cpp $(CPPOBJECTS) $(LDFLAGS) -o src/server  

.cpp.o:
	@$(CPP) $(CPPFLAGS) -c $*.cpp -o ./$*.o

.cc.o:
	@$(CPP) $(CPPFLAGS) -c $*.cc -o ./$*.o

doc: $(DOCS) 

doc/xanalysis.pdf: doc/xanalysis.tex
	xelatex doc/xanalysis.tex
	rm -f xanalysis.aux xanalysis.log
	mv xanalysis.pdf doc/

clean:
	rm -f $(CLEANOBJECTS) 
	rm -f doc/xanalysis.aux doc/xanalysis.log
	rm -f xanalysis.aux xanalysis.log
	rm -f *.log

distclean: clean
	rm -f bin/client bin/server

install: client server
	cp src/client bin/
	cp src/server bin/