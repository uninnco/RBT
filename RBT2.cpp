#include<iostream>
using namespace std;

// 定义节点颜色  
enum COLOR
{  
    BLACK = 0,  
    RED  
};  
  
// 红黑树节点  
typedef struct RB_Tree_Node
{  
    int key;  
    struct RB_Tree_Node *left;  
    struct RB_Tree_Node *right;  
    struct RB_Tree_Node *parent;  
    unsigned char RB_COLOR;  
}RB_Node;

// 红黑树，包含一个指向根节点的指针  
typedef struct RBTree{  
    RB_Node* root;
}*RB_Tree;

// 红黑树的NIL节点  
static RB_Tree_Node NIL = {0, 0, 0, 0, BLACK}; 

#define PNIL (&NIL)   // NIL节点地址 

void Init_RBTree(RB_Tree pTree) // 初始化一棵红黑树  
{  
    pTree->root = PNIL;  
}   

// 查找最小键值节点  
RB_Node* RBTREE_MIN(RB_Node* pRoot)  
{  
    while (PNIL != pRoot->left)
    {
        pRoot = pRoot->left;
    }  
    return pRoot;
}


/*
                          15
                        /    \
                       /      \
                      /        \
                     6          18
                    /  \       /  \
                   /    \     /    \
                  3      7   17    20
                 /  \     \
                /    \     \
               2      4     13
                            /
                           /
                          9
*/
// 查找指定节点的后继节点  
RB_Node* RBTREE_SUCCESSOR(RB_Node*  pRoot)  
{  
    if (PNIL != pRoot->right)    // 查找图中6的后继节点时就调用RBTREE_MIN函数
    {  
        return RBTREE_MIN(pRoot->right);  
    }
    // 节点没有右子树的时候，进入下面的while循环（如查找图中13的后继节点时，它的后继节点是15）
    RB_Node* pParent = pRoot->parent;  
    while((PNIL != pParent) && (pRoot == pParent->right))
    {  
        pRoot = pParent;
        pParent = pRoot->parent;      
    }
    return pParent;
}

// 红黑树的节点删除
RB_Node* Delete(RB_Tree pTree , RB_Node* pDel)  
{  
    RB_Node* rel_delete_point;
    if(pDel->left == PNIL || pDel->right == PNIL)
        rel_delete_point = pDel;
    else
        rel_delete_point = RBTREE_SUCCESSOR(pDel);     // 查找后继节点

    RB_Node* delete_point_child;  
    if(rel_delete_point->right != PNIL)  
    {  
        delete_point_child = rel_delete_point->right;  
    }  
    else if(rel_delete_point->left != PNIL)  
    {  
        delete_point_child = rel_delete_point->left;  
    }  
    else  
    {  
        delete_point_child = PNIL;  
    }  
    delete_point_child->parent = rel_delete_point->parent;  
    if(rel_delete_point->parent == PNIL)    // 删除的节点是根节点
    {  
        pTree->root = delete_point_child;
    }  
    else if(rel_delete_point == rel_delete_point->parent->right)
    {  
        rel_delete_point->parent->right = delete_point_child;  
    }  
    else  
    {  
        rel_delete_point->parent->left = delete_point_child;  
    }
    if(pDel != rel_delete_point)
    {
        pDel->key = rel_delete_point->key;
    }
    if(rel_delete_point->RB_COLOR == BLACK)  
    {  
        DeleteFixUp(pTree , delete_point_child);  
    }
    return rel_delete_point;  
}  
             

