malloctest: mymalloc.o mallocdrv.o
	gcc -m32 -o malloctest mymalloc.o mallocdrv.o 
mymalloc.o: mymalloc.c mymalloc.h
	gcc -m32 -c mymalloc.c
mallocdrv.o: mymalloc.h mallocdrv.c
	gcc -m32 -c mallocdrv.c