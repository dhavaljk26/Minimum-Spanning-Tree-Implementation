#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <math.h>
#include <vector>
#include <climits>
using namespace std;

vector< pair<int, int> > G[100005];
int debug_flag = 0;

struct node
{
    int key;
    struct node *parent;            //Pointer to parent of the node
    struct node *child;             //Pointer to any of the children of the node
    struct node *left;              //Pointer to left sibiling of the node
    struct node *right;             //Pointer to right sibiling of the node
    int degree;                     //Number of children of the node
    bool mark;                      //Node is marked or not
    int vertex_no;
};

struct fibonacci_heap
{
    struct node *min;               //Pointer to node with minimum key value
    int n;                          //Number of nodes in the heap
};

void copy(struct node *a, struct node *b)
{
    a->key          = b->key;
    a->parent       = b->parent;  
    a->child        = b->child;     
    a->left         = b->left;  
    a->right        = b->right;
    a->degree       = b->degree;
    a->mark         = b->mark;   
    a->vertex_no    = b->vertex_no;     
}

void traverse_list_util(struct node *p)
{
    if(!p)
        return;

    struct node *t = p;

    if(debug_flag)
        printf("\nInside Traverse ");

     do
    {
        if(debug_flag)
            printf("%d(%d %d) --->",t->key,t->left->key,t->right->key);
        t = t->right;
    }while(t != p);

    if(debug_flag)
        printf("\n");

}

void print_fibonacci_heap(struct fibonacci_heap *H)
{
    printf("\n\n----------------------------------------------------------------------------------------------------------------");
    printf("\nNumber of nodes : %d\n", H->n);
    
    if(H->min == NULL)
    {
        printf("Heap is empty\n");
        printf("----------------------------------------------------------------------------------------------------------------\n\n");
        return;
    }

    struct node *x;
    queue<struct node *> q;

    q.push(H->min);

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
            printf(" %d(%c)[%d] ->",temp->key, (temp->mark?'*':' '), temp->vertex_no);
            if(temp->child)
                q.push(temp->child);
            temp = temp->right;
        }while(temp!=x);

        printf("\n");
    }
    printf("Min - %d\n",H->min->key);
    printf("----------------------------------------------------------------------------------------------------------------\n\n");

}

struct node * find(struct fibonacci_heap *H, int k)
{
   
    if(H->min == NULL)
    {
        printf("Find called on Empty Heap\n");
        return NULL;
    }

    struct node *x;
    queue<struct node *> q;

    q.push(H->min);

    while(!q.empty())
    {
        x = q.front();
        q.pop();

        if(x == NULL)
            break;

        struct node *temp;
        temp = x;

        do
        {
            if(temp->key == k)
                return temp;

            if(temp->child)
                q.push(temp->child);

            temp = temp->right;
        }while(temp!=x);
    }

    return NULL;
}

void insert_node_in_list(struct node *head, struct node *x)
{
    if(debug_flag)
        printf("Inside Insert Node %d(%p) %d(%p)\n", head->key,head, x->key,x);
    
    struct node *t;
    t = head->left;

    head->left  = x;
    t->right    = x;

    x->right    = head;
    x->left     = t;    

    if(debug_flag)
        printf("AFTER Insert Node %d(%p) %d(%p) %d(%p)\n",x->key,x, head->key,head, t->key,t);
}

void link(struct fibonacci_heap *H, struct node *y, struct node *x)
{
    y->left->right = y->right;
    y->right->left = y->left;

    y->parent   = x;
    y->mark     = false;

    if(x->child)
    {
        insert_node_in_list(x->child, y);
    }    
    else
    {
        x->child = y->left = y->right = y;
    }

    x->degree += 1;
}

void consolidate(struct fibonacci_heap *H)
{
    int max = log2(H->n+1) + 5;
    struct node *A[max];

    for(int i=0;i<max;i++)
    {
        A[i] = NULL;
    }

    vector<struct node *> root;
    struct node *w, *x, *y;
    int d, i, n;
    
    w = H->min;

    do
    {
        if(debug_flag)
            printf("Going inside root %d\n",w->key);
        root.push_back(w);    
        w = w->right;
    }while(w!=H->min);

    n = root.size();

    if(debug_flag)
    {
        printf("check a inside consolidate\n");
        print_fibonacci_heap(H);
    }
    
    for(i=0; i<n; i++)
    {
        x = root[i];
        d = x->degree;

        if(debug_flag)
            printf("check b %d(%p) %d\n",x->key,x,d);

        while(A[d])
        {
            y = A[d];

            if(debug_flag)
                printf("check c %d(%p) %d(%p) %d\n",x->key,x,y->key,y,d);

            if(x->key > y->key)
            {
                swap(x, y);    
            }

            link(H, y, x);

            if(debug_flag)
                printf("check d %d(%p) %d(%p) %d\n",x->key,x,y->key,y,d);

            A[d] = NULL;
            d++;    
        }

        A[d] = x;
        
        if(debug_flag)
            printf("check e %d %d(%p) %d(%p) %d(%p)\n",d,x->key,x,x->left->key,x->left,x->right->key,x->right);
    }

    H->min = NULL;

    for(int i=0;i<max;i++)
    {
        if(A[i])
        {
            struct node *n = A[i];
            
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
        }
    }

    if(debug_flag)
    {
        printf("EXiting consolidate\n");
        print_fibonacci_heap(H);
    }
}


