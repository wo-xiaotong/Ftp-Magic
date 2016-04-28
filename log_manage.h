#ifndef LOG_MANAGE_H
#define LOG_MANAGE_H

//#define LOG_FILE "login.log"

int log_2_file(const char *file,const int level,const char* msg);
int log_2_console(const int level,const char* msg);

#endif
