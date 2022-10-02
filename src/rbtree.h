#pragma once

enum {
	red,
	black
};

typedef struct rbtree {
	char *value;
	int key;
	struct rbtree *left;
	struct rbtree *right;
	struct rbtree *parent;
	int color;
} rbtree;

rbtree *rbtree_create(int key, char *value);
rbtree *rbtree_add(rbtree *root, int key, char *value);
rbtree *rbtree_delete(rbtree *root, int key);
rbtree *rbtree_transplant(rbtree *root, rbtree *node, rbtree *new_node);
rbtree *rbtree_delete_fixup(rbtree *root, rbtree *node);
rbtree *rbtree_lookup (rbtree *root, int key);
rbtree *rbtree_min(rbtree *root);
rbtree *rbtree_max(rbtree *root);
rbtree *rbtree_add_fixup(rbtree *root, rbtree *node);
rbtree *rbtree_rotate_left(rbtree *node);
rbtree *rbtree_rotate_right(rbtree *node);
void rbtree_print_dfs(struct rbtree *x, int H);