CPPOPTS = -O3 -m64 -g -std=c++11 -Wall
CPP = g++
DEFS = 
INCLUDES = -I. -I/usr/include
CCFLAGS = $(CCOPTS) $(DEFS) $(INCLUDES) 
CPPFLAGS = $(CPPOPTS) $(DEFS) $(INCLUDES) 
LIBS = -lgrpc++ -lprotobuf -lssl -lcrypto -lpthread
LDFLAGS = $(LIBS)
CPPOBJECTS = src/base64.o src/basket.grpc.pb.o src/basket.pb.o src/coptions.o src/rsa.o src/serverfileop.o src/soptions.o
EXEC = client server
DOCS = doc/xanalysis.pdf

OBJECTS = $(CPPOBJECTS) 
CLEANOBJECTS = $(TARGET) $(OBJECTS) src/client src/server src/client.o src/server.o

all: $(OBJECTS) $(EXEC)

client: src/client.o $(OBJECTS)
#	$(CPP) $(CPPFLAGS) src/client.cpp $(CPPOBJECTS) $(LDFLAGS) -o src/client  
	$(CPP)  src/client.o $(OBJECTS) $(LDFLAGS) -o src/client
	strip src/client -o bin/client

server: src/server.o  $(OBJECTS)
	$(CPP)  src/server.o $(OBJECTS) $(LDFLAGS) -o src/server
	strip src/server -o bin/server

.cpp.o:
	@$(CPP) $(CPPFLAGS) -c $*.cpp -o ./$*.o

.cc.o:
	@$(CPP) $(CPPFLAGS) -c $*.cc -o ./$*.o

doc: $(DOCS) 

doc/xanalysis.pdf: doc/xanalysis.tex
	xelatex doc/xanalysis.tex
	rm -f xanalysis.aux xanalysis.log doc/xanalysis.synctex.gz
	mv xanalysis.pdf doc/

clean:
	rm -f $(CLEANOBJECTS) 
	rm -f doc/xanalysis.aux doc/xanalysis.log
	rm -f xanalysis.aux xanalysis.log
	rm -f *.log

distclean: clean
	rm -f bin/client bin/server
