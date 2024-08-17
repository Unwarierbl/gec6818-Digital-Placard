#include "accountInfo.h"


accountInfo_t* request_account_node_direct()
{
    accountInfo_t* new_account = (accountInfo_t*)malloc(sizeof(accountInfo_t));
    INIT_LIST_HEAD(&new_account->list_node);

    return new_account;
}

accountInfo_t* request_account_node(const char* account_str, const char* password_str)
{
    accountInfo_t* new_account = request_account_node_direct();
    int            copy_len;

    copy_len = strlen(account_str);
    if (copy_len >= sizeof(new_account->account_str)) {
        copy_len = sizeof(new_account->account_str) - 1;
    }
    memset(new_account->account_str, 0, sizeof(new_account->account_str));
    strncpy(new_account->account_str, account_str, copy_len);

    copy_len = strlen(password_str);
    if (copy_len >= sizeof(new_account->password_str)) {
        copy_len = sizeof(new_account->password_str) - 1;
    }
    memset(new_account->password_str, 0, sizeof(new_account->password_str));
    strncpy(new_account->password_str, password_str, copy_len);

    return new_account;
}

void insert_account_node(accountInfo_t* new_head, accountInfo_t* head_node)
{
    list_add(&new_head->list_node, &head_node->list_node);
}


accountInfo_t* find_account_node(accountInfo_t* head_node, const char* find_account_str)
{
    if (list_empty(&head_node->list_node)) {
        return head_node;
    }
    accountInfo_t* pos;

    list_for_each_entry(pos, &head_node->list_node, list_node)
    {
        if (strcmp(pos->account_str, find_account_str) == 0) {
            break;
        }
    }
    return pos;
}

accountInfo_t* find_match_account_node(accountInfo_t* head_node, const char* find_account_str,
                                       const char* match_password_str)
{
    accountInfo_t* find_node = find_account_node(head_node, find_account_str);
    if (find_node == head_node) {
        return head_node;
    }

    if (strcmp(find_node->password_str, match_password_str) == 0) {
        return find_node;
    }
    else {
        return head_node;
    }
}

void destroy_all_account_node(accountInfo_t* head_node)
{
    if (list_empty(&head_node->list_node)) {
        free(head_node);
        return;
    }

    accountInfo_t *pos, *next;

    list_for_each_entry_safe(pos, next, &head_node->list_node, list_node)
    {
        free(pos);
    }
    free(head_node);
}
