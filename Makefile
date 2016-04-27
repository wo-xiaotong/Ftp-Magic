ftp_magic : ftp_main.o connect_func.o
	gcc -o ftp_magic ftp_main.o connect_func.o

ftp_main.o : ftp_main.c connect_func.h
	gcc -c ftp_main.c

connect_func.o : connect_func.c connect_func.h
	gcc -c connect_func.c

clean:
	-rm ftp_magic ftp_main.o connect_func.o

.PHONY:clean

