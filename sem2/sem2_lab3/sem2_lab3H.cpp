#include <vector>
#include <iostream>
#include <string>

using namespace std;
struct node{
    int key;
    int h;
    node* l;
    node* r;
    node* parent;
    node(): key(0),h(0),l(nullptr),r(nullptr),parent(nullptr){}
    node(int k,int he, node* p = nullptr): key(k),h(he),l(nullptr),r(nullptr),parent(p){}
    ~node() {
        delete l;
        delete r;
    }
    void insert(int k){
        if(key==k){
            return;
        }
        if(k<key && l==nullptr){
            l= new node(k,h+1,this);
            return;
        }
        if(k>key && r==nullptr){
            r= new node(k,h+1,this);
            return;
        }
        if(k<key && l){
            l->insert(k);
        }
        if(k>key && r){
            r->insert(k);
        }
    }

    bool count(int k){
        if(key==k){
            return true;
        }
        if(k<key && l){
            return l->count(k);
        }
        if(k>key && r){
            return r->count(k);
        }
        return false;
    }
    void print(){
        if(l){
            l->print();
        }
        for(int i=0;i<h;i++){
            cout<<".";
        }
        cout<<key<<endl;
        if(r){
            r->print();
        }
        return;
    }
};

int main(){
    string command;
    int number,c=0;
    node* tree=nullptr;
    while(cin>>command){
        if(command=="ADD" && c==0){
            cin>>number;
            tree=new node(number,0, nullptr);
            cout<<"DONE\n";
            c++;
            continue;
        }
        if(command=="SEARCH" && c==0){
            cin>>number;
            cout<<"NO\n";
            continue;
        }
        if(command=="ADD"){
            cin>>number;
            if(tree->count(number)){
                cout<<"ALREADY\n";
                continue;
            }
            tree->insert(number);
            cout<<"DONE\n";
            continue;
        }
        if(command=="SEARCH"){
            cin>>number;
            if(tree->count(number)){
                cout<<"YES\n";
                continue;
            }
            cout<<"NO\n";
            continue;
        }
        if(command=="PRINTTREE"){
            tree->print();
        }
    }
    delete tree;
    return 0;
}