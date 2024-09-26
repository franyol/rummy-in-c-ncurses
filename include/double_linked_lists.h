#ifndef _DOUBLE_LINKED_LISTS_H_
#define _DOUBLE_LINKED_LISTS_H_

#include <stdlib.h>

#define DECLARE_DOUBLE_LINKED_LIST(type) \
    typedef struct type##DLLNode {                                    \
        type data;                                                    \
        struct type##DLLNode *next;                                   \
        struct type##DLLNode *prev;                                   \
    } type##DLLNode;                                                  \
                                                                      \
    void type##_dll_append(type##DLLNode *head, type##DLLNode *new_node); \
                                                                      \
    void type##_dll_insert_after(type##DLLNode *node, type##DLLNode *new_node); \
                                                                      \
    void type##_dll_swap_nodes(type##DLLNode *n1, type##DLLNode *n2); \
                                                                      \
    void type##_pop_from_list(type##DLLNode *node);                   \
																	  \
	void type##_free_all(type##DLLNode *head);						  \
                                                                      \
    type##DLLNode* type##_create_new_node(type data);				  \
																	  \
	int type##_dll_len(type##DLLNode *head);                          \
                                                                      \
	int type##_dll_idx(type##DLLNode *node); 						  \
                                                                      \
	type##DLLNode* type##_dll_get_by_index(type##DLLNode *head, int idx);


#define DEFINE_DOUBLE_LINKED_LIST(type)                               \
                                                                      \
    void type##_dll_append(type##DLLNode *head, type##DLLNode *new_node) { \
        while ( head->next != NULL ) head = head->next;               \
        head->next = new_node;                                        \
        new_node->prev = head;                                        \
    }                                                                 \
                                                                      \
    void type##_dll_insert_after(type##DLLNode *node, type##DLLNode *new_node) { \
        type##DLLNode *temp;                                          \
        temp = node->next;                                            \
        node->next = new_node;                                        \
        new_node->prev = node;                                        \
        if ( temp != NULL ) {                                         \
            temp->prev = new_node;                                    \
        }                                                             \
        new_node->next = temp;                                        \
    }                                                                 \
                                                                      \
    void type##_dll_swap_nodes(type##DLLNode *n1, type##DLLNode *n2) { \
        type##DLLNode *temp;                                          \
                                                                      \
        temp = n2->prev;                                              \
        if (n1->prev != n2) n2->prev = n1->prev;                      \
		else n2->prev = n1;											  \
        if (temp != n1) n1->prev = temp;		                      \
		else n1->prev = n2;											  \
                                                                      \
        temp = n2->next;                                              \
        if (n1->next != n2) n2->next = n1->next;                      \
		else n2->next = n1;											  \
        if (temp != n1) n1->next = temp;		                      \
		else n1->next = n2;											  \
																	  \
		if (n1->next != NULL) n1->next->prev = n1;					  \
		if (n1->prev != NULL) n1->prev->next = n1;					  \
		if (n2->next != NULL) n2->next->prev = n2;					  \
		if (n2->prev != NULL) n2->prev->next = n2;					  \
    }                                                                 \
                                                                      \
    void type##_pop_from_list(type##DLLNode *node) {                  \
        if ( node->next != NULL ) node->next->prev = node->prev;      \
        if ( node->prev != NULL ) node->prev->next = node->next;      \
        node->next = NULL;                                            \
        node->prev = NULL;                                            \
    }                                                                 \
																	  \
	void type##_free_all(type##DLLNode *head) {						  \
		type##DLLNode *next;                                          \
		while (head != NULL) {                                        \
			next = head->next;                                        \
			free(head);                                               \
			head = next;                                              \
		}                                                             \
	}                                                                 \
                                                                      \
    type##DLLNode* type##_create_new_node(type data) {                \
        type##DLLNode* new_node = (type##DLLNode*) malloc(sizeof(type##DLLNode));      \
        new_node->data = data;                                        \
        new_node->prev = NULL;                                        \
        new_node->next = NULL;                                        \
        return new_node;                                              \
    }																  \
                                                                      \
	int type##_dll_idx(type##DLLNode *node) {						  \
		int count = 0;                                                \
		while (node != NULL && node->prev != NULL) {                  \
			count++; node = node->prev;                               \
		}                                                             \
		return count;                                                 \
	}\
																	  \
	int type##_dll_len(type##DLLNode *head) {						  \
		int count = 0;                                                \
		while (head != NULL) {                                        \
			count++; head = head->next;                               \
		}                                                             \
		return count;                                                 \
	}\
 \
	type##DLLNode* type##_dll_get_by_index(type##DLLNode *head, int idx) { \
		for (int i = 0; i < idx; i++, head = head->next) { \
			if ( head == NULL || head->next == NULL ) break; \
		} \
		return head; \
	} 

typedef struct NodeState {
	void *who;
	void *prev;
	void *next;
} NodeState;

DECLARE_DOUBLE_LINKED_LIST(NodeState);

#define dll_save_state(type, head, dest) \
	type *_node; \
	NodeState temp = {(void*) head, (void*) head->prev, (void*) head->next}; \
	NodeState_free_all(dest); \
    dest = NodeState_create_new_node(temp); \
	for (_node=head->next; _node != NULL; _node=_node->next) { \
		temp.who = (void*) _node; \
		temp.prev = (void*) _node->prev; \
		temp.next = (void*) _node->next; \
		NodeState_dll_append(dest, NodeState_create_new_node(temp)); \
	}

#define dll_load_state(type, src) \
	type *_node; \
	NodeStateDLLNode *src_node = src; \
	for (src_node = src; src_node != NULL; src_node = src_node->next) { \
		_node = (type*) src_node->data.who; \
		_node->next = (type*) src_node->data.next; \
		_node->prev = (type*) src_node->data.prev; \
	}

#endif
