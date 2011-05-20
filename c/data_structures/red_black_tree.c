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
        /* is z's grandparent's left child z's parent? */
        if (z->p == z->p->p->left) {
            /* z's right uncle */
            y = z->p->p->right;

            if (y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            } else {
                if (z == z->p->right) {
                    /* z is its parent's right child */
                    z = z->p;
                    rb_left_rotate(T, z);
                }

                z->p->color = BLACK;
                z->p->p->color = RED;
                rb_right_rotate(T, z->p->p);
            }
        } else {
            /* z's left uncle */
            y = z->p->p->left;
            if (y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            } else {
                if (z == z->p->left) {
                    /* z is its parent's left child */
                    z = z->p;
                    rb_right_rotate(T, z);
                }

                z->p->color = BLACK;
                z->p->p->color = RED;
                rb_left_rotate(T, z->p->p);
            }
        }
    }

    /* the root is always black */
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

static void print_node(struct rb_node_s *p) {
    printf("  %d", p->key);
    if (p->color == RED)
        printf(" (red)\n");
    else
        printf(" (black)\n");
}

static void rb_preorder(struct rb_node_s *p) {
    if (p != nil) {
        print_node(p);
        rb_preorder(p->left);
        rb_preorder(p->right);
    }
}

static void rb_inorder(struct rb_node_s *p) {
    if (p != nil) {
        rb_inorder(p->left);
        print_node(p);
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

static rb_node_s *rb_min(struct rb_node_s *x) {
    while (x->left)
        x = x->left;
    return x;
}

static struct rb_node_s *rb_successor(struct rb_node_s *x) {
    /*
     * The successor of node x is the node with the smallest key larger than
     * x->key. The BST property lets us determine a successor without key comparisons.
     */
    struct rb_node_s *t;

    if (x->right) {
        /* all values in this branch are larger than x, so find the min of it */
        return rb_min(x->right);
    }

    t = x->p;
    while (t && x == t->right) {
        x = t;
        t = t->p;
    }

    return t;
}

static rb_node_s *rb_delete(struct rb_node_s **T, struct rb_node_s *z) {
    struct rb_node_s *x, *y;

    if (z->left == nil || z->right == nil)
        y = z;
    else
        y = rb_successor(z);

    if (y->left != nil)
        x = y->left;
    else
        x = y->right;

    x->p = y->p;

    if (y->p == nil)
        *T = x;
    else if (y == y->parent->left)
        y->p->left = x;
    else
        y->p->right = x;

    if (y != z) {
        z->key = y->key;
        z->color = y->color;
    }

    if (y->color == BLACK)
        rb_delete_fixup(T, x);

    return y;
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

    rb_preorder(root);

    printf("\nroot = %d\n", root->key);

    rb_destroy(root);
    free(nil);

    return 0;
}
