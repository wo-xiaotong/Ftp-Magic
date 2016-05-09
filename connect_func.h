#ifndef CONNECT_FUNC_H
#define CONNECT_FUNC_H


#define WRITE_BUFSIZEE BUFSIZ


int init_socket(const char* ip,const int port);
int close_socket(const int socket_fd);
int read_socket(int sock_fd,char * content,int size);
int write_socket_v(int sock_fd,const char* format,...);
int write_socket(int sock_fd,char* write_buf);

int read_after_write(int sock_fd,char* read_buf,int read_size,const char* write_format,...)

#endif
