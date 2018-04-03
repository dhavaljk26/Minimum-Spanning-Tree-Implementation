#include<bits/stdc++.h>
using namespace std;

vector<pair<int,int> >v[100005];

int done[100005];

struct heap{
    int val;
    int to;
    int from;
}h[2000005];

int sz=0;



void print(){
    printf("\nPrinting\n");
    for(int i=1;i<=sz;i++){
        printf("%d\n",h[i].val);
    }
    printf("\n");
}

void heapify(int node){
    if(node==1){
        return ;
    }

    int par=node/2;
    if(h[node].val<h[par].val){
        swap(h[node],h[par]);
        heapify(par);
    }
}

void insert(int v,int t,int f){
    sz++;
    h[sz].val=v;
    h[sz].to=t;
    h[sz].from=f;
    heapify(sz);
    //print();
}

pair<int,pair<int,int> >getmin(){
    return make_pair(h[1].val,make_pair(h[1].to,h[1].from));
}

void recurse(int node){
    int l=2*node,r=l+1;
    if(h[l].val==-1){
        return ;
    }

    else if(h[r].val==-1){
        if(h[node].val>h[l].val){
            swap(h[node],h[l]);
            return ;
        }
        return ;
    }

    if(h[node].val <= h[l].val && h[node].val<=h[r].val)
    return ;

    if(h[l].val<=h[r].val){
        
        swap(h[node],h[l]);
        recurse(l);
    }

    else{
        swap(h[node],h[r]);
        recurse(r);
    }
}


void deletemin(){
    swap(h[sz],h[1]);
    h[sz].val=-1;
    sz--;
    //print();
    recurse(1);
    //print();
}

int main(){
    int n,e;
    scanf("%d %d",&n,&e);

    for (int i=0;i<e;i++){
        int a,b,c;
        scanf("%d %d %d",&a,&b,&c);
        v[a].push_back(make_pair(b,c));
        v[b].push_back(make_pair(a,c));
    }

    for (int i=0;i<=4*e;i++){
        h[i].val=-1;
    }

    for (int i=0;i<v[1].size();i++){
        insert(v[1][i].second,1,v[1][i].first);
    }

    /*for (int i=0;i<n;i++){
        int choice;
        scanf("%d",&choice);
        if(choice==1){
            int x;
            scanf("%d",&x);
            insert(x,1,2);
        }
        else{
            pair<int,pair<int,int> >p=getmin();
            printf("min is %d\n",p.first);
            deletemin();
        }
    }*/

    clock_t start = clock();
    
    int cnt=1;
    long long ans=0;
    done[1]=1;

    while(cnt<n){
        pair<int,pair<int,int> >p=getmin();
        while(done[p.second.second]){
            deletemin();
            p=getmin();
        }

        ans+=p.first;

        int node=p.second.second;
        done[node]=1;

        for (int i=0;i<v[node].size();i++){
            if(!done[v[node][i].first]){
                insert(v[node][i].second,node,v[node][i].first);
            }
        }

        cnt++;
    }

    clock_t stop = clock();

    printf("COST = %lld\n",ans);

    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("Time elapsed in ms: %f\n", elapsed);

}