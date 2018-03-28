#include <bits/stdc++.h>
using namespace std;

vector<pair<int,int> >v[100005];

int done[100005];

int main(){
    int n,e;
    scanf("%d %d",&n,&e);

    for (int i=0;i<e;i++){
        int a,b,w;
        scanf("%d %d %d",&a,&b,&w);
        v[a].push_back(make_pair(b,w));
        v[b].push_back(make_pair(a,w));
    }

    done[1]=1;
    priority_queue<pair<int,pair<int,int> > >pq;

    for (int i=0;i<v[1].size();i++){
        pair<int,pair<int,int> >p;
        p.first=-v[1][i].second;
        p.second.first=1;
        p.second.second=v[1][i].first;
        pq.push(p);
    }
    
    long long sum=0;
    int cnt=1;
    while(cnt<n){
        pair<int,pair<int,int> >p=pq.top();
        while(done[p.second.second]){
            pq.pop();
            p=pq.top();
        }
        pq.pop();
        done[p.second.second]=1;
        sum+=-p.first;
        int node=p.second.second;
        for (int i=0;i<v[node].size();i++){
            p.first=-v[node][i].second;
            p.second.first=node;
            p.second.second=v[node][i].first;
            pq.push(p);
        }
        cnt++;
    }

    cout<<"Mst is "<<sum<<endl;
}