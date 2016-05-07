#ifndef CONNECT_FUNC_H
#define CONNECT_FUNC_H


int init_socket(const char* ip,const int port);
int close_socket(const int socket_fd);
int read_socket(int sock_fd,char* read_buf,int read_size);
int write_socket(int sock_fd,char* write_buf);


#endif
