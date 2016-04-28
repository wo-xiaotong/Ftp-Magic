
ftp_magic : ftp_main.o connect_func.o log_manage.o
	cc -o ftp_magic ftp_main.o connect_func.o log_manage.o
	
ftp_main.o: ftp_main.c connect_func.h log_manage.h
connect_func.o: connect_func.c connect_func.h log_manage.h
log_manage.o: log_manage.c log_manage.h

clean:
	-rm ftp_magic ftp_main.o connect_func.o log_manage.o

.PHONY:clean

