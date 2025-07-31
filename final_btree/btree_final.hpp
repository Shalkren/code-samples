#pragma once
#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <chrono>
#include <ctime>
using ll = long long;
using namespace std;
namespace fs = std::filesystem;

struct Date {
    string date;
    Date() {
        getdate();
    }
    void getdate() {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm local_time;
        #ifdef _WIN32
            localtime_s(&local_time, &now_time);
        #else
            localtime_r(&now_time, &local_time);
        #endif
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%d.%m.%Y", &local_time);
        date = "Date: ";
        date += buffer;
    }
    string str() const {
        return date;
    }
};

class FileSystemNode : public std::enable_shared_from_this<FileSystemNode> {
    protected:
        ll id;
    public:
        ll getID() const { return id; }
        void rewriteid(ll t){
            id=t;
        }
public:
    virtual void setPath(const string& new_path) = 0;
    virtual string getPath() const = 0;
    virtual void changeDate() = 0;
    virtual void writeToFile(const string& content) = 0;
    virtual ~FileSystemNode() = default;
    virtual void print1() const = 0; // Виртуальный метод для вывода
    virtual string getHashString() const = 0; // Для вычисления хеша
    virtual shared_ptr<FileSystemNode> toBtreeElem() {
        return shared_from_this();
    }
};
string normalizePath(const string& path) {
    fs::path p(path);
    p = p.lexically_normal();
    string str = p.string();
    if (!str.empty() && (str.back() == '\\' || str.back() == '/')) {
        str.pop_back();
    }
    return str;
}

template <typename date, typename path, typename dirName, typename Type>
struct File;

template <typename date, typename path, typename dirName>
struct Directory;

template <typename T, typename D>
bool operator==(const pair<T, shared_ptr<D>>& a, const pair<T, shared_ptr<D>>& b) {
    return a.first == b.first && a.second->getHashString() == b.second->getHashString();
}

template <typename T, typename D>
class btree{
    protected:
        struct node {
        vector<pair<T,shared_ptr<D>>> values;
        vector<node*> p_children;
        bool is_leaf;
        ll size;
        node(ll t, bool leaf): size(0), is_leaf(leaf){
            values.resize(2*t-1);
            p_children.resize(2*t, nullptr);
        }
        ~node(){}
    };
    protected:
        unordered_map<T, shared_ptr<D>> idlist;
        ll t;    
        node* root;
        pair<T, shared_ptr<D>> hashfunc(shared_ptr<D> data) {
            T id;
            const int k = 11, mod = 1e9 + 7;
            string s = data->getHashString();
            ll h = 0, m = 1;

            for (char c : s) {
                int x = static_cast<int>(c - 'a' + 1);
                h = (h + m * x) % mod;
                m = (m * k) % mod;
            }

            id = h;
            while (idlist.count(id)) id++;
            data->rewriteid(id); 
            idlist[id] = data;
            return {id, data};
        }
    public:
        btree(ll degree) : t(degree), root(nullptr){}
        ~btree() {
            deleteTree(root);
        }
        node* getRoot() const { return root; }
    public:
        void insert(shared_ptr<D> data) {
            pair<T, shared_ptr<D>> key = hashfunc(data);
            if (root == nullptr) {
                root = new node(t, true);
                root->values[0] = hashfunc(data);
                root->size = 1;
            } else {
                if (root->size == 2*t - 1) {
                    node* new_root = new node(t, false);
                    new_root->p_children[0] = root;
                    splitChild(new_root, 0, root);
                    
                    ll i = 0;
                    if (new_root->values[0].first < key.first)
                        i++;
                    insertNotFull(new_root->p_children[i], key);
                    
                    root = new_root;
                } else {
                    insertNotFull(root, key);
                }
            }
        }

        bool contains(pair<T, shared_ptr<D>> key) {
            return (search(root, key) != nullptr);
        }

        pair<T, shared_ptr<D>> searchByName(const string& name, const string& path) const {
            return searchByName(root, name, path);
        }

