#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
using namespace std;

struct node
{
    int key;
    struct node *parent;            //Pointer to parent of the node
    struct node *child;             //Pointer to any of the children of the node
    struct node *left;              //Pointer to left sibiling of the node
    struct node *right;             //Pointer to right sibiling of the node
    int degree;                     //Number of children of the node
    bool mark;                      //Node is marked or not
};

struct node *A[20];

struct fibonacci_heap
{
    struct node *min;               //Pointer to node with minimum key value
    int n;                          //Number of nodes in the heap
};

void copy(struct node *a, struct node *b)
{
    a->key      = b->key;
    a->parent   = b->parent;  
    a->child    = b->child;     
    a->left     = b->left;  
    a->right    = b->right;
    a->degree   = b->degree;
    a->mark     = b->mark;        
}

void traverse_list(struct node *p)
{
    if(!p)
        return;
    struct node *t = p;

    printf("vfdvf ");
    do
    {
        printf("%d(%d %d) --->",t->key,t->left->key,t->right->key);
        t = t->right;
    }while(t != p);
        printf("%d(%d %d) --->",t->key,t->left->key,t->right->key);
    printf(" cdscd\n");
}

void insert_node_in_list(struct node *head, struct node *x)
{
    printf("INSIDE - ");
    struct node *t;
    t = head->left;

    head->left  = x;
    t->right    = x;

    x->right    = head;
    x->left     = t;    

    printf("AFTER  %d %d %d\n",x->key, head->key, t->key);
}


void print_fibonacci_heap(struct fibonacci_heap *ptr)
{
    printf("\n\n----------------------------------------------------------------------------------------------------------------");

    printf("\nNumber of nodes : %d\n", ptr->n);
    
    if(ptr->min == NULL)
    {
        printf("Heap is empty\n");
        printf("----------------------------------------------------------------------------------------------------------------\n\n");
        return;
    }

    struct node *x;
    queue<struct node *> q;

    q.push(ptr->min);

    while(!q.empty())
    {
        x = q.front();
        q.pop();

        if(x == NULL)
            break;
        struct node *temp;
        temp = x;
        if(temp->parent)
            printf("Parent of %d is %d\n",temp->key,temp->parent->key);
        printf("Traversing list with head having key = %d :",temp->key);
        do
        {
            printf(" %d(%c)[l-%d r-%d] ->",temp->key, (temp->mark?'*':' '), temp->left->key, temp->right->key);
            if(temp->child)
                q.push(temp->child);
            temp = temp->right;
        }while(temp!=x);

        printf("\n");
    }
    printf("Min - %d\n",ptr->min->key);
    printf("----------------------------------------------------------------------------------------------------------------\n\n");
}

void link(struct fibonacci_heap *H, struct node *y, struct node *x)
{
    y->left->right = y->right;
    y->right->left = y->left;

    if(x->child)
    {
        insert_node_in_list(x->child, y);
    }
    else
    {
        x->child = y;
        y->left = y->right = y;
    }

    y->parent = x;
    x->degree += 1;

    y->mark = false;
}

void consolidate(struct fibonacci_heap *H)
{
    for(int i=0;i<20;i++)
    {
        A[i] = NULL;
    }

    struct node *w, *x, *y, *visited;
    int d;
    
    w = visited = H->min;

    printf("check  a inside consolidate\n");
    print_fibonacci_heap(H);
    do
    {
        if(w == NULL)
            break;
        
        x = w;
        d = x->degree;

        printf("check b %p %d %d\n",w,w->key,d);


        while(A[d])
        {
            y = A[d];

            printf("check c %p %p %d %d %d\n",x,y,x->key,y->key,d);    
            if(x->key > y->key)
                swap(x, y);    
            
            if(visited == y)
                visited = x;

            link(H, y, x);

            printf("check d %p %p %d %d %d\n",x,y,x->key,y->key,d);
            A[d] = NULL;
            d++;    
        }

        A[d] = x;
        
        printf("check e %p %d %d %d\n",x,x->key,x->left->key,x->right->key);

        w = x->right;
    }while(w != visited);

    H->min = NULL;

    for(int i=0;i<20;i++)
    {
        if(A[i])
        {
            struct node *n = A[i];

            if(H->min)
            {
                insert_node_in_list(H->min, n);

                if(n->key < H->min->key)
                {
                    H->min = n;
                }        
            }
            else
            {
                n->left = n->right = n;
                H->min = n;        
            }
        }
    }
}

