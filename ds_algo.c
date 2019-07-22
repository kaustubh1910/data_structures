/**
 * @file     ds_alog.c 
 * @author  
 * @date     03-13-2019
 * @version 1.0
 *
 * @property    $ Copyright: (c) 2019 Broadcom Limited All Rights Reserved $
 *       No portions of this material may be reproduced in any form without the
 *       written permission of: 
 *               Broadcom Limited
 *               1320 Ridder Park Drive
 *               San Jose, California 95131
 *               United States
 * All information contained in this document/file is Broadcom Limit company
 * private proprietary, trade secret, and remains the property of Broadcom
 * Limited. The intellectual and technical concepts contained herein are
 * proprietary to Broadcom Limited and may be covered by U.S. and Foreign
 * Patents, patents in process, and are protected by trade secret or copyright
 * law. Dissemination of this information or reproduction of this material is
 * strictly forbidden unless prior written permission is obtained from Bloadcom
 * Limited.
 *
 * @brief   
 *
 * @section
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_ROW 3
#define MAX_COLUMN  10

typedef enum
{
    FALSE = 0,
    TRUE = 1
}BOOL;

typedef enum
{
    NOT_FOUND  = 0,
    IN_LEFT = 1,
    IN_RIGHT = 2
}FOUND;

#if 0
int graph[5][5] = { 0, 3, -1, 12, 2,
    3, 0, -1, 5, -1, 
    -1, -1, 0, 1, -1, 
    12, 5, 1, 0, 22,
    2, -1, -1, 22, 0};
#endif

typedef enum
{
    IN_ORDER,
    PRE_ORDER,
    POST_ORDER
        //BREADTH_FIRST
} TRAVERSE_TYPE;

typedef struct node{
    int data;
    void * ptr;
    struct node *next;
} NODE;

typedef struct queue_of_pointers{
    NODE * first;
    NODE * last;
} QUEUE;

typedef struct stack_of_pointers{
    NODE * bottom;
    NODE * top;
}STACK;

typedef struct tree_node{
    int data;
    char is_visited;

    struct tree_node * left;
    struct tree_node * right;
}TREE_NODE;

NODE * create_node(int val)
{
    NODE * new_node  = malloc(sizeof(NODE));
    if (new_node != NULL)
    {
        new_node->data = val;
        new_node->ptr = NULL;
        new_node->next = NULL;
        return new_node;
    }
    else
        return NULL;
}

QUEUE* new_queue ()
{
    QUEUE * element = malloc(sizeof(QUEUE));

    if (NULL == element)
        return NULL;

    element->first = NULL;
    element->last = NULL;

    return element;
}

STACK * new_stack()
{
    STACK * this = malloc(sizeof(STACK));

    if (NULL == this)
        return NULL;

    this->top = NULL;
    this->bottom = NULL;

    return this;
}

int enqueue(QUEUE* my_queue, void* tnode)
{
    if (NULL == my_queue || NULL == tnode)
        return -1;

    if (my_queue->first == NULL)
    {
        my_queue->first = create_node(0);
        my_queue->first->ptr = (void *) tnode;
        my_queue->last = my_queue->first;
    }
    else
    {
        NODE* new_node = create_node(0);
        if (NULL != new_node)
        {
            my_queue->last->next = new_node;
            my_queue->last = new_node;
            my_queue->last->ptr = (void *) tnode;
        }
    }

    return 0;
}

void* dequeue(QUEUE* my_queue)
{
    NODE * tmp;
    void* node_ptr = NULL;
    if (NULL == my_queue)
        return NULL;

    if (NULL == my_queue->first)
    {
        //printf("Empty queue.\n");
        return NULL;
    }
    else
    {
        tmp = my_queue->first;
        node_ptr = tmp->ptr;
        my_queue->first = tmp->next;

        free(tmp);
        return node_ptr;
    }
}

void push(STACK * stk, void * element)
{
    if ( (NULL == stk) || (NULL == element) )
        return;

    if (NULL == stk->bottom)
    {
        //First element to be pushed in the stack:
        stk->bottom = create_node(0);
        if (NULL != stk->bottom)
        {
            stk->bottom->ptr = element;
            stk->bottom->next = NULL;
            stk->top = stk->bottom;
        }
    }
    else
    {
        NODE * tmp = create_node(0);
        if (NULL != tmp)
        {
            tmp->next = stk->top;
            stk->top = tmp;
            stk->top->ptr = element;
        }
    }
}

void * pop(STACK * stk)
{
    void * tmp = NULL;
    if (NULL == stk)   
        return NULL;

    if (NULL == stk->bottom)    //Empty stack
        return NULL;

    if (stk->top == stk->bottom)    //Only one element in stack
    {
        tmp = stk->top->ptr;
        free(stk->top);
        stk->top = stk->bottom = NULL;
    }
    else
    {
        tmp = stk->top->ptr;
        free(stk->top);
        stk->top = stk->top->next;
    }
    return tmp;
}


//###########################       LINKED LIST       ########################
NODE * create_linked_list(int a[10])
{
    NODE * head = create_node(a[0]);
    NODE * ref_node;
    int i;

    if (NULL != head)
    {
        ref_node = head;
        for (i = 1; i < 10; i++)
        {
            ref_node->next = create_node(a[i]);
            if (NULL != ref_node->next)
            {
                ref_node = ref_node->next;
            }
        }
    }

    return head;
}

void reverse_linked_list(NODE **head)
{
    NODE *tmp, *prev_node = NULL;
    if (NULL == head)
        return;

    if (*head == NULL )
        return;

    while(1)
    {
        tmp = (*head)->next;
        (*head)->next = prev_node;
        prev_node = *head;
        if (tmp != NULL)
        {
            *head = tmp;
        }
        else
        {
            break;
        }
    }
    return;
}

void reverse_ll_recursive(NODE** head)
{
    static NODE * prev_node = NULL;
    NODE * tmp;
    if (head == NULL)
        return;

    if (*head == NULL)
        return;

    tmp = (*head)->next;
    (*head)->next = prev_node;
    prev_node = *head;
    if(tmp)
    {
        (*head) = tmp;
        reverse_ll_recursive(head);
    }
    return;
}

void print_linked_list(NODE *head)
{
    NODE * tmp = head;
    if (NULL == head)
        return;

    do
    {
        printf("%d, ", tmp->data);
        tmp = tmp->next;
    }while(tmp);

    return;
}

//#######################        TREE FUNCTIONS        #########################

TREE_NODE * create_tree_node(int val)
{
    TREE_NODE * NODE = malloc(sizeof(TREE_NODE));
    if (NULL == NODE)
        return NULL;

    NODE->data = val;
    NODE->is_visited = 0;

    NODE->left = NULL;
    NODE->right = NULL;

    return NODE;
}

void add_node_to_tree(TREE_NODE * root, TREE_NODE * new_node )
{
    if ( (root == NULL) || (new_node == NULL))
        return;

    if (new_node->data > root->data)
    {
        if(root->right == NULL)
            root->right = new_node;
        else
            add_node_to_tree(root->right, new_node);
    }
    else if(new_node->data < root->data)
    {
        if(root->left == NULL)
            root->left = new_node;
        else
            add_node_to_tree(root->left, new_node);
    }

    return;
}


TREE_NODE * construct_tree(int a[10])
{
    TREE_NODE * root = NULL;
    TREE_NODE * new_node = NULL;
    int i;

    for (i = 0; i < 10; i++)
    {
        new_node = create_tree_node(a[i]);
        if (NULL == root)
            root = new_node;
        else
        {
            add_node_to_tree(root, new_node);
            printf("Added node %d...\n", new_node->data);
        }
    }

    return root;
}


void traverse_tree(TREE_NODE * root, TRAVERSE_TYPE tt, char is_delete)
{
    switch(tt)
    {
        case IN_ORDER:
            {
                if (root == NULL )
                {
                    //printf("NULL Tree...\n");
                    return;
                }

                traverse_tree(root->left, tt, 0);
                printf("%d, ", root->data);
                traverse_tree(root->right, tt, 0);

                break;
            }

        case PRE_ORDER:
            {
                if (root == NULL )
                {
                    //printf("NULL Tree...\n");
                    return;
                }

                printf("%d, ", root->data);
                traverse_tree(root->left, tt, 0);
                traverse_tree(root->right, tt, 0);
                break;
            }

        case POST_ORDER:
            {
                if (root == NULL )
                {
                    //printf("NULL Tree...\n");
                    return;
                }

                traverse_tree(root->left, tt, is_delete);
                traverse_tree(root->right, tt, is_delete);

                printf("%d, ", root->data);

                if(is_delete)
                    free(root);

                break;
            }
    }
    return;
}

void delete_tree(TREE_NODE * root)
{
    if (NULL == root)
        return;

    traverse_tree(root, POST_ORDER, 1);
}

void traverse_breadth_first(TREE_NODE* root)
{
    TREE_NODE* current_node = root;
    if (root == NULL)
        return;

    QUEUE *bfs_queue = new_queue();

    enqueue(bfs_queue, root);

    while(1)
    {
        current_node = (TREE_NODE *) dequeue(bfs_queue);
        if(NULL == current_node)
            break;

        printf("%d, ", current_node->data);

        enqueue(bfs_queue, (void *) current_node->left);
        enqueue(bfs_queue, (void *) current_node->right);
    }
}

int find_tree_height(TREE_NODE* root)
{
    int left_subtree_height = 0, right_subtree_height = 0;
    if (NULL == root)
        return 0;

    if ((NULL == root->left) && (NULL == root->right))
    {
        return 1;
    }    

    if(root->left)
    {
        left_subtree_height = find_tree_height(root->left);
    }
    if(root->right)
    {
        right_subtree_height = find_tree_height(root->right);
    }

    return((left_subtree_height>right_subtree_height)?(++left_subtree_height):(++right_subtree_height));

}

void fetch_tree_nodes_at_level(TREE_NODE* root, int height, QUEUE* level_queue)
{
    if ((NULL == level_queue) || (NULL == root) || (height < 1))
        return;

    if ( height == 1 ) 
    {
        enqueue(level_queue, (void*)root);
        return;
    }

    if(root->left != NULL)
        fetch_tree_nodes_at_level(root->left, (height - 1), level_queue);

    if(root->right != NULL)
        fetch_tree_nodes_at_level(root->right, (height- 1), level_queue);

    return;
}

void print_peer_nodes(TREE_NODE* root)
{
    QUEUE* tree_queue;
    TREE_NODE* node_tmp;
    int height, level;

    if (NULL == root)
    {
        return;
    }

    tree_queue = new_queue();
    height = find_tree_height(root);
    printf("Height of tree is %d\n", height);

    for(level = 1; level <= height; level++)
    {
        fetch_tree_nodes_at_level(root, level, tree_queue);

        while(1)
        {
            node_tmp = (TREE_NODE*)dequeue(tree_queue);
            if(NULL == node_tmp)
            {
                break;
            }
            printf("%d ", node_tmp->data);
        }
        printf("\n");
    }

    free(tree_queue);
}

BOOL search_node_in_tree(TREE_NODE *root, int val)
{
    BOOL found = FALSE;

    if (root == NULL)
        return FALSE;
    printf("Searching %d in node of %d\n", val, root->data);

    if (root->data == val)
        return TRUE;

    if (root->left)
    {
        found = search_node_in_tree(root->left, val);
        if (found)
            return found;
    }

    if (root->right)
    {
        found = search_node_in_tree(root->right, val);
    }

    return found;
}

BOOL search_and_trace(TREE_NODE * root, STACK * trace, int val)
{
    BOOL retval = FALSE;
    if ((NULL == root) || (NULL == trace))
        return FALSE;

    if(root->data == val)
    {
        push(trace, root);
        //printf("\nPushed %d", root->data);
        retval = TRUE;
        return retval;
    }
    else
    {
        if( TRUE == search_and_trace(root->left, trace, val ) )
        {
            push(trace, root);
            //printf("\nPushed %d", root->data);
            retval = TRUE;
            return retval;
        }
        else if(search_and_trace(root->right, trace, val))
        {
            push(trace, root);
            //printf("\nPushed %d", root->data);
            retval = TRUE;
            return retval;
        }

        return retval;
    }
}

TREE_NODE* find_common_ancestor(TREE_NODE* root, int val1, int val2)
{
    TREE_NODE* common_ancestor = NULL;

    STACK* trace_val1 = new_stack();
    STACK* trace_val2 = new_stack();

    if (NULL == root)
        return NULL;

    search_and_trace(root, trace_val1, val1);
    search_and_trace(root, trace_val2, val2);

    while(1)
    {
        TREE_NODE* tmp1 = pop(trace_val1);
        TREE_NODE* tmp2 = pop(trace_val2);
        if((NULL == tmp1) || (NULL == tmp2))
            break;

        //printf("\nPopped %d and %d\n", tmp1->data, tmp2->data);
        if(tmp1 == tmp2)
        {
            common_ancestor = tmp1;
        }
        else
        {
            break;
        }
    }

    free(trace_val1);
    free(trace_val2);
    return common_ancestor;
}

void print_tree_top_view(TREE_NODE * root)
{
    QUEUE * q1;
    STACK * s1;
    TREE_NODE * tmp = NULL;
    TREE_NODE * ref_node = root;
    if (NULL == root)
        return;

    while(ref_node)
    {
        push(s1, (void *)(ref_node->left));
        ref_node = ref_node->left;
    }

    while(tmp = (TREE_NODE*)pop(s1))
        printf("%d, ", tmp->data);

    //printf("%d, ", root_node->data);
    ref_node = root;

    while(ref_node)
    {
        printf("%d, ", ref_node->data);
        ref_node = ref_node->right;
    }
}

#if 0
// Funtions to determine if a given tree is a Binary Search tree:
//int find_least_element_int_subtree
int find_greatest_element_in_subtree(TREE_NODE * root)
{
    int greatest_number;
    if (NULL != root->left)
    {
        greatest_number find_greatest_element_in_subtree(root->left);
    }
}
int is_bst(TREE_NODE * root)
{
    if (NULL == root)
        return -1;

    if (root->right && root->right->data < root->data)
        return -1;


}
#endif

//#####################        MISCELLANEOUS FUNCTIONS       ###################

void print_fibonacci(int seed, int ceiling)
{
    static int prev_val = 1;
    int tmp;

    if (seed < 1)
        return;

    printf("%d, ", seed);

    if ((seed + prev_val) > ceiling)
        return;

    tmp = seed;
    seed = seed + prev_val;
    prev_val = tmp;

    print_fibonacci(seed, ceiling);

    return;
}

void print_MxN_matrix()
{
    int tmp;
    char final_string[MAX_COLUMN + 1];
    char matrix[][MAX_COLUMN] = 
    { 'a', 'd', 'g', 'j', 'm', 'p', 's', 'v', 'y', '@',
        'b', 'e', 'h', 'k', 'n', 'q', 't', 'w', 'z', '#',
        'c', 'f', 'i', 'l', 'o', 'r', 'u', 'x', ' ', '*'
    };

    int row, column, r, c;
    for(row = 0; row < MAX_ROW; row++)
    {
        for(column = (MAX_COLUMN - 1); column >= 0 ; column--)
        {
            for(r = 0; r < MAX_ROW; r++)
            {
                for(tmp = 0; tmp <= MAX_COLUMN; tmp++)
                {
                    final_string[tmp] = 0x00;
                }
                for(c = 0; c < MAX_COLUMN; c++)
                {
                    if (c == column)
                    {
                        final_string[c] = matrix[r][c];
                    }
                    else
                    {
                        final_string[c] = matrix[row][c];
                    }
                }
                printf("%s\n", final_string);
            }
        }
        printf("\n");
    }
}


void mobile_number_strings(unsigned long long cell_number)
{
    int arr[10];
    char final_string[MAX_COLUMN + 1];
    char matrix[][MAX_COLUMN] = 
    { 'a', 'd', 'g', 'j', 'm', 'p', 's', 'v', 'y', '@',
        'b', 'e', 'h', 'k', 'n', 'q', 't', 'w', 'z', '#',
        'c', 'f', 'i', 'l', 'o', 'r', 'u', 'x', ' ', '*'
    };

    unsigned long long tmp = cell_number;
    int pos = 0, digit, row, row2, column, c, digit_main;
    unsigned long divisor = 1000000000;
    //printf("Cell number = %llu\n", cell_number);
    while(tmp)
    {
        digit = tmp / divisor;
        arr[pos] = digit;
        tmp = tmp % divisor;
        divisor = divisor / 10;
        pos++;
    }

    //printf("\nCell number entered = ");

    //for (pos = 0; pos < 10; pos++)
    //{
    //printf("%d", arr[pos] );
    //digit = arr[pos];
    for (row = 0; row < MAX_ROW; row++)
    {
        for (column = 0; column < MAX_COLUMN; column++ )
        {
            digit_main = arr[column];
            printf("\n%llu\n", cell_number);
            for (row2 = 0; row2 < MAX_ROW; row2++)
            {
                for(tmp = 0; tmp <= MAX_COLUMN; tmp++)
                {
                    final_string[tmp] = 0x00;
                }
                for (c = 0; c < MAX_COLUMN; c++)
                {
                    digit = arr[c];
                    if (digit == digit_main)
                    {
                        final_string[c] = matrix[row2][digit];
                    }
                    else
                    {
                        final_string[c] = matrix[row][digit];
                    }
                }
                printf("%s\n", final_string);
            }
        }
        printf("\n==========\n");
    }
}

void sizeof_array(int* array)
{
    int size_of_array = sizeof(array);

    printf("Sizeof array = %d\n", size_of_array);
}

//##############################################################################
//##############################################################################
int main(void) {
    // your code goes here
    TREE_NODE *my_tree = NULL;
    TREE_NODE * CA = NULL;
    int a[10][10] = {59, 04, 28, 93, 83, 84, 69, 30, 8, 86, 
                     168, 1, 69, 27, 77, 17, 14, 63, 691, 5,
                     19, 57, 74, 7, 79, 3, 424, 2, 38, 335,
                     71, 98, 85, 386, 16, 497, 60, 4, 92, 32,
                     59, 65, 166, 49, 118, 96, 414, 21, 102, 52, 
                     02, 36, 20, 1, 350, 49, 207, 783, 368, 690,
                     110, 25, 200, 59, 204, 48, 97, 76, 33, 19,
                     196, 75, 139, 26, 165, 180, 54, 90, 15, 403,
                     83, 426, 30, 40, 89, 172, 521, 5, 95, 36,
                     29, 47, 107, 567, 861, 215, 303, 278, 722, 862
    };
    int single_arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

    sizeof_array(single_arr);
    printf("The Size of this array is %d\n", sizeof(single_arr));

    //int val = 10;
    //TRAVERSE_TYPE T = POST_ORDER;
    /*
       int i, j;
       for (i = 0; i <10; i++)
       {
           for (j = 0; j < 10; j++)
           {
               a[i][j] = rand();
           }

       }

       for (i = 0; i <10; i++)
       {
           for (j = 0; j < 10; j++)
           {
               printf("a[%d][%d] = %d\n", i, j, a[i][j]);
           }
       }
     */