        void print() const {
            if (root) {
                print(root);
                cout << flush;
            }
        }

        void getAllElements(vector<shared_ptr<D>>& elements) {
            getAllElements(root, elements);
        }
    protected:

        void getAllElements(node* node, vector<shared_ptr<D>>& elements) {
            if (!node) return;
            for (ll i = 0; i < node->size; ++i) {
                elements.push_back(node->values[i].second);
                if (!node->is_leaf) {
                    getAllElements(node->p_children[i], elements);
                }
            }
            if (!node->is_leaf) {
                getAllElements(node->p_children[node->size], elements);
            }
        }

        void deleteTree(node* node) {
            if (node) {
                if (!node->is_leaf) {
                    for (ll i = 0; i <= node->size; ++i)
                        deleteTree(node->p_children[i]);
                }
                delete node;
            }
        }

        void splitChild(node* parent, ll idx, node* child) {
            node* new_node = new node(t, child->is_leaf);
            new_node->size = t - 1;

            for (ll j = 0; j < t-1; ++j)
                new_node->values[j] = child->values[j + t];

            if (!child->is_leaf) {
                for (ll j = 0; j < t; ++j)
                    new_node->p_children[j] = child->p_children[j + t];
            }

            child->size = t - 1;

            for (ll j = parent->size; j >= idx+1; --j)
                parent->p_children[j+1] = parent->p_children[j];

            parent->p_children[idx+1] = new_node;

            for (ll j = parent->size-1; j >= idx; --j)
                parent->values[j+1] = parent->values[j];

            parent->values[idx] = child->values[t-1];
            parent->size += 1;
        }

        void insertNotFull(node* node, pair<T, shared_ptr<D>> key) {
            ll i = node->size - 1;

            if (node->is_leaf) {
                while (i >= 0 && node->values[i].first > key.first) {
                    node->values[i+1] = node->values[i];
                    i--;
                }
                node->values[i+1] = key;
                node->size += 1;
            } else {
                while (i >= 0 && node->values[i].first > key.first)
                    i--;
                i++;

                if (node->p_children[i]->size == 2*t - 1) {
                    splitChild(node, i, node->p_children[i]);
                    if (node->values[i].first < key.first)
                        i++;
                }
                insertNotFull(node->p_children[i], key);
            }
        }

        node* search(node* node, pair<T, shared_ptr<D>> key) {
            ll i = 0;
            while (i < node->size && key.first > node->values[i].first)
                i++;

            if (i < node->size && key == node->values[i])
                return node;

            if (node->is_leaf)
                return nullptr;

            return search(node->p_children[i], key);
        }
        
        pair<T, shared_ptr<D>> searchByName(node* current_node, const string& name, const string& path) const {
            if (!current_node) {
                cout << "[DEBUG] Current node is null" << endl;
                return {};
            }
            cout << "[DEBUG] Searching for: " << name << " at " << path << endl;
            // 1. Поиск в текущем узле
            for (ll i = 0; i < current_node->size; ++i) {
                const auto& val = current_node->values[i];
                shared_ptr<D> elem = val.second;

                // Проверка для файлов
                if (auto file = dynamic_pointer_cast<File<Date, string, string, string>>(elem)) {
                    fs::path file_parent_path = fs::path(file->path1).parent_path();
                    if (file->dirName1 == name && file_parent_path == fs::path(path)) {
                        return val;
                    }
                }

                // Проверка для директорий
                else if (auto dir = dynamic_pointer_cast<Directory<Date, string, string>>(elem)) {
                    fs::path dir_path_normalized = fs::path(dir->path1).lexically_normal();
                    fs::path target_path_normalized = (fs::path(path)).lexically_normal();
                    

                    // Убираем завершающий слэш для сравнения
                    string dir_path_str = dir_path_normalized.string();
                    if (dir_path_str.back() == '\\' || dir_path_str.back() == '/') {
                        dir_path_str.pop_back();
                    }

                    string target_path_str = target_path_normalized.string();
                    if (target_path_str.back() == '\\' || target_path_str.back() == '/') {
                        target_path_str.pop_back();
                    }
                    if (dir->dirName1 == name && dir_path_str == target_path_str) {
                        return val;
                    }
                    // Рекурсивный поиск внутри директории
                    if (dir->data.getRoot()) {
                        auto res_in_child = searchByName(dir->data.getRoot(), name, path);
                        if (res_in_child.second) return res_in_child;
                    }
                }
            }

            // 2. Рекурсивный поиск в дочерних узлах (если current_node не лист)
            if (!current_node->is_leaf) {
                for (ll i = 0; i <= current_node->size; ++i) {
                    auto res = searchByName(current_node->p_children[i], name, path);
                    if (res.second) return res;
                }
            }

            return {};
        }
        

