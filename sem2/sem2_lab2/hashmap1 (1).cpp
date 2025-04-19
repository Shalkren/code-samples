#include <iostream>
#include <vector>
#include <string>
using namespace std;
struct namepair{
    int num;
    string name;
    namepair(int n, string s): num(n), name(s){}
};

class hash_map{
    private:
    int size;
    const double loadFactor = 0.75;
    int count=0;
    vector<pair<namepair,long long>> table;
    long long polynomialHash(const string& s, int p = 31, long long m = 1e9 + 7) {
        long long hashValue = 0;
        long long power = 1;
        for (char ch : s) {
            hashValue = (hashValue + (ch - 'a' + 1) * power) % m;
            power = (power * p) % m;
        }
        return hashValue;
    }
    long long midsquarefuncHash(namepair key){
        long long squared = static_cast<long long>(key.num) * key.num;
        string squaredStr = to_string(squared);
        int length = squaredStr.length();
        int start = (length - size) / 2;
        int end = start + size;
        if (length < size) {
            squaredStr.insert(0, size - length, '0');
            start = 0;
            end = size;
        }
        string midDigits = squaredStr.substr(start, size);
        long long hashValue = stoll(midDigits)+polynomialHash(key.name);
    
        return hashValue;
    }
    int getIndex(const namepair& key){
        int h = midsquarefuncHash(key);
        int i = h % size;
        while (true) {
            if (table[i].first.num == key.num && table[i].first.name==key.name && table[i].second == h) {
                return i;
            }
            if (table[i].first.num == -1 && table[i].first.name=="") {
                return -1;
            }
            i = (i + 1) % size;
            if (i == h % size) break;
        }
        return -1;
    }
    int getIndex1(const namepair& key) {
        int h = midsquarefuncHash(key);
        int i = h % size;
        int startIndex = i;
        do{
            if ((table[i].first.num==-1 && table[i].first.name=="") || table[i].second == -1) {
                return i;
            }
            i = (i + 1) % size;
        }while (i != h % size); 
        rehash();
        return getIndex1(key);
    }
    bool brehash() {
        return count / size >= loadFactor;
    }
    void rehash() {
        int newSize = size * 2;
        vector<pair<namepair,long long>> newTable(newSize, {{-1,""}, -1});
        for (const auto& entry : table) {
            if (entry.first.num==-1 && entry.first.name=="") {
                continue;
            }
            long long h = midsquarefuncHash(entry.first);
            int i = h % newSize;
            while (newTable[i].first.num!=-1 || newTable[i].first.name!="") {
                i = (i + 1) % newSize;
            }
            newTable[i] = entry;
            
        }
        table = move(newTable);
        size = newSize;
    }
    public:
    // Конструкторы
    hash_map(int initSize = 8) : size(initSize), table(initSize, {{-1,""}, -1}) {}
    ~hash_map(){}

    // Основные операции
    void insert(const namepair& key){
        if (brehash()) {
            rehash();
        }
        long long h = midsquarefuncHash(key);
        int index = getIndex1(key);
        if (table[index].first.num == -1 && table[index].first.name == "") {
            table[index] = {key, h};
            count++;
        }
    } // Вставка элемента
    bool remove(const namepair& key) {
        int index = getIndex(key);
        if (index>=0 && table[index].first.num == key.num && table[index].first.name==key.name) {
            table[index] = {{-1,""}, -1};
            count--;
            return true;
        }
        return false;
    } // Удаление элемента
    bool find(const namepair& key){
        return getIndex(key) != -1;
    } // Поиск элемента
    void clear(){
        for(int i=0;i<size;i++){
            table[i]={{-1,""},-1};
        }
        count=0;
    }// Очистка таблицы

    // Дополнительные методы
    int hsize() const{
        return count;
    } // Количество элементов в таблице
    bool isEmpty() const{
        if(count==0){
            return true;
        }
        return false;
    } // Проверка на пустоту
    void print() const{
        if(isEmpty()){
            return;
        }
        for(auto& x : table){
            if(x.second!=-1){
                cout<<"key: "<<x.first.num<<" "<<x.first.name<<" value: "<<x.second<<"\n";
            }
        }
    } // Вывод таблицы
    // Итератор
    class iterator {
    private:
        hash_map& map;
        int index;

    public:
        iterator(hash_map& m, int i) : map(m), index(i) {}

        pair<namepair, long long>& operator*() {
            return map.table[index];
        }

        iterator& operator++() {
            do {
                index++;
            } while (index < map.size && (map.table[index].first.num==-1 && map.table[index].first.name==""));
            return *this;
        }

        bool operator!=(const iterator& other) const {
            return index != other.index;
        }
    };

    iterator begin() {
        int i = 0;
        while (i < size && table[i].first.num==-1 && table[i].first.name=="") {
            i++;
        }
        return iterator(*this, i);
    }

    iterator end() {
        return iterator(*this, size);
    }

};

int main(){
    hash_map a(4);
    for(int i=0;i<15;i++){
        a.insert({i,"hello"});
    }
    a.print();
    for (auto it = a.begin(); it != a.end(); ++it) {
        cout << "key: " << (*it).first.num<<" "<< (*it).first.name<< " value: " << (*it).second << "\n";
    }
    return 0;
}
