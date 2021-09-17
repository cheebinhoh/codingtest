/* Copyright © 2021 Chee Bin HOH. All rights reserved.
 *
 * ADT binary search tree
 */

#include <stdio.h>
#include <stdlib.h>
#include "btree.h"
#include "llist.h"


void printTreeNodeInOrder(struct TreeNode *root)
{
    if ( NULL == root )
        return;

    printTreeNodeInOrder(root->left);
    printf("%d, ", root->val);
    printTreeNodeInOrder(root->right);
}


void findTreeNodeAndParentRecursive(struct TreeNode   *root,
                                    int                val,
                                    struct TreeNode  **retNode,
                                    struct TreeNode ***retParent)
{
    if ( NULL == root )
        goto notfound;

    if ( val == root->val )
    {
        *retNode = root;
    }
    else if ( val < root->val
              && NULL != root->left )
    {
        if (NULL != retParent)
            *retParent = &(root->left);

        findTreeNodeAndParentRecursive(root->left, val, retNode, retParent);
    }
    else if ( NULL != root->right )
    {
        if (NULL != retParent)
            *retParent = &(root->right);

        findTreeNodeAndParentRecursive(root->right, val, retNode, retParent);
    }

    return;

notfound:
    if ( NULL != retParent )
        *retParent = NULL;

    *retNode = NULL;
}


/* WARNING that caller is responsible to set the dereferenced object if parent
 * is not NULL as upon return if the deleted node is root, then the recursive
 * function will not the value into location at parent as there is no way
 * we can tell what is the address of the root in caller of this method via
 * to levels of method calls.
 */
void findTreeNodeAndParent(struct TreeNode   *root,
                           int                val,
                           struct TreeNode  **node,
                           struct TreeNode ***parent)
{
    findTreeNodeAndParentRecursive(root, val, node, parent);
}


struct TreeNode * findTreeNode(struct TreeNode *root, int val)
{
    struct TreeNode *ret = NULL;


    findTreeNodeAndParent(root, val, &ret, NULL);

    return ret;
}


struct TreeNode * addTreeNode(struct TreeNode *root, int val)
{
    struct TreeNode *node;


    if ( NULL == root )
    {
        node = malloc(sizeof( struct TreeNode )); // I do not care about NULL error as it is just a test

        node->val   = val;
        node->left  = NULL;
        node->right = NULL;
    }
    else
    {
        node = root;


        if ( val == node->val )
        {
            // do nothing
        }
        else if ( val < node->val )
        {
            node->left = addTreeNode(node->left, val);
        }
        else
        {
            node->right = addTreeNode(node->right, val);
        }
    }

    return node;
}


struct TreeNode * delTreeNode(struct TreeNode *root, int val)
{
    struct TreeNode **parent;
    struct TreeNode  *node;
    struct TreeNode  *tmp;
    int               leftLevel;
    int               rightLevel;


    if ( NULL == root )
        return NULL;

    parent = &root;
    findTreeNodeAndParent(root, val, &node, &parent);
    if ( NULL != node )
    {
        leftLevel  = determineMaxDepthLevel(node->left);
        rightLevel = determineMaxDepthLevel(node->right);

        if ( leftLevel >= rightLevel )
        {
            tmp = node->left;
            while ( NULL != tmp
                    && NULL != tmp->right )
                tmp = tmp->right;

            if ( NULL != tmp )
                tmp->right = node->right;

            *parent = node->left;
        }
        else
        {
            tmp = node->right;
            while ( NULL != tmp
                    && NULL != tmp->left )
                tmp = tmp->left;

            if ( NULL != tmp )
                tmp->left = node->left;

            *parent = node->right;
        }

        free(node);
    }

    return root;
}


int determineMaxDepthLevelRecursive(struct TreeNode *root,
                                    int              level)
{
    int rightLevel;
    int leftLevel;


    if ( NULL == root )
        return level;

    rightLevel = determineMaxDepthLevelRecursive(root->left, level + 1);
    leftLevel = determineMaxDepthLevelRecursive(root->right, level + 1);

    return ( rightLevel > leftLevel ) ? rightLevel : leftLevel;
}


int determineMaxDepthLevel(struct TreeNode *root)
{
    return determineMaxDepthLevelRecursive(root, 0);
}

