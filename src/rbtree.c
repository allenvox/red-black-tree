#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rbtree.h"

rbtree empty = {"0", 0, NULL, NULL, NULL, black};
rbtree *leafNode = &empty;

rbtree *rbtree_create(int key, char *value)
{
	rbtree *node = malloc(sizeof(rbtree));
	if (node == NULL) {
		return NULL;
	}
	node->key = key;
	node->value = value;
	node->color = red;
	node->left = leafNode;
	node->right = leafNode;
	node->parent = leafNode;
	return node;
}

rbtree *rbtree_add(rbtree *root, int key, char *value) {
	struct rbtree *node = root, *parent = NULL;
	while (node != leafNode && node != NULL) {
		parent = node;
		if (key < node->key) {
			node = node->left;
		} else if (key > node->key) {
			node = node->right;
		} else { // already exists
			return root;
		}
	}
	node = rbtree_create(key, value);
	if (node == NULL) {
		return root;
	}
	node->parent = parent;
	if (root == NULL) {
		root = node;
		root->color = black;
		return root;
	} else if (key < parent->key) {
		parent->left = node;
	} else if (key > parent->key) {
		parent->right = node;
	}
	root = rbtree_add_fixup(root, node);
	return root;
}

rbtree *rbtree_delete(rbtree *tree, int key)
{
	rbtree *node, *node_trans;
	int node_color;
	node = rbtree_lookup(tree, key);
	if (node == NULL) {
		return tree;
	}
	node_color = node->color;
	if (node->left != NULL && node->right == leafNode) { // no left subtree
		node_trans = node->left;
		tree = rbtree_transplant(tree, node, node->left);
	} else if (node->right != NULL && node->left == leafNode) { // no right subtree
		node_trans = node->right;
		tree = rbtree_transplant(tree, node, node->right);
	} else { // has both subtrees
		rbtree *node_old = node;
		node = rbtree_min(node->right);
		node_color = node->color;
		node_trans = node->right;
		if (node->parent == node_old) {
			node_trans->parent = node;
		} else {
			tree = rbtree_transplant(tree, node, node->right);
			node->right = node_old->right;
			node->right->parent = node;
		}
		tree = rbtree_transplant(tree, node_old, node);
		node->left = node_old->left;
		node->left->parent = node;
		node->color = node_old->color;
	}
	if (node_color == black) { // restore red-black properties
		tree = rbtree_delete_fixup(tree, node_trans);
	}
	return tree;
}

rbtree *rbtree_delete_fixup(rbtree *tree, rbtree *node)
{
	rbtree *rotated;
	while (node != tree && node->color == black) {
		if (node == node->parent->left) { // node is left child
			rbtree *node_br = node->parent->right;
			if (node_br != NULL && node_br->color == red) { // case 1 - brother is red
				node_br->color = black;
				node->parent->color = red;
				rotated = rbtree_rotate_left(node->parent);
				if (rotated->parent == NULL) {
					tree = rotated;
				}
				node_br = node->parent->right;
			}
			if (node_br->left->color == black && node_br->right->color == black) { // case 2 - 
				node_br->color = red;
				node = node->parent;
			} else {
				if (node_br->right->color == black) { // case 3 - 
					node_br->left->color = black;
					node_br->color = red;
					rotated = rbtree_rotate_right(node_br);
					if (rotated->parent == NULL) {
						tree = rotated;
					}
					node_br = node->parent->right;
				} // case 4 - brothers right is red
				node_br->color = node->parent->color;
				node->parent->color = black;
				node_br->right->color = black;
				rotated = rbtree_rotate_left(node->parent);
				if (rotated->parent == NULL) {
					tree = rotated;
				}
				node = tree;
			}
		} else { // similar
			rbtree *node_br = node->parent->left;
			if (node_br != NULL && node_br->color == red) {
				node_br->color = black;
				node->parent->color = red;
				rotated = rbtree_rotate_right(node->parent);
				if (rotated->parent == NULL) {
					tree = rotated;
				}
				node_br = node->parent->left;
			}

			if (node_br->left->color == black && node_br->right->color == black) {
				node_br->color = red;
				node = node->parent;
			} else {
				if (node_br->left->color == black) {
					node_br->right->color = black;
					node_br->color = red;
					rotated = rbtree_rotate_left(node_br);
					if (rotated->parent == NULL) {
						tree = rotated;
					}
					node_br = node->parent->left;
				}
				node_br->color = node->parent->color;
				node->parent->color = black;
				node_br->left->color = black;
				rotated = rbtree_rotate_right(node->parent);
				if (rotated->parent == NULL) {
					tree = rotated;
				}
				node = tree;
			}
		}

	}
	node->color = black;
	return tree;
}