        void print(const node* node) const {
            if (!node) return;
            if (node == root && node->size == 0) {
                cout << "Empty"<< endl;
                return;
            }
            for (ll i = 0; i < node->size; ++i) {
                node->values[i].second->print1();
                if (!node->is_leaf){
                    print(node->p_children[i]);
                    cout<<flush;
                }
            }
            if (!node->is_leaf) 
                print(node->p_children[node->size]);
                cout<< flush;
        }
        
        //удаление элемента
    protected:
    
        void removeFromLeaf(node* Node, ll idx) {
            for (ll i = idx + 1; i < Node->size; ++i)
                Node->values[i - 1] = Node->values[i];
            Node->size--;
        }

        pair<T, shared_ptr<D>> getMaxFromLeft(node* Node, ll idx) {
            node* curr = Node->p_children[idx];
            while (!curr->is_leaf)
                curr = curr->p_children[curr->size];
            return curr->values[curr->size - 1];
        }

        pair<T, shared_ptr<D>> getMinFromRight(node* Node, ll idx) {
            node* curr = Node->p_children[idx + 1];
            while (!curr->is_leaf)
                curr = curr->p_children[0];
            return curr->values[0];
        }

        void borrowFromPrev(node* Node, ll idx) {
            if (!Node || idx >= Node->p_children.size() - 1) return;
            node* child = Node->p_children[idx];
            node* sibling = Node->p_children[idx - 1];
            if (!child || !sibling || sibling->size == 0) return;
            for (ll i = child->size - 1; i >= 0; --i)
                child->values[i + 1] = child->values[i];

            if (!child->is_leaf) {
                for (ll i = child->size; i >= 0; --i)
                    child->p_children[i + 1] = child->p_children[i];
            }

            child->values[0] = Node->values[idx - 1];
            if (!child->is_leaf)
                child->p_children[0] = sibling->p_children[sibling->size];

            Node->values[idx - 1] = sibling->values[sibling->size - 1];
            child->size++;
            sibling->size--;
        }

        void borrowFromNext(node* Node, ll idx) {
            if (!Node || idx >= Node->p_children.size() - 1) return;
            node* child = Node->p_children[idx];
            node* sibling = Node->p_children[idx + 1];
            if (!child || !sibling || sibling->size == 0) return;
            child->values[child->size] = Node->values[idx];
            child->size++;
            if (!child->is_leaf)
                child->p_children[child->size] = sibling->p_children[0];

            Node->values[idx] = sibling->values[0];
            for (ll i = 0; i < sibling->size-1; i++)
                sibling->values[i] = sibling->values[i+1];
            if (!sibling->is_leaf) {
                for (ll i = 0; i < sibling->size; ++i) {
                    sibling->p_children[i] = sibling->p_children[i + 1];
                }
                sibling->p_children[sibling->size] = nullptr;
            }
            sibling->size--;
        }

        void merge(node* Node, ll idx) {
            node* child = Node->p_children[idx];
            node* sibling = Node->p_children[idx + 1];

            child->values[t - 1] = Node->values[idx];

            for (ll i = 0; i < sibling->size; ++i)
                child->values[i + t] = sibling->values[i];

            if (!child->is_leaf) {
                for (ll i = 0; i <= sibling->size; ++i)
                    child->p_children[i + t] = sibling->p_children[i];
            }

            for (ll i = idx + 1; i < Node->size; ++i)
                Node->values[i - 1] = Node->values[i];

            for (ll i = idx + 2; i <= Node->size; ++i)
                Node->p_children[i - 1] = Node->p_children[i];

            child->size += sibling->size + 1;
            Node->size--;
        }

        void fill(node* Node, ll idx) {
            if (idx != 0 && Node->p_children[idx - 1]->size >= t)
                borrowFromPrev(Node, idx);
            else if (idx != Node->size && Node->p_children[idx + 1]->size >= t)
                borrowFromNext(Node, idx);
            else {
                if (idx != Node->size)
                    merge(Node, idx);
                else
                    merge(Node, idx - 1);
            }
        }

    public:
        void remove(shared_ptr<D> data) {
            ll target_id = data->getID(); // Используем сохраненный ID
            auto key = make_pair(target_id, data);
            remove(root, key);

            if (root->size == 0) {
                if (root->is_leaf)
                    delete root;
                else
                    root = root->p_children[0];
            }
        }

    protected:
        void remove(node* node, pair<T, shared_ptr<D>> key) {
            if (!node) return;
            ll idx = 0;
            while (idx < node->size && node->values[idx] < key)
                ++idx;

            if (idx < node->size && node->values[idx] == key) {
                if (node->is_leaf)
                    removeFromLeaf(node, idx);
                else
                    removeFromNonLeaf(node, idx);
            } else {
                if (node->is_leaf) return;

                bool flag = (idx == node->size);
                if (node->p_children[idx]->size < t)
                    fill(node, idx);

                if (flag && idx > node->size)
                    remove(node->p_children[idx - 1], key);
                else
                    remove(node->p_children[idx], key);
            }
        }

        void removeFromNonLeaf(node* node, ll idx) {
            pair<T, shared_ptr<D>> k = node->values[idx];

            if (node->p_children[idx]->size >= t) {
                auto pred = getMaxFromLeft(node, idx);
                node->values[idx] = pred;
                remove(node->p_children[idx], pred);
            } else if (node->p_children[idx + 1]->size >= t) {
                auto succ = getMinFromRight(node, idx);
                node->values[idx] = succ;
                remove(node->p_children[idx + 1], succ);
            } else {
                merge(node, idx);
                remove(node->p_children[idx], k);
            }
        }
};


template <typename date = Date, typename path = string, typename dirName = string>
struct Directory : public FileSystemNode{
    date date1;
    path path1;
    dirName dirName1;
    btree<ll, FileSystemNode> data;
    Directory() : date1(), path1(""), dirName1(""), data(3) {}
    Directory(ll t) : date1(), path1(""), dirName1(""), data(t) {}
    Directory(path path2, dirName dirName2) 
    : date1(), path1(normalizePath(path2)), dirName1(dirName2), data(3){}
    Directory(path path2, dirName dirName2, ll t): date1(), path1(normalizePath(path2)), dirName1(dirName2), data(t){}
    Directory(path path2, dirName dirName2, btree<ll, FileSystemNode> data1):
        date1(), path1(normalizePath(path2)), dirName1(dirName2), data(data1){}
    void print1() const override {
        cout << "Directory: " << dirName1 << " " << date1.str() << " Path: " << path1 << endl <<"Inside dir:" << endl;
        data.print();
        cout<<flush;
    }
    void printN() const {
        cout << "Directory: " << dirName1 << " " << date1.str() << " Path: " << path1 << endl;
    }

    string getHashString() const override {
        return path1 + dirName1;
    }
   
    void changeDate(){
        Date x;
        date1=x;
        cout<<"In ";
        printN();
        cout<<"date changed to: "<<date1.str()<<endl;
    }
    shared_ptr<FileSystemNode> toBtreeElem() override{
        return shared_from_this();
    }
    void clear() {
        vector<shared_ptr<FileSystemNode>> elements;
        data.getAllElements(elements);
        for (auto& elem : elements) {
            data.remove(elem->toBtreeElem());
        }
    }
    void setPath(const string& new_path) override { path1 = new_path; }
    string getPath() const override { return path1; }
    void writeToFile(const string& content) override {
        cerr << "Error: Cannot write to a directory!" << endl;
    }
};


