all:	encode\
    	decode\
	both.o

both.o: both.c header.h
	gccx -c both.c

encode:	encode.c both.o
	gccx -o encode encode.c both.o

decode: decode.c both.o
	gccx -o decode decode.c both.o

clean:	
	rm -f encode decode *.o
