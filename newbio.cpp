#include<bits/stdc++.h>
using namespace std;

struct node{
    pair<int,pair<int,int> >val;
    node *child;
    node *parent,*left,*right;
    int degree;
};

int n=0;
vector <pair<int,int> > v[100005];
int done[100005];
node *root;

node *getnew(pair<int,pair<int,int> >p){
    node *tmp=(node *)malloc(sizeof(node));
    tmp->val=p;
    tmp->left=tmp;
    tmp->right=tmp;
    tmp->parent=NULL;
    tmp->child=NULL;
    tmp->degree=0;
}

pair<int,pair<int,int> >getmin(){

    return root->val;
}


int getval(node *n){
    return n->val.first;
}

void insertchild(node *x,node *y){
    x->degree++;
    if(x->child==NULL){
        y->left=y;
        y->right=y;
        y->parent=x;
        x->child=y;
    }

    else{
        node *cur=x->child;
        node *tmp=cur->left;
        node *h=getnew(y->val);
        h->degree=y->degree;
        h->left=tmp;
        h->right=cur;
        cur->left=h;
        tmp->right=h;

        if(cur->right==cur){
            cur->right=h;
        }
    }
}

void print(){
    return  ;
    node *tr=root;
    int cnt=0,tcnt=0;
    printf("\n");
    while((tr!=root || cnt==0) && tcnt<5){
        tcnt++;
        if(tr==root)
        cnt++;
        printf("%d(%d)->",getval(tr),tr->degree);
        tr=tr->right;
        if(tcnt>5)
        break;
    }
    printf("\n");
}

void consolidate(int f){
    node *tr=root;
    int cnt=0;
    vector<node *>v;
    while(cnt==0 || tr!=root){
        if(tr==root){
            cnt++;
        }

        //v.push_back(tr);
        if(getval(tr)<getval(root))
        root=tr;
        tr=tr->right;
    }

    while(cnt==0 || tr!=root){
        if(tr==root){
            cnt++;
        }

        v.push_back(tr);
        tr=tr->right;
    }
    //cout<<"size "<<v.size()<<endl;

    print();
    node *a[(int)log2(n)+3];

    for (int i=0;i<=log2(n)+2;i++){
        a[i]=NULL;
    }

    int flag=0;
    for (int i=0;i<v.size();i++){
        node *cur=v[i];
     //   printf("\ndegree %d\n",cur->degree);
        while(a[cur->degree]!=NULL){
            flag=1;
         //   printf("Entered at %d\n",getval(cur));
            node *y=a[cur->degree];
            if(cur->val>y->val){
                swap(cur,y);
            }
           // printf("\n--%d %d\n",getval(cur),getval(y));
                node *t1,*t2;
                t1=y->left;
                t2=y->right;
             //   printf("%d %d\n",getval(t1),getval(t2));
                t1->right=t2;
                t2->left=t1;
                insertchild(cur,y);
            
        }
       // cout<<"last degree "<<cur->degree<<endl;
        a[cur->degree]=v[i];
        
        if(!f && flag)
        break;
    }
  //  print();
}

void deletemin(){
    n--;
    if(n==0){
        root=NULL;
        return;
    }

    if(root->child){
        node *tr=root->child;
        int cnt=0;
        vector<node *>v;
        while(cnt==0 ||tr!=root->child){
            if(tr==root->child)
            cnt++;
            v.push_back(tr);
            tr=tr->right;
        }

        for (int i=0;i<v.size();i++){
            node *tmp=v[i],*tmp1=root->left;
            tmp->right=root;
            tmp->left=tmp1;
            tmp1->right=tmp;
            root->left=tmp;
            if(root->right==root){
                root->right=tmp;
            }
        }
        root->child=NULL;
    }

    node *t1,*t2;
    t1=root->left;
    t2=root->right;

    root=t1;

    t1->right=t2;
    t2->left=t1;

    int cnt=0;node *tr=root;
    while(cnt==0 || tr!=root){
        if(tr==root)
        cnt++;
        if(getval(tr)<getval(root))
        root=tr;
        tr=tr->right;
    }
    consolidate(1);
   // printf("\nDelete\n");
    print();
}
void insert(pair<int,pair<int,int> >p){
        n++;
    if(root==NULL){
        root=getnew(p);
        return ;
    }

    
    node *tr=root;
    int cnt=0;

    node *tmp=getnew(p),*tmp1=root->left;
    tmp->left=root->left;    
    tmp->right=root;
    tmp1->right=tmp;
    root->left=tmp;

    if(root->right==root){
        root->right=tmp;
    }
  //  print();

    consolidate(0);
    //printf("\nInsert\n");
    print();

    /*while(cnt==0 || tr!=root){
        if(tr==root){
            cnt++;
        }
        a[tr->degree]=tr;
        tr=tr->right;
    }

    node *tmp=getnew(p);*/
}


int main(){
    root=NULL;
    int n,e;
    int a,b,c;
    scanf("%d %d",&n,&e);

    for(int i=0;i<e;i++)
    {
        scanf("%d %d %d",&a,&b,&c);
        v[a].push_back(make_pair(b,c));
        v[b].push_back(make_pair(a,c));
    }    

    clock_t start = clock();

    done[1]=1;
    int cnt=1;
    for (int i=0;i<v[1].size();i++){
        insert(make_pair(v[1][i].second,make_pair(1,v[1][i].first)));
    }

    int cost=0;
    while(cnt<n){
        pair<int,pair<int,int> >p=getmin();
        while(done[p.second.second]){
            deletemin();
            p=getmin();
        }
        cost+=p.first;
        deletemin();
        int node=p.second.second;
        done[node]=1;
        cnt++;

        for (int i=0;i<v[node].size();i++){
            if(!done[v[node][i].first]){
                insert(make_pair(v[node][i].second,make_pair(node,v[node][i].first)));
            }
        }

    }

    clock_t stop = clock();

    printf("COST = %d\n",cost);

    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("Time elapsed in ms: %f\n", elapsed);

    //    print();

}