template <typename date = Date, typename path = string, typename dirName = string, typename Type = string>
struct File : public FileSystemNode{
    date date1;
    path path1;
    dirName dirName1;
    Type type1;
    File() : date1(), path1(""), dirName1(""), type1("") {}
    File(path path2, dirName dirName2, Type type2) 
    : date1(), path1(normalizePath(path2)), dirName1(dirName2), type1(type2) {}
    void print1() const override {
        cout << "File: " << dirName1 << " " << date1.str() << " Path: " << path1 << " Type: " << type1 << endl;
        cout << flush;
    }

    string getHashString() const override {
        return path1 + dirName1 + type1;
    }

    void changeDate(){
        Date x;
        date1=x;
        cout<<"In ";
        print1();
        cout<<"date changed to: "<<date1.str()<<endl;
    }

    shared_ptr<FileSystemNode> toBtreeElem() override{
        return shared_from_this();
    }
    
    void writeToFile(const string& content) {
        fs::path full_path = fs::path(path1);
        ofstream file(full_path, ios::app); // Режим добавления
        if (file.is_open()) {
            file << content << endl;
            file.close();
            changeDate(); // Обновляем дату
        }
    }
    void setPath(const string& new_path) override { path1 = new_path; }
    string getPath() const override { return path1; }
};

string getFileExtension(const string& filename) {
    size_t dotPos = filename.find_last_of('.');
    return (dotPos != string::npos) ? filename.substr(dotPos + 1) : "";
}


bool pathExists(const string& path) {
    try {
        fs::path fs_path(path);
        return fs::exists(fs_path);
    } 
    catch (const exception& e) {
        return false;
    }
}

bool directoryExists(const string& path) {
    try {
        fs::path dir_path(path);
        return fs::exists(dir_path) && fs::is_directory(dir_path);
    } 
    catch (const fs::filesystem_error& e) {
        return false;
    }
}

bool containsChar(const string& str, char target) {
    return (str.find(target) != string::npos);
}
//bool createF
//bool createD
bool createF(const string& name, const string& parent_path, File<Date, string, string, string>& file) {
    // Проверка имени файла
    if (name.empty() || name.find_first_of("\\/:*?\"<>|") != string::npos) {
        cerr << "Error: Invalid file name '" << name << "'" << endl;
        return false;
    }

    // Проверка родительской директории
    if (!fs::exists(parent_path)) {
        cerr << "Error: Parent path '" << parent_path << "' does not exist" << endl;
        return false;
    }
    if (!fs::is_directory(parent_path)) {
        cerr << "Error: '" << parent_path << "' is not a directory" << endl;
        return false;
    }

    // Формирование полного пути
    fs::path full_path = fs::path(parent_path) / name;
    file.dirName1 = name;
    file.path1 = full_path.string();

    try {
        // Создание файла
        ofstream fstream(full_path);
        if (!fstream.is_open()) {
            cerr << "Error: Failed to create file '" << full_path << "'" << endl;
            return false;
        }
        fstream.close();

        // Установка типа и даты
        file.type1 = getFileExtension(name);
        file.changeDate();
        return true;
    } 
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return false;
    }
}


