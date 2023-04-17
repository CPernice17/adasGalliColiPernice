finalFile: hmiInput centralECU hmi install clean

hmiInput: hmiInput.o	commonFunctions.o
		gcc -o hmiInput hmiInput.o commonFunctions.o

centralECU: centralECU.o	commonFunctions.o
		gcc -o centralECU centralECU.o commonFunctions.o

hmi: hmi.o	commonFunctions.o
		gcc -o hmi hmi.o commonFunctions.o

hmiInput.o: ./src/interface/hmiInput.c	./src/header/commonFunctions.h
		gcc -c ./src/interface/hmiInput.c

centralECU.o: ./src/control/centralECU.c	./src/header/commonFunctions.h
		gcc -c ./src/control/centralECU.c

hmi.o: ./src/interface/hmi.c	./src/header/commonFunctions.h
		gcc -c ./src/interface/hmi.c

commonFunctions.o: ./src/header/commonFunctions.c	./src/header/commonFunctions.h
		gcc -c ./src/header/commonFunctions.c

install:
	mkdir -p ./bin
	mv -f hmiInput ./bin/interface
	mv -f centralECU ./bin/control
	mv -f hmi ./bin/interface

clean:
	rm *.o