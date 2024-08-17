#ifndef ACCOUNT_INFO_H
#define ACCOUNT_INFO_H

#include "decTypes.h"

typedef struct accountInfo
{
    char account_str[128];
    char password_str[128];

    struct list_head list_node;
} accountInfo_t;


accountInfo_t* request_account_node_direct();                                             // ok
accountInfo_t* request_account_node(const char* account_str, const char* password_str);   // ok


void insert_account_node(accountInfo_t* new_head, accountInfo_t* head_node);   // ok

accountInfo_t* find_account_node(accountInfo_t* head_node, const char* find_account_str);   // ok
accountInfo_t* find_match_account_node(accountInfo_t* head_node,
                                       const char*    find_account_str,   // ok
                                       const char*    match_password_str);

void destroy_all_account_node(accountInfo_t* head_node);   // ok


#endif