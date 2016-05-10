CC=gcc
CFLAGS=-g -O1 -std=c99 

ftp_magic : ftp_main.o connect_func.o log_manage.o ftp_func.o utils.o
	$(CC) $(CFLAGS) -o magic_ftp ftp_main.o connect_func.o log_manage.o ftp_func.o utils.o

ftp_func.o: ftp_func.c connect_func.h log_manage.h ftp_func.h utils.h
	$(CC) $(CFLAGS) -c ftp_func.c
connect_func.o: connect_func.c connect_func.h log_manage.h
	$(CC) $(CFLAGS) -c connect_func.c
log_manage.o: log_manage.c log_manage.h
	$(CC) $(CFLAGS) -c log_manage.c
ftp_main.o: ftp_main.c ftp_func.h
	$(CC) $(CFLAGS) -c ftp_main.c
utils.o: utils.c utils.h 
	$(CC) $(CFLAGS) -c utils.c
	
clean:
	-rm magic_ftp ftp_main.o connect_func.o log_manage.o ftp_func.o utils.o

.PHONY:clean

