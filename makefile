finalFile: hmiInput forwardFacingRadar frontWindshieldCamera \
parkAssist surroundViewCameras brakeByWire steerByWire throttleControl \
centralECU hmi install clean

hmiInput: hmiInput.o	commonFunctions.o
		gcc -o hmiInput hmiInput.o commonFunctions.o

forwardFacingRadar: forwardFacingRadar.o	socketFunctions.o	commonFunctions.o
		gcc -o forwardFacingRadar forwardFacingRadar.o socketFunctions.o commonFunctions.o

frontWindshieldCamera: frontWindshieldCamera.o	socketFunctions.o	commonFunctions.o
		gcc -o frontWindshieldCamera frontWindshieldCamera.o socketFunctions.o commonFunctions.o

parkAssist: parkAssist.o	socketFunctions.o	commonFunctions.o
		gcc -o parkAssist parkAssist.o socketFunctions.o commonFunctions.o

surroundViewCameras: surroundViewCameras.o	socketFunctions.o	commonFunctions.o
		gcc -o surroundViewCameras surroundViewCameras.o socketFunctions.o commonFunctions.o

brakeByWire: brakeByWire.o	commonFunctions.o
		gcc -o brakeByWire brakeByWire.o commonFunctions.o

steerByWire: steerByWire.o	commonFunctions.o
		gcc -o steerByWire steerByWire.o commonFunctions.o

throttleControl: throttleControl.o	commonFunctions.o
		gcc -o throttleControl throttleControl.o commonFunctions.o

centralECU: centralECU.o	commonFunctions.o	socketFunctions.o
		gcc -o centralECU centralECU.o commonFunctions.o socketFunctions.o

hmi: hmi.o	commonFunctions.o
		gcc -o hmi hmi.o commonFunctions.o

hmiInput.o: ./src/interface/hmiInput.c	./src/header/commonFunctions.h
		gcc -c ./src/interface/hmiInput.c

forwardFacingRadar.o: ./src/sensors/forwardFacingRadar.c	./src/header/socketFunctions.h	./src/header/commonFunctions.h
		gcc -c ./src/sensors/forwardFacingRadar.c

frontWindshieldCamera.o: ./src/sensors/frontWindshieldCamera.c	./src/header/socketFunctions.h	./src/header/commonFunctions.h
		gcc -c ./src/sensors/frontWindshieldCamera.c

parkAssist.o: ./src/sensors/parkAssist.c	./src/header/socketFunctions.h	./src/header/commonFunctions.h
		gcc -c ./src/sensors/parkAssist.c

surroundViewCameras.o: ./src/sensors/surroundViewCameras.c	./src/header/socketFunctions.h	./src/header/commonFunctions.h
		gcc -c ./src/sensors/surroundViewCameras.c

brakeByWire.o: ./src/actuators/brakeByWire.c	./src/header/commonFunctions.h
		gcc -c ./src/actuators/brakeByWire.c

steerByWire.o: ./src/actuators/steerByWire.c	./src/header/commonFunctions.h
		gcc -c ./src/actuators/steerByWire.c

throttleControl.o: ./src/actuators/throttleControl.c	./src/header/commonFunctions.h
		gcc -c ./src/actuators/throttleControl.c

centralECU.o: ./src/control/centralECU.c	./src/header/commonFunctions.h	./src/header/socketFunctions.h
		gcc -c ./src/control/centralECU.c

hmi.o: ./src/interface/hmi.c	./src/header/commonFunctions.h
		gcc -c ./src/interface/hmi.c

commonFunctions.o: ./src/header/commonFunctions.c	./src/header/commonFunctions.h
		gcc -c ./src/header/commonFunctions.c

socketFunctions.o: ./src/header/socketFunctions.c	./src/header/socketFunctions.h
		gcc -c ./src/header/socketFunctions.c

install:
	mkdir -p ./bin
	mv -f hmiInput ./bin/interface
	mv -f hmi ./bin/interface
	mv -f centralECU ./bin/control
	mv -f forwardFacingRadar ./bin/sensors
	mv -f frontWindshieldCamera ./bin/sensors
	mv -f parkAssist ./bin/sensors
	mv -f surroundViewCameras ./bin/sensors
	mv -f brakeByWire ./bin/actuators
	mv -f steerByWire ./bin/actuators
	mv -f throttleControl ./bin/actuators

clean:
	rm *.o