void cut(struct fibonacci_heap *H, struct node *x, struct node *y)
{
    struct node *temp;
    temp = y->child;
    
    while(temp->key != x->key)
    {
        temp = temp->right;
    }

    x = temp; 

    x->left->right = x->right;
    x->right->left = x->left;

    y->degree -= 1;

    insert_node_in_list(H->min, x);

    x->parent   = NULL;
    x->mark     = false;     
}

void cascading_cut(struct fibonacci_heap *H, struct node *y)
{
    struct node *z;

    z = y->parent;

    if(z)
    {
        if(y->mark)
        {
            cut(H, y, z);
            cascading_cut(H, z);
        }
        else
        {
            y->mark = true;
        }
    }
}

struct fibonacci_heap * create_fibonacci_heap()
{
    struct fibonacci_heap *H = (struct fibonacci_heap *)malloc(sizeof(struct fibonacci_heap));

    H->min = NULL;
    H->n   = 0;

    return H;
}

void insert_node(struct fibonacci_heap *H, int k)
{
    struct node *n = (struct node *)malloc(sizeof(struct node));

    n->key      = k;
    n->parent   = NULL;
    n->child    = NULL;
    n->degree   = 0;
    n->mark     = false;

    if(H->min == NULL)
    {
        n->left = n->right = n;
        H->min = n;    
    }
    else
    {
        insert_node_in_list(H->min, n);

        if(n->key < H->min->key)
        {
            H->min = n;
        }
    }

    H->n += 1;
}

int extract_min(struct fibonacci_heap *H)
{
    if(H->min == NULL)
    {
        printf("Extract min called on empty heap\n");
        return 0;
    }

    struct node *z, *it, *temp, *next;

    z = H->min;
    it = z->child;
   
    printf("check 1\n");
    for(int i=0; i < z->degree; i++ )
    {
        printf("wth - ");
        traverse_list(z);
        printf("extra %d %d\n",it->key,i);    
        next = it->right;

        temp = (struct node *)malloc(sizeof(struct node));
        copy(temp, it);
        temp->parent = NULL;
        printf("zz %d zz\n",temp->key);
        insert_node_in_list(z, temp);

        traverse_list(z);
        //Delete node it
        it->left->right = it->right;
        it->right->left = it->left;
    
        it = next;
    }
    printf("check 2\n");
    

    //Delete node z
    z->left->right = z->right;
    z->right->left = z->left;
    //z->child->parent = NULL;

    if(z == z->right)
    {
        H->min = NULL;
    }
    else
    {
        H->min = z->right;

        consolidate(H);
    
    }


    H->n -= 1;

    return z->key;
}

void decrease_key(struct fibonacci_heap *H, struct node *x, int k)
{
    if(k > x->key)
    {
        printf("New key is greater than current key\n");
        return;
    }

    x->key = k;

    struct node *y;
    y = x->parent;

    if(y && x->key < y->key)
    {
        cut(H, x, y);
        cascading_cut(H, y);        
    }

    if(x->key < H->min->key)
        H->min = x;
}

int main()
{
    struct fibonacci_heap *heap;

    heap = create_fibonacci_heap();

    while(1)
    {
        int x, y;
       // printf("1-insert 2-extract min 3-decrease key\n");
        scanf("%d",&x);

        if(x==1)
        {
            scanf("%d",&y);
            insert_node(heap, y);
        }
        else if(x==2)
        {
            printf("\nEXTRACT - %d\n",extract_min(heap));
        }
        else if(x==3)
        {

        }
        else
            return 0;

        print_fibonacci_heap(heap);
    }
    
    return 0;
}
