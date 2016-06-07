#ifndef UTILS_H
#define UTILS_H

int remove_reply_code(char* reply);
int split_list_info(char* reply);
int get_pasv_port(char* reply);
int get_reply_code(char* reply);
int compare_reply_code(char* read_buf,const char* reply_code);

#endif
