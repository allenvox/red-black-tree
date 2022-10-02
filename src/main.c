#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "rbtree.h"

int main()
{
	srand(time(NULL));
	struct rbtree *root = rbtree_add(NULL, 1, "1");
	for(int i = 2; i <= 50000; i++) {
		char buffer[20];
		sprintf(buffer, "%d", i);
		root = rbtree_add(root, i, buffer);
	}
	for(int i = 0; i < 1000; i++) {
		rbtree *node = rbtree_lookup(root, rand() % 50000);
		if(node == NULL) {
			printf("not found :(\n");
		}
	}
	int H = 0;
	rbtree_print_dfs(root, H);
	root = rbtree_delete(root, 100);
	rbtree *node = rbtree_lookup(root, 100);
	if(node == NULL) {
		printf("not found - deleted :(\n");
	}
	return 0;
}