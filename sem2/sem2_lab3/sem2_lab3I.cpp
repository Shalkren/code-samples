#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    int n,v,u;
    cin >> n;
    
    vector<vector<int>> tree(n);//Списки смежности для дерева
    vector<int> parent(n, -1);//Родители вершин
    vector<int> subtree_size(n, 1);//Размер поддерева (изначально 1 — сама вершина)
    
    //Считываем рёбра дерева (n-1 ребро)
    for (int i = 0; i < n - 1; ++i) {
        cin >> u >> v;
        u--; v--;//Переводим в 0-индексацию
        tree[u].push_back(v);
        tree[v].push_back(u);
    }
    
    queue<int> q;
    q.push(0);
    parent[0] = 0;//У корня нет родителей
    
    while (!q.empty()) {//Обход в ширину для нахождения родителей
        int node = q.front();
        q.pop();
        for (int child : tree[node]) {
            if (parent[child] == -1 && child != parent[node]) {
                parent[child] = node;
                q.push(child);
            }
        }
    }
    
    queue<int> leaf_queue;
    vector<int> children_count(n, 0);
    
    //Cчитаем количество детей для каждой вершины
    for (int i = 0; i < n; ++i) {
        children_count[i] = tree[i].size();
        if (i != 0) children_count[i]--;//У корня нет родителя, у остальных — есть
        if (children_count[i] == 0) {
            leaf_queue.push(i);//Листья сразу в очередь
        }
    }
    
    while (!leaf_queue.empty()) {
        int node = leaf_queue.front();
        leaf_queue.pop();
        if (node != 0) { //Не корень
            int p = parent[node];
            subtree_size[p] += subtree_size[node];//Добавляем размер поддерева ребёнка к родителю
            children_count[p]--;
            if (children_count[p] == 0) {
                leaf_queue.push(p);//Если у родителя больше нет необработанных детей
            }
        }
    }
    
    for (int i = 0; i < n; ++i) {
        cout << subtree_size[i] << " ";
    }
    
    return 0;
}
