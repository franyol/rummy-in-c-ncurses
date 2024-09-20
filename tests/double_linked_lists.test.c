#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "../include/double_linked_lists.h"

typedef struct TestData {
	int value;
} TestData;

int test_data_eq(TestData a, TestData b) {
	return (a.value == b.value);
}

DEFINE_DOUBLE_LINKED_LIST(TestData);

void printList(TestDataDLLNode* head) {
	for ( ;head != NULL; head = head->next ) {
		printf("TestDataNode: %d ", head->data.value);
	}
	printf("\n");
}

int compArray(TestDataDLLNode* head, int array[], size_t len) {
	for (size_t i=0; i < len; i++, head = head->next) {
		if (head == NULL || array[i] != head->data.value)
			return 0;
	}
	return 1;
}

void test_create() {
	TestData temp = { 10 };
	int i;
	TestDataDLLNode* node;
	TestDataDLLNode* head;

	temp.value = 0;
	head = TestData_create_new_node(temp);
	for ( i = 1; i < 10; i++ ) {
		temp.value = i;
		TestData_dll_append(head, TestData_create_new_node(temp));
	}

	for ( i = 0, node=head; i < 10; i++, node = node->next ) {
		temp.value = i;
		assert(test_data_eq(temp, node->data));
	}
	TestData_free_all(head);
}

void test_swap() {
	int input[] = {9, 3, 4, 6, 2, 5};
	int expected[] = {9, 6, 5, 4, 3, 2};
	TestDataDLLNode *head, *iNode, *jNode, *temp;
	size_t len = sizeof(input)/sizeof(input[0]);
	TestData data;
	
	data.value = input[0];
	head = TestData_create_new_node(data);
	for (int i = 1; i < len; i++) {
		data.value = input[i];
		TestData_dll_append(head, TestData_create_new_node(data));
	}
	
	// Order the array using swap
	for (iNode = head; iNode->next != NULL; iNode = iNode->next) {
		for (jNode = iNode->next; jNode != NULL; jNode = jNode->next) {
			if (iNode->data.value < jNode->data.value) {
				TestData_dll_swap_nodes(iNode, jNode);
				temp = jNode;
				jNode = iNode;
				iNode = temp;
			}
		}
	}

	assert(compArray(head, expected, len));
	TestData_free_all(head);
}

void test_pop_insert() {
	int input[] = {9, 3, 4, 11, 2, 5};
	int expected[] = {11, 9, 5, 4, 3, 2};
	TestDataDLLNode *head, *iNode, *jNode, *temp;
	size_t len = sizeof(input)/sizeof(input[0]);
	TestData data;
	
	data.value = input[0];
	head = TestData_create_new_node(data);
	for (int i = 0; i < len; i++) {
		data.value = input[i];
		TestData_dll_append(head, TestData_create_new_node(data));
	}
	
	// Order the array using swap
	for (iNode = head->next; iNode->next != NULL; iNode = iNode->next) {
		for (jNode = iNode->next; jNode != NULL; jNode = jNode->next) {
			if (iNode->data.value < jNode->data.value) {
				temp = iNode->prev;
				TestData_pop_from_list(iNode);
				TestData_dll_insert_after(jNode, iNode);
				TestData_pop_from_list(jNode);
				TestData_dll_insert_after(temp, jNode);
				temp = jNode;
				jNode = iNode;
				iNode = temp;
			}
		}
	}

	assert(compArray(head->next, expected, len));
	TestData_free_all(head);
}

int main() {
	test_create();
	printf("test_create: OK\n");
	test_swap();
	printf("test_swap: OK\n");
	test_pop_insert();
	printf("test_pop_insert: OK\n");

	printf("all tests passed\n");
	exit(EXIT_SUCCESS);
}