/*
算法导论上的描述如下：
RB-DELETE-FIXUP(T, x) 
1 while x ≠ root[T] and color[x] = BLACK 
2     do if x = left[p[x]] 
3           then w ← right[p[x]] 
4                if color[w] = RED 
5                   then color[w] ← BLACK                           Case 1 
6                        color[p[x]] ← RED                          Case 1 
7                        LEFT-ROTATE(T, p[x])                       Case 1 
8                        w ← right[p[x]]                            Case 1 
9                if color[left[w]] = BLACK and color[right[w]] = BLACK 
10                   then color[w] ← RED                            Case 2 
11                        x p[x]                                    Case 2 
12                   else if color[right[w]] = BLACK 
13                           then color[left[w]] ← BLACK            Case 3 
14                                color[w] ← RED                    Case 3 
15                                RIGHT-ROTATE(T, w)                Case 3 
16                                w ← right[p[x]]                   Case 3 
17                         color[w] ← color[p[x]]                   Case 4 
18                         color[p[x]] ← BLACK                      Case 4 
19                         color[right[w]] ← BLACK                  Case 4 
20                         LEFT-ROTATE(T, p[x])                     Case 4 
21                         x ← root[T]                              Case 4 
22        else (same as then clause with "right" and "left" exchanged) 
23 color[x] ← BLACK  
*/  
//接下来的工作，很简单，即把上述伪代码改写成c++代码即可  
void DeleteFixUp(RB_Tree pTree , RB_Node* node)  
{  
    while(node != pTree->root && node->RB_COLOR == BLACK)  
    {  
        if(node == node->parent->left)  
        {  
            RB_Node* brother = node->parent->right;  
            if(brother->RB_COLOR==RED)   //情况1：x的兄弟w是红色的。  
            {  
                brother->RB_COLOR = BLACK;  
                node->parent->RB_COLOR = RED;  
                RotateLeft(node->parent);  
            }  
            else     //情况2：x的兄弟w是黑色的，  
            {  
                if(brother->left->RB_COLOR == BLACK && brother->right->RB_COLOR == BLACK)  //w的两个孩子都是黑色的  
                {  
                    brother->RB_COLOR = RED;  
                    node = node->parent;  
                }  
                else
                {
                    if(brother->right->RB_COLOR == BLACK)   //情况3：x的兄弟w是黑色的，w的右孩子是黑色（w的左孩子是红色）。  
                    {
                        brother->RB_COLOR = RED;
                        brother->left->RB_COLOR = BLACK;
                        RotateRight(brother);
                        brother = node->parent->right;      //情况3转换为情况4
                    }
                    //情况4：x的兄弟w是黑色的，且w的右孩子时红色的
                    brother->RB_COLOR = node->parent->RB_COLOR;  
                    node->parent->RB_COLOR = BLACK;  
                    brother->right->RB_COLOR = BLACK;  
                    RotateLeft(node->parent);  
                    node = pTree->root;
                }//else
            }//else
        }  
        else   //同上，原理一致，只是遇到左旋改为右旋，遇到右旋改为左旋即可。其它代码不变。  
        {  
            RB_Node* brother = node->parent->left;  
            if(brother->RB_COLOR == RED)  
            {  
                brother->RB_COLOR = BLACK;  
                node->parent->RB_COLOR = RED;  
                RotateRight(node->parent);  
            }  
            else  
            {  
                if(brother->left->RB_COLOR==BLACK && brother->right->RB_COLOR == BLACK)  
                {  
                    brother->RB_COLOR = RED;  
                    node = node->parent;  
                }  
                else
                {
                    if(brother->left->RB_COLOR==BLACK)  
                    {  
                        brother->RB_COLOR = RED;  
                        brother->right->RB_COLOR = BLACK;  
                        RotateLeft(brother);
                        brother = node->parent->left;      //情况3转换为情况4
                    }
                    brother->RB_COLOR = node->parent->RB_COLOR;  
                    node->parent->RB_COLOR = BLACK;  
                    brother->left->RB_COLOR = BLACK;  
                    RotateRight(node->parent);  
                    node = pTree->root;  
                }  
            }  
        }  
    }//while 
    node->RB_COLOR = BLACK;    //如果X节点原来为红色，那么直接改为黑色  
}