#if 0
    //#############################     TREE     ############################

    my_tree = construct_tree(a[0]);

    printf("\nIn Order Traversal: ");
    traverse_tree(my_tree, IN_ORDER, 0);

    printf("\nPre Order Traversal: ");
    traverse_tree(my_tree, PRE_ORDER, 0);

    printf("\nPost Order Traversal: ");
    traverse_tree(my_tree, POST_ORDER, 0);

    printf("\nBFS :");
    traverse_breadth_first(my_tree);

    printf("\nHeight of tree = %d", find_tree_height(my_tree));


    printf("\n\nCommon Ancestor: ");
    CA = find_common_ancestor(my_tree, 86, 69);
    if (CA)
    printf("Common ancestor is %d\n", CA->data);

    printf("/nPrinting Nodes Level-wise...:\n");
    print_peer_nodes(my_tree);

    delete_tree(my_tree);
#endif

    //print_MxN_matrix();
    //mobile_number_strings(7506929449);
    //print_fibonacci(1, 100);


    //#########################     LINKED LIST     ########################
    /*
       NODE *ll = create_linked_list(a[0]);
       print_linked_list(ll);
       printf("\nReversing Linked List: ");
       reverse_linked_list(&ll);
       print_linked_list(ll);

       printf("\nReversing using recursive function: ");
       reverse_ll_recursive(&ll);
       print_linked_list(ll);
     */
    return 0;
}