bool createD(const string& name, const string& parent_path, Directory<Date, string, string>& dir) {
    // Проверка имени директории
    if (name.empty() || name.find_first_of("\\/:*?\"<>|") != string::npos) {
        cerr << "Error: Invalid directory name '" << name << "'" << endl;
        return false;
    }

    // Проверка родительской директории
    if (!fs::exists(parent_path)) {
        cerr << "Error: Parent path '" << parent_path << "' does not exist" << endl;
        return false;
    }
    if (!fs::is_directory(parent_path)) {
        cerr << "Error: '" << parent_path << "' is not a directory" << endl;
        return false;
    }

    // Формирование полного пути
    fs::path full_path = fs::path(parent_path) / name;
    dir.dirName1 = name;
    dir.path1 = full_path.lexically_normal().string();

    try {
        // Создание директории
        if (fs::exists(full_path)) {
            cout<<"hueta "<< name<<endl;
            if (!fs::is_directory(full_path)) {
                cerr << "Error: '" << full_path << "' exists but is not a directory" << endl;
                return false;
            }
        } else {
            cout<<"here "<< name<<endl;
            if (!fs::create_directory(full_path)) {
                cerr << "Error: Failed to create directory '" << full_path << "'" << endl;
                return false;
            }
        }

        // Обновление даты
        dir.changeDate();
        return true;
    } 
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return false;
    }
}

struct newproject{
    Directory<Date, string, string> project;
    newproject(){
        Directory<Date, string, string> x;
        project=x;
    }
    newproject(ll t){
        Directory<Date, string, string> x(t);
        project=x;
    }
    newproject(string path2, string dirName2){
        Directory<Date, string, string> x(path2,dirName2);
        project=x;
    }
    newproject(string path2, string dirName2, ll t){
        Directory<Date, string, string> x(path2,dirName2,t);
        project=x;
        createD(dirName2, path2, project);
    }
    newproject(string path2, string dirName2, btree<ll, FileSystemNode> data1){
        Directory<Date, string, string> x(path2,dirName2,data1);
        project=x;
        createD(dirName2, path2, project);
    }
    string parent_path(const string& path) {
        fs::path p(path);
        return p.parent_path().string();
    }
    string get_directory_name(const string& path) {
        fs::path p(path);
        if (p.filename().empty()) 
            p = p.parent_path();
        cout<<p.filename().string()<< " "<< path<<endl;
        return p.filename().string();
    }
    void updatePaths(shared_ptr<Directory<Date, string, string>> dir, const string& new_base) {
        vector<shared_ptr<FileSystemNode>> children;
        dir->data.getAllElements(children);
        
        for (auto& child : children) {
            if (auto file = dynamic_pointer_cast<File<Date, string, string, string>>(child)) {
                file->setPath((fs::path(new_base) / file->dirName1).string());
            } 
            else if (auto subdir = dynamic_pointer_cast<Directory<Date, string, string>>(child)) {
                subdir->setPath((fs::path(new_base) / subdir->dirName1 / "").string());
                updatePaths(subdir, subdir->getPath());
            }
        }
    }
    shared_ptr<FileSystemNode> FIND(const string& name, const string& path) {
        cout << "[DEBUG] FIND called: " << name << " in " << path << endl;
        
        // Нормализация входного пути
        fs::path target_dir = fs::path(path).lexically_normal();
        string normalized_target_dir = normalizePath(target_dir.string());
        fs::path target_file_path = target_dir;
        string normalized_target_file = normalizePath(target_file_path.string());

        // Поиск элемента в B-дереве
        auto result = project.data.searchByName(name, path);
        
        if (!result.second) {
            cout << "[ERROR] Element not found" << endl;
            return nullptr;
        }

        // Проверка для файлов
        if (auto file = dynamic_pointer_cast<File<Date, string, string, string>>(result.second)) {
            string file_path = normalizePath(file->path1);
            string expected_path = normalizePath((target_dir / name).string());
            if (file_path != expected_path) {
                cout << "[DEBUG] File path mismatch:\n"
                    << "  Actual:   " << file_path << "\n"
                    << "  Expected: " << expected_path << endl;
                return nullptr;
            }
        }
        // Проверка для директорий
        else if (auto dir = dynamic_pointer_cast<Directory<Date, string, string>>(result.second)) {
            string dir_path = normalizePath(dir->path1);
            string expected_path = normalized_target_file; // Путь директории включает её имя
            
            if (dir_path != expected_path) {
                cout << "[DEBUG] Directory path mismatch:\n"
                    << "  Actual:   " << dir_path << "\n"
                    << "  Expected: " << expected_path << endl;
                return nullptr;
            }
        }

        return result.second;
    }
    
    
    void CREATEf(const string& name, const string& parent_path) {
        // 1. Найти родительскую директорию

        auto parent_dir = FIND(get_directory_name(parent_path), parent_path);
        if (!parent_dir) {
            cout << "[Error] Parent directory not found: " << parent_path << endl;
            return;
        }

        // 2. Создать shared_ptr файла
        auto f = make_shared<File<Date, string, string, string>>();
        
        // 3. Инициализировать файл через createF
        if (createF(name, parent_path, *f)) {
            // 4. Добавить файл в B-дерево родительской директории
            if (auto dir = dynamic_pointer_cast<Directory<Date, string, string>>(parent_dir)) {
                dir->data.insert(f);
                cout << "[Success] File added to directory: " << parent_path << endl;
            }
        }
    }