rbtree *rbtree_transplant(rbtree *root, rbtree *node, rbtree *new_node) // swap u and v
{
	if (node->parent == NULL) {
		root = new_node;
	} else if (node == node->parent->left) {
		node->parent->left = new_node;
	} else {
		node->parent->right = new_node;
	}
	if (new_node != NULL) {
		new_node->parent = node->parent;
	}
	return root;
}

rbtree *rbtree_lookup(rbtree *root, int key)
{
	rbtree *node = root;
	int h = 0;
	while (node != NULL) {
		if (key == node->key) {
			printf("found on h = %d\n", h);
			return node;
		} else if (key < node->key) {
			node = node->left;
		} else if (key > node->key) {
			node = node->right;
		}
		h++;
	}
	return node;
}

rbtree *rbtree_min(rbtree *node)
{
	while (node->left != leafNode) {
		node = node->left;
	}
	return node;
}

rbtree *rbtree_max(rbtree *node)
{
	while (node->right != leafNode) {
		node = node->right;
	}
	return node;
}

rbtree *rbtree_add_fixup(rbtree *root, rbtree *node)
{
	rbtree *rotated;
	while (node->parent != NULL && node->parent->color == red) {
		if (node->parent == node->parent->parent->left) { // 2 reds in a left
			rbtree *uncle = node->parent->parent->right;
			if (uncle != NULL && uncle->color == red) { // case 1 - uncle is red
				node->parent->color = black;
				uncle->color = black;
				node->parent->parent->color = red;
				node = node->parent->parent;
			} else { // case 2 - uncle is black and reds: left-right
				if (node == node->parent->right) {
					node = node->parent;
					rotated = rbtree_rotate_left(node);
					if (rotated->parent == NULL) {
						root = rotated;
					}
				} // case 3 - uncle black and 2 reds in a left
				node->parent->color = black;
				node->parent->parent->color = red;
				rotated = rbtree_rotate_right(node->parent->parent);
				if (rotated->parent == NULL) {
					root = rotated;
				}
			}
		} else { // similar
			rbtree *uncle = node->parent->parent->left;
			if (uncle != NULL && uncle->color == red) {
				node->parent->color = black;
				node->parent->parent->color = red;
				uncle->color = black;
				node = node->parent->parent;
			} else {
				if (node == node->parent->left) {
					node = node->parent;
					rotated = rbtree_rotate_right(node);
					if (rotated->parent == NULL) {
						root = rotated;
					}
				}
				node->parent->color = black;
				node->parent->parent->color = red;
				rotated = rbtree_rotate_left(node->parent->parent);
				if (rotated->parent == NULL) {
					root = rotated;
				}
			}
		}
	}
	root->color = black;
	return root;
}

rbtree *rbtree_rotate_left(rbtree *node)
{
	rbtree *des = node->right;
	node->right = des->left;
	if (des->left != NULL) {
		des->left->parent = node;
	}
	des->parent = node->parent;
	if (node->parent != NULL) {
		if (node->parent->left == node) {
			node->parent->left = des;
		} else {
			node->parent->right = des;
		}
	} else {
		des->parent = node->parent;
	}

	des->left = node;
	node->parent = des;
	return des;
}

rbtree *rbtree_rotate_right(rbtree *node)
{
	rbtree *des = node->left;
	node->left = des->right;
	if (des->right != NULL) {
		des->right->parent = node;
	}
	des->parent = node->parent;
	if (node->parent != NULL) {
		if (node->parent->right == node) {
			node->parent->right = des;
		} else {
			node->parent->left = des;
		}
	} else {
		des->parent = node->parent;
	}
	des->right = node;
	node->parent = des;
	return des;
}

static int maxH = 0;
void rbtree_print_dfs(struct rbtree *x, int H) {
    if(x == NULL || x == leafNode) {
		if(H > maxH) {
			maxH = H;
			printf("maxH is %d\n", H);
		}
        return;
    }
	H++;
    //printf("%d -—> %d\n", x->key, x->right->key);
    //printf("   L—> %d\n", x->left->key);
    rbtree_print_dfs(x->left, H);
    rbtree_print_dfs(x->right, H);
}