#include <stdio.h>
#include <stdlib.h>

#define RED     0
#define BLACK   1

struct rb_node_s {
    int key, color;
    struct rb_node_s *p;
    struct rb_node_s *left;
    struct rb_node_s *right;
};

/* nil[T] sentinel */
static struct rb_node_s *nil;

static void rb_left_rotate(struct rb_node_s **T, struct rb_node_s *x) {
    struct rb_node_s *y;

    y = x->right;
    x->right = y->left;

    if (y->left != nil)
        y->left->p = x;

    y->p = x->p;

    if (x->p == nil)
        *T = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->left = x;
    x->p = y;
}

static void rb_right_rotate(struct rb_node_s **T, struct rb_node_s *x) {
    struct rb_node_s *y;

    y = x->left;
    x->left = y->right;

    if (y->right != nil)
        y->right->p = x;

    y->p = x->p;

    if (x->p == nil)
        *T = y;
    else if (x == x->p->right)
        x->p->right = y;
    else
        x->p->left = y;

    y->right = x;
    x->p = y;
}

static void rb_insert_fixup(struct rb_node_s **T, struct rb_node_s *z) {
    struct rb_node_s *y;

    while (z->p->color == RED) {
        if (z->p == z->p->p->left) {
            y = z->p->p->right;
            if (y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            } else {
                if (z == z->p->right) {
                    z = z->p;
                    rb_left_rotate(T, z);
                }

                z->p->color = BLACK;
                z->p->p->color = RED;
                rb_right_rotate(T, z->p->p);
            }
        } else {
            y = z->p->p->left;
            if (y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            } else {
                if (z == z->p->left) {
                    z = z->p;
                    rb_right_rotate(T, z);
                }

                z->p->color = BLACK;
                z->p->p->color = RED;
                rb_left_rotate(T, z->p->p);
            }
        }
    }

    (*T)->color = BLACK;
}

static void rb_insert(struct rb_node_s **T, struct rb_node_s *z) {
    struct rb_node_s *x, *y;

    x = *T;
    y = nil;

    while (x != nil) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->p = y;
    if (y == nil)
        *T = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    z->left = z->right = nil;
    z->color = RED;
    rb_insert_fixup(T, z);
}

static void rb_inorder(struct rb_node_s *p) {
    if (p != nil) {
        rb_inorder(p->left);
        printf("  %d", p->key);
        if (p->color == RED)
            printf(" (red)\n");
        else
            printf(" (black)\n");
        rb_inorder(p->right);
    }
}

static void rb_destroy(struct rb_node_s *p) {
    if (p != nil) {
        rb_destroy(p->left);
        rb_destroy(p->right);
        free(p);
    }
}

static struct rb_node_s *rb_new_node(int key) {
    struct rb_node_s *n;
    n = malloc(sizeof(struct rb_node_s));
    n->key = key;
    n->left = NULL;
    n->right = NULL;
    n->p = NULL;
    return n;
}

int main(int argc, char **argv) {
    struct rb_node_s *root;

    nil = malloc(sizeof(struct rb_node_s));
    nil->color = BLACK;
    nil->p = nil->left = nil->right = nil;

    root = nil;

    rb_insert(&root, rb_new_node(1));
    rb_insert(&root, rb_new_node(2));
    rb_insert(&root, rb_new_node(4));
    rb_insert(&root, rb_new_node(5));
    rb_insert(&root, rb_new_node(7));
    rb_insert(&root, rb_new_node(8));
    rb_insert(&root, rb_new_node(11));
    rb_insert(&root, rb_new_node(14));
    rb_insert(&root, rb_new_node(15));
    rb_inorder(root);

    printf("\nroot = %d\n", root->key);

    rb_destroy(root);
    free(nil);

    return 0;
}