    void CREATEd(const string& name, const string& parent_path, ll t) {
        if (!fs::exists(parent_path)) {
            cerr << "[Error] Parent directory does not exist: " << parent_path << endl;
            return;
        }

        
        // 1. Найти родительскую директорию
        auto parent_dir = FIND(get_directory_name(parent_path), parent_path);
        if (!parent_dir) {
            cout << "[Error] Parent directory not found: " << parent_path << endl;
            return;
        }

        // 2. Создать shared_ptr директории
        auto d = make_shared<Directory<Date, string, string>>(t);
        
        // 3. Инициализировать директорию через createD
        if (createD(name, parent_path, *d)) {
            if (auto dir = dynamic_pointer_cast<Directory<Date, string, string>>(parent_dir)) {
                dir->data.insert(d);
                cout << "[SUCCESS] Directory added to B-tree: " << name << endl;
            } else {
                cerr << "[ERROR] Parent directory not found!" << endl;
            }
        }
        
    }

    void CREATEd(const string& name, const string& parent_path) {
        CREATEd(name, parent_path, 3); // Вызов с дефолтным параметром степени B-дерева
    }

    bool REMOVE(const string& name, const string& parent_dir_path) {
        // Нормализация пути родительской директории
        string normalized_parent = normalizePath(parent_dir_path);
        
        // Поиск элемента в родительской директории
        auto elem = FIND(name, normalized_parent);
        if (!elem) {
            cout << "[Error] Element '" << name << "' not found in directory: " << normalized_parent << endl;
            return false;
        }

        try {
            // Удаление файла
            if (auto file = dynamic_pointer_cast<File<Date, string, string, string>>(elem)) {
                fs::remove(file->path1);
                cout << "[Success] File deleted: " << file->path1 << endl;
            } 
            // Удаление директории
            else if (auto dir = dynamic_pointer_cast<Directory<Date, string, string>>(elem)) {
                vector<shared_ptr<FileSystemNode>> children;
                dir->data.getAllElements(children);
                for (auto& child : children) {
                    string child_name;
                    if (auto dir_child = dynamic_pointer_cast<Directory<Date, string, string>>(child)) {
                        child_name = dir_child->dirName1;
                    } else if (auto file_child = dynamic_pointer_cast<File<Date, string, string, string>>(child)) {
                        child_name = file_child->dirName1;
                    }
                    REMOVE(child_name, dir->path1); // Путь корректен: dir->path1 — родительская директория для child
                }
                fs::remove_all(dir->path1);
                cout << "[Success] Directory deleted: " << dir->path1 << endl;
            }

            // Удаление из B-дерева родительской директории
            fs::path parent_path_obj(normalized_parent);
            auto parent_dir = FIND(parent_path_obj.filename().string(), parent_path_obj.parent_path().string());
            if (auto parent = dynamic_pointer_cast<Directory<Date, string, string>>(parent_dir)) {
                parent->data.remove(elem);
                cout << "[Info] Element removed from parent directory." << endl;
            }

            return true;
        } 
        catch (const fs::filesystem_error& e) {
            cerr << "[Critical] Filesystem error: " << e.what() << endl;
            return false;
        }
        catch (const exception& e) {
            cerr << "[Critical] Unknown error: " << e.what() << endl;
            return false;
        }
    }
    

