myHell: myShell.o
	g++ myShell.o -o myHell

MyShell.o: myShell.cpp
	g++ -c myShell.cpp

clean:
	rm *.o myHell