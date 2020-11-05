#include "tldlist.h"
#include "date.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct tldnode{
    char *host;
    struct tldnode *left;
    struct tldnode *right;
    struct tldnode *parent;
    long count;
    int height;
    int balance;
};

struct tldlist{
    TLDNode *root;
    Date *begin;
    Date *end;
    int count;
};

struct tlditerator{
    TLDNode *curr_node;
    TLDNode *next;
    TLDNode *root;


};

// Start of helper functions:
int height(TLDNode *n){
	if (n == NULL){
		return -1;
	}
	return n->height;
}

void reheight(TLDNode *node){
    if (node != NULL){
        int curr_max_height = 0;
        if (height(node->left) > height(node->right)){
            curr_max_height = height(node->left);
        } else {
            curr_max_height = height(node->right);
        }
        node->height = 1 + curr_max_height;
    }
}

void setBalance(TLDNode *n){
    reheight(n);
    n->balance = height(n->right) - height(n->left);
}

TLDNode *rotateLeft(TLDNode *a){
    TLDNode *b = malloc(sizeof(TLDNode));
    b = a->right;
    b -> parent = a->parent;

    a->right = b->left;
    if (a->right != NULL){
        a->right->parent = a;
    }
    b->left = a;
    a->parent = b;

    if (b->parent != NULL){
        if (b->parent->right == a){
            b->parent->right = b;
        } else {
            b->parent->left = b;
        }
    }

    setBalance(a);
    setBalance(b);

    return b;
}

TLDNode *rotateRight(TLDNode *a){
    TLDNode *b = malloc(sizeof(TLDNode));
    b = a->left;
    b->parent = a->parent;

    a->left = b->right;

    if (a->left != NULL){
        a->left->parent = a;
    }

    b->right = a;
    a->parent = b;

    if (b->parent != NULL){
        if (b->parent->right == a){
            b->parent->right = b;
        } else {
            b->parent->left = b;
        }
    }

    setBalance(a);
    setBalance(b);

    return b;
}

TLDNode *rotateLeftThenRight(TLDNode *n) {
    n->left = rotateLeft(n->left);
    return rotateRight(n);
}

TLDNode *rotateRightThenLeft(TLDNode *n){
    n->right = rotateRight;
    return rotateLeft(n);
}

void rebalance(TLDNode *n, TLDList *tld){
    setBalance(n);
	
	if (n->balance == -2) {
        if (height(n->left->left) >= height(n->left->right)){
            n = rotateRight(n);
        } else{
            n = rotateLeftThenRight(n);
		}
    } else if (n->balance == 2) {
        if (height(n->right->right) >= height(n->right->left)){
            n = rotateLeft(n);
        } else{
            n = rotateRightThenLeft(n);
		}
    }
 
    if (n->parent != NULL) {
        rebalance(n->parent, tld);
    } else {
        tld->root = n;
    }
}
// End of helper functions

TLDList *tldlist_create(Date *begin, Date *end){

    if (date_compare(begin,end) < 0){
        TLDNode *root = NULL;
        TLDList *new_list = malloc(sizeof(TLDList));
		new_list -> begin = begin;
        new_list -> end = end;
        new_list -> root = root;
        new_list -> count = 0;
        if (new_list == NULL){
            return NULL;
        } else {
            //new_list -> begin = begin;
            //new_list -> end = end;
            //new_list -> root = root;
            //new_list -> count = 0;
            return new_list;
        }
    } else {
        return NULL;
    }
}

void tldlist_destroy(TLDList *tld){
    free(tld);
}

int tldlist_add(TLDList *tld, char *hostname, Date *d){
    char *host;
    if (date_compare(tld->begin, d) < 0 && date_compare(tld->end, d) > 0){
        char *host_orig = strrchr(hostname, ".");
        if (host[0] == "."){
            host = tolower(host_orig + 1);
        } else {
            host = tolower(host_orig);
        }

        if (tld->root == NULL) {
            TLDNode *root_orig = malloc(sizeof(TLDNode));
            root_orig -> host = host;
            root_orig -> left = NULL;
            root_orig -> right = NULL;
            tld -> root = root_orig;
            tld -> count += 1;
        }

        TLDNode *node = malloc(sizeof(TLDNode));
        node = tld->root;
        while (true){
            if (strcmp(node->host,host) == 0) { return 0;}

            TLDNode *parent_node = malloc(sizeof(TLDNode));
            parent_node = node;

            bool goLeft = strcmp(node->host, host);
            node = goLeft ? node->left : node->right;

            if (node == NULL){
                if (goLeft) {
                    TLDNode *curr_node_left = malloc(sizeof(TLDNode));
                    curr_node_left -> host = host;
                    curr_node_left -> parent = parent_node;
                    parent_node -> left = curr_node_left;
                } else {
                    TLDNode *curr_node_right = malloc(sizeof(TLDNode));
                    curr_node_right -> host = host;
                    curr_node_right -> parent = parent_node;
                    parent_node -> right = curr_node_right;
                }
                tld -> count += 1;
                rebalance(parent_node, tld);
                break;
            }
        }
        return 1;
    } else {
        return 0;
    }
}

long tldlist_count(TLDList *tld){
    return tld -> count;
}

TLDIterator *tldlist_iter_create(TLDList *tld){
    TLDIterator *iter = malloc(sizeof(TLDIterator));
    iter->root = tld->root;
    iter->curr_node = tld->root;

    if(iter != NULL){
        return iter;
    } else{
        return NULL;
    }
}

TLDNode *tldlist_iter_next(TLDIterator *iter){
    if (iter->curr_node->left != NULL){
        return iter->curr_node->left;
    } else if (iter->curr_node->right != NULL){
        return iter->curr_node->right;
    } else {
        return NULL;
    }
}

void tldlist_iter_destroy(TLDIterator *iter){
    free(iter);
}

char *tldnode_tldname(TLDNode *node){
    return node->host;
}

// helper method for tldnode_count
void tld_traverse(TLDNode *node, char *desired_host,long count){
    if(node == NULL){
        return;
    }
    if (strcmp(node->host,desired_host) == 0){
        count += 1;
        node->count = count;
    }

    tld_traverse(node->left, desired_host,count);
    tld_traverse(node->right, desired_host, count);

}
// end of helper method
long tldnode_count(TLDNode *node){
	long count = 0;
    node->count = count;
    char *desired_host = node->host;
    tld_traverse(node, desired_host, count);
    node->count = count;
    return node->count;
}

