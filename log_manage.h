#ifndef LOG_MANAGE_H
#define LOG_MANAGE_H

#define LOG_FILE "login.log"
#define LOG_DEBUG(msg) __LINE__,__FILE__,msg

int log_file(const char* logfile,int level,char *msg);
int log_file_debug(const char* logfile,int level,int line,const char* file,char* msg);
int log_console_debug(int level,int line,const char* file,char* msg);
int log_console(int level,char* msg);

#endif
