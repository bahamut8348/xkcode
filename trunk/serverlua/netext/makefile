
cflag = -c -g -Wno-deprecated -std=gnu++0x
lflag = -g -shared
objpath = obj

cc = g++-4.4
#cc = llvm-g++

netext.so : $(objpath)/netext.o $(objpath)/netmanager.o $(objpath)/buffer.o $(objpath)/appinfo.o $(objpath)/ae.o $(objpath)/zmalloc.o
	$(cc) $(lflag)  -o netext.so $(objpath)/netmanager.o $(objpath)/netext.o $(objpath)/buffer.o $(objpath)/appinfo.o $(objpath)/ae.o $(objpath)/zmalloc.o

$(objpath)/netext.o : netext.cpp
	- mkdir $(objpath)
	$(cc) $(cflag) $< -o $@

$(objpath)/netmanager.o : NetManager.cpp netmanager.h
	$(cc) $(cflag) $< -o $@

$(objpath)/buffer.o : buffer.cpp buffer.h
	$(cc) $(cflag) $< -o $@

$(objpath)/appinfo.o : appinfo.cpp appinfo.h
	$(cc) $(cflag) $< -o $@

$(objpath)/ae.o : ae.c ae.h
	gcc -c -g -std=c99 $< -o $@

$(objpath)/zmalloc.o : zmalloc.c zmalloc.h
	gcc -c -g -std=c99 $< -o $@

clean:
	rm -R $(objpath)