void cut(struct fibonacci_heap *H, struct node *x, struct node *y)
{
    
    x->left->right = x->right;
    x->right->left = x->left;

    if(debug_flag)
        printf("Inside cut %d %d %d %d\n", x->key, x->degree, y->key, y->degree);
    
    y->degree -= 1;

    if(debug_flag)
        printf("Inside cut %d %d %d %d\n", x->key, x->degree, y->key, y->degree);


    if(y->child == x)
        y->child = x->right;
    if(x == x->right)
        y->child = NULL;

    x->parent    = NULL;
    x->mark      = false; 

    insert_node_in_list(H->min, x); 
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


struct node * insert_node(struct fibonacci_heap *H, int k, int v)
{
    struct node *n = (struct node *)malloc(sizeof(struct node));

    n->key          = k;
    n->parent       = NULL;
    n->child        = NULL;
    n->degree       = 0;
    n->mark         = false;
    n->vertex_no    = v;

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

    return n;
}

struct node * extract_min(struct fibonacci_heap *H)
{
    if(H->min == NULL)
    {
        printf("Extract min called on empty heap\n");
        return NULL;
    }

    struct node *z, *it, *temp, *next;

    z = H->min;
    it = z->child;
   
    if(debug_flag)
        printf("check 1\n");

    for(int i=0; i < z->degree; i++)
    {
        next = it->right;
        if(debug_flag)
        {
            traverse_list_util(z);
            printf("extra %d %d\n",it->key,i);    
        }

        it->parent = NULL;
        
        insert_node_in_list(H->min, it);

        if(debug_flag)
        {
            traverse_list_util(z);
            printf("extra after insert %d %d\n",it->key,i);    
        }
    
        it = next;
    }

    if(debug_flag)
        printf("check 2\n");
    
    z->left->right = z->right;
    z->right->left = z->left;
    H->n -= 1;

    if(z == z->right)
    {
        H->min = NULL;
    }
    else
    {
        H->min = z->right;
        consolidate(H);
    }
    return z;
}

void decrease_key(struct fibonacci_heap *H, struct node *x, int k)
{
    if(x==NULL)
    {
        printf("Element not found\n");
        return;    
    }
    if(k > x->key)
    {
        printf("New key is greater than current key\n");
        return;
    }

    if(debug_flag)
    {
        printf("FOUND %d %p\n",x->key,x);
        printf("\n%p\n",x->parent);
    }
    x->key = k;

    struct node *y;
    y = x->parent;

    if(debug_flag)
        printf("C 1 %d \n", x->key);
    
    if(y && x->key < y->key)
    {
        if(debug_flag)
            print_fibonacci_heap(H);
        if(debug_flag)
            printf("Before cut %p %d %d %p %d %d\n",x, x->key, x->degree, y, y->key, y->degree);
        if(debug_flag)
            printf("C 2 %d %d\n", x->key, y->key);

        cut(H, x, y);
        cascading_cut(H, y);        
    }

    if(debug_flag)
        print_fibonacci_heap(H);

    if(debug_flag)
        printf("C 3 %d \n", x->key);

    if(x->key < H->min->key)
        H->min = x;
}

void prim(vector< pair<int, int> > G[], int n, int source)
{
    long long int cost = 0ll;
    int u, v, w, i;
    int par[n+1], visit[n+1];
    struct node *t, *map[n+1];

    struct fibonacci_heap *heap = create_fibonacci_heap();

    map[1]      = insert_node(heap, 0, 1);
    par[1]      = -1;
    visit[1]    = 0;

    for(i=2;i<=n;i++)
    {
        map[i]      = insert_node(heap, INT_MAX, i);
        par[i]      = -1;
        visit[i]    = 0;
    }

    printf("MST Traversal Edges\n");
    while(heap->n)
    {
        if(debug_flag)
            print_fibonacci_heap(heap);
        t = extract_min(heap);

        cost += t->key;

        u = t->vertex_no;
        visit[u] = 1;
        printf("(%d , %d)\n",par[u],u);

        for(i=0;i<G[u].size();i++)
        {
            v = G[u][i].first;
            w = G[u][i].second;

            if(!visit[v] && w < map[v]->key)
            {
                par[v] = u;
                decrease_key(heap, map[v], w);
            }
        }
    }

    printf("COST = %lld\n",cost);
}

int main()
{
    // struct fibonacci_heap *heap = create_fibonacci_heap();
    // int i=1;
    // scanf("%d",&debug_flag);
    // while(1)
    // {
    //     int x, y, z;

    //     printf("\nSTEP %d\n",i++);
    //     scanf("%d",&x);

    //     if(x==1)
    //     {
    //         scanf("%d",&y);
    //         insert_node(heap, y, 0);
    //     }  
    //     else if(x==2)
    //     {
    //         printf("EXTRACT MIN %d\n", (extract_min(heap))->key);
    //     }  
    //     else if(x==3)
    //     {
    //         scanf("%d%d",&y,&z);
    //         decrease_key(heap, find(heap, y), z);
    //     }
    //     else if(x==4)
    //     {
    //         print_fibonacci_heap(heap);
    //     }
    //     else
    //         return 0;
    // }
    int v, e, x, y, w, i;

    scanf("%d %d",&v, &e);

    for(int i = 0; i<e; i++)
    {
        scanf("%d%d%d",&x,&y,&w);

        G[x].push_back(make_pair(y, w));
        G[y].push_back(make_pair(x, w));
    }

    prim(G, v, 1);


    return 0;
}