int determineMinDepthLevelRecursive(struct TreeNode *root,
                                    int              level,
                                    int             *retMinLevel)
{
    int leftLevel;
    int rightLevel;
    int oldMinLevel;


    if ( NULL == root )
    {
        if ( level < *retMinLevel )
            *retMinLevel = level;

        return level;
    }

    level       = level + 1;
    oldMinLevel = *retMinLevel;
    leftLevel   = determineMinDepthLevelRecursive(root->left, level, retMinLevel);

    if ( leftLevel == level
         && NULL != root->right )
        *retMinLevel = oldMinLevel;

    oldMinLevel = *retMinLevel;
    rightLevel  = determineMinDepthLevelRecursive(root->right, level, retMinLevel);

    if ( rightLevel == level
         && NULL != root->left )
        *retMinLevel = oldMinLevel;

    return ( leftLevel < rightLevel ) ? rightLevel : leftLevel;
}

int determineMinDepthLevel(struct TreeNode *root)
{
    int minLevel;
    int i;


    i        = 1;
    minLevel = i;
    while ( i > 0 )
    {
        minLevel = i;
        i = i << 1;
        i = i + 1;
    }

    determineMinDepthLevelRecursive(root, 0, &minLevel);

    return minLevel;
}

void printTreeNodeInTreeTopologyRecursive(struct TreeNode *root,
                                          int              level)
{
    int i;


    for ( i = 0; i < level * 5; i++ )
        putchar(' ');

    if ( NULL == root)
    {
        printf("-\n");
        return;
    }

    printf("%d (L=%d, R=%d)\n",
           root->val,
           determineMaxDepthLevel(root->left),
           determineMaxDepthLevel(root->right));

    printTreeNodeInTreeTopologyRecursive(root->right, level + 1);
    printTreeNodeInTreeTopologyRecursive(root->left, level + 1);
}


void printTreeNodeInTreeTopology(struct TreeNode *root)
{
    printTreeNodeInTreeTopologyRecursive(root, 0);
}


void traverseTreeNodeInOrderRecursive(struct TreeNode        *root,
                                      int                    *pos,
                                      bTreeTraversalCallback  func,
                                      void                   *data)
{
    if ( NULL == root )
        return;

    traverseTreeNodeInOrderRecursive(root->left, pos, func, data);

    func(root, *pos, data);
    *pos = *pos + 1;

    traverseTreeNodeInOrderRecursive(root->right, pos, func, data);
}


void traverseTreeNodeInOrder(struct TreeNode *root, bTreeTraversalCallback func, void *data)
{
    int pos;


    pos = 0;
    traverseTreeNodeInOrderRecursive(root, &pos, func, data);
}


void countTreeNode(struct TreeNode *node, int pos, void *data)
{
    int *count;


    count  = data;
    *count = *count + 1;
}


int findTotalNumberOfTreeNode(struct TreeNode *root)
{
    int count = 0;


    traverseTreeNodeInOrder(root, countTreeNode, &count);

    return count;
}


void freeTreeNode(struct TreeNode *root)
{
    if ( NULL == root )
        return;

    if ( NULL != root->left )
        freeTreeNode(root->left);

    if ( NULL != root->right )
        freeTreeNode(root->right);

    free(root);
}

int isSmallerThanList(int val, struct ListNode *list)
{
    if ( NULL == list )
       return 1;

    do {
        if ( val > list->val )
            return 0;

        list = list->next;
    } while ( NULL != list );

    return 1;
}

int isLargerThanList(int val, struct ListNode *list)
{
    if ( NULL == list )
       return 1;

    do {
        if ( val < list->val )
            return 0;

        list = list->next;
    } while ( NULL != list );

    return 1;
}

int isTreeBinarySearchTreeRecursive(struct TreeNode *root,
                                    struct ListNode *smaller,
                                    struct ListNode *larger)
{
    struct ListNode *tmp;


    if ( NULL == root )
        return 1;

    if ( ! isSmallerThanList(root->val, smaller) )
        return 0;

    if ( ! isLargerThanList(root->val, larger) )
        return 0;

    pushStack(root->val, &smaller);
    if ( ! isTreeBinarySearchTreeRecursive(root->left, smaller, larger) )
        return 0;

    tmp = popStack(&smaller);
    free(tmp);

    pushStack(root->val, &larger);
    if ( ! isTreeBinarySearchTreeRecursive(root->right, smaller, larger) )
        return 0;

    tmp = popStack(&larger);
    free(tmp);

    return 1;
}

int isTreeBinarySearchTree(struct TreeNode *root)
{
    struct ListNode *smaller = NULL;
    struct ListNode *larger  = NULL;
    int              ret;


    ret = isTreeBinarySearchTreeRecursive(root, smaller, larger);

    delStack(smaller);
    delStack(larger);

    return ret;
}