    bool MOVE(const string& elem_name, const string& old_path, const string& new_path) {
        // Нормализация путей
        string norm_old_path = normalizePath(old_path);
        string norm_new_path = normalizePath(new_path);

        // 1. Найти элемент
        auto elem = FIND(elem_name, norm_old_path);
        if (!elem || !directoryExists(norm_new_path)) {
            cout << "[Error] Element or destination invalid" << endl;
            return false;
        }

        // 2. Получить родительские директории
        fs::path parent_old_dir = fs::path(norm_old_path).parent_path();
        fs::path parent_new_dir = fs::path(norm_new_path).parent_path();

        auto parent_old = FIND(parent_old_dir.filename().string(), parent_old_dir.string());
        auto parent_new = FIND(parent_new_dir.filename().string(), parent_new_dir.string());

        if (!parent_old || !parent_new) {
            cout << "[Error] Parent directories not found" << endl;
            return false;
        }

        try {
            // 3. Перемещение
            fs::path new_full_path = fs::path(norm_new_path) / elem_name;
            fs::rename(elem->getPath(), new_full_path);

            // 4. Обновление B-дерева
            if (auto dir_old = dynamic_pointer_cast<Directory<Date, string, string>>(parent_old)) {
                dir_old->data.remove(elem);
            }

            elem->setPath(new_full_path.string());
            if (auto dir = dynamic_pointer_cast<Directory<Date, string, string>>(elem)) {
                updatePaths(dir, dir->getPath());
            }

            if (auto dir_new = dynamic_pointer_cast<Directory<Date, string, string>>(parent_new)) {
                dir_new->data.insert(elem);
            }

            return true;
        } 
        catch (const fs::filesystem_error& e) {
            cerr << "[Error] Move failed: " << e.what() << endl;
            return false;
        }
    }
    

    void WRITE(const string& name, const string& path, const string& s) {
        cout << "Write to: " << (fs::path(path) / name).string() << endl;
        const auto& val = FIND(name, path);
        if (!val) return;

        if (auto file = dynamic_pointer_cast<File<Date, string, string, string>>(val)) {
            file->writeToFile(s);
            cout << "Successfully written to file\n";
            file->changeDate();
            file->print1();
        }
    }
    
    void START(string username, ll t, const fs::path& base_dir) {
        project.dirName1 = username;
        project.path1 = (base_dir / username).lexically_normal().string();
        cout<< "start "<<project.dirName1<< " "<< project.path1<<endl;
        // Создаем корневую директорию физически
        if (!fs::exists(project.path1)) {
            fs::create_directories(project.path1);
            cout<<"success"<<endl;
        }

        // Инициализируем B-дерево
        project.data = btree<ll, FileSystemNode>(t);

        // Явно добавляем корневую директорию в B-дерево
        auto root_dir = make_shared<Directory<Date, string, string>>(project);
        project.data.insert(root_dir);
        project.data.print();

        // Создаём поддиректории внутри корневой директории
        CREATEd("documents1", project.path1, t);
        CREATEd("programfiles1", project.path1, t);
        CREATEd("photos1", project.path1, t);
        CREATEd("books1", project.path1, t);

        // Создаём файлы внутри корневой директории
        CREATEf("readme.txt", project.path1);
        CREATEf("readme2.txt", project.path1);

        // Вложенные элементы
        fs::path docs_path = fs::path(project.path1) / "documents1";
        CREATEd("harry_potter", docs_path.string(), t);
        CREATEf("referat.txt", docs_path.string());
        CREATEf("referat2.txt", docs_path.string());

        fs::path photos_path = fs::path(project.path1) / "photos1";
        CREATEd("anime", photos_path.string(), t);
        CREATEf("elbrus.txt", photos_path.string());
        CREATEf("laba.txt", photos_path.string());
    }
    
};