#include <stdlib.h>
#include "avl.h"

//Balance functions

unsigned char height(AVL *p)
{
    return p? p->height : 0;
}

int balance_factor(AVL *p)
{
    return height(p->right) - height(p->left);
}

void reset_height(AVL *p)
{
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl > hr? hl : hr) + 1;
}

AVL *rotate_right(AVL *p)
{
    AVL *q = p->left;
    p->left = q->right;
    q->right = p;
    reset_height(p);
    reset_height(q);
    return q;
}

AVL *rotate_left(AVL *q)
{
    AVL *p = q->right;
    q->right = p->left;
    p->left = q;
    reset_height(q);
    reset_height(p);
    return p;
}

AVL *balance(AVL *p)
{
    reset_height(p);
    if (balance_factor(p) == 2)
    {
        if (balance_factor(p->right) < 0)
            p->right = rotate_right(p->right);
        return rotate_left(p);
    }
    if (balance_factor(p) == -2)
    {
        if (balance_factor(p->left) > 0)
            p->left = rotate_left(p->left);
        return rotate_right(p);
    }
    return p;
}

AVL *avl_add(AVL *root, int num)
{
    if (root)
    {
        if (num > root->value)
            root->right = avl_add(root->right, num);
        else if (num < root->value)
            root->left = avl_add(root->left, num);
        return balance(root);
    }
    else
    {
        //printf("avl_add: %u\n", (unsigned)offsetof(AVL, right));
        AVL *newel = malloc(sizeof(AVL));
        newel->value = num;
        newel->left = newel->right = NULL;
        newel->height = 1;
        return newel;
    }
}

void avl_free(AVL *root)
{
    if (root)
    {
        avl_free(root->right);
        avl_free(root->left);
        free(root);
    }
}

AVL* findmin(AVL* p) // поиск узла с минимальным ключом в дереве p
{
    return p->left?findmin(p->left):p;
}

AVL* removemin(AVL* p) // удаление узла с минимальным ключом из дерева p
{
    if( p->left==0 )
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}

AVL* del(AVL* p, int k) // удаление ключа k из дерева p
{
    if( !p ) return 0;
    if( k < p->value )
        p->left = del(p->left, k);
    else if (k > p->value)
        p->right = del(p->right,k);
    else //  k == p->key
    {
        AVL* q = p->left;
        AVL* r = p->right;
        free(p);
        if( !r ) return q;
        AVL* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}
