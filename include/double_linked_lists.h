#ifndef _DOUBLE_LINKED_LISTS_H_
#define _DOUBLE_LINKED_LISTS_H_

#define DEFINE_DOUBLE_LINKED_LIST(type)                               \
    typedef struct type##DLLNode {                                    \
        type data;                                                    \
        struct type##DLLNode *next;                                   \
        struct type##DLLNode *prev;                                   \
    } type##DLLNode;                                                  \
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
        type##DLLNode* new_node = malloc(sizeof(type##DLLNode));      \
        new_node->data = data;                                        \
        new_node->prev = NULL;                                        \
        new_node->next = NULL;                                        \
        return new_node;                                              \
    }

#endif
