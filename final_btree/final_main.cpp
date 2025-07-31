#include "btree_final.hpp"

using namespace std;

int main() {
    newproject project;
    fs::path project_dir = "D:\\IT\\1course\\filemanager\\final_btree";//Сюда указыватьнужную папку для старта проекта
    //После инициализации проекта в папке documents1 создается файл test_file.txt
    //После чего в него записывается "Hello, btree!"
    //Потом он перемещается в папку photos1
    //Затем файл удаляется
    //Затем удаляется директория photos1
    //Чтобы проверить выполнение функций закомментируйте нужные части кода, с учетом сказанного выше
    //Естественно никто не мешает немного переписать функцию START в нужный стартовый вариант
    //ВАЖНО: при работе с директорией указывайте полный путь, при работе с файлом указывайте родительскую директорию

    //part1
    // Инициализация проекта
    project.START("testuser", 3, project_dir);
    cout << "=== Project initialized ===" << endl;

    // Создание тестовых файлов и папок
    fs::path user_dir = project_dir / "testuser";
    fs::path docs_dir = user_dir / "documents1";
    fs::path photos_dir = user_dir / "photos1";
    
    project.CREATEd("test_folder", docs_dir.string());
    project.CREATEf("test_file.txt", docs_dir.string());
    
    cout << "\n=== Structure after creation ===" << endl;
    project.project.data.print();


    //part2
    // Тестирование WRITE
    cout << "\n=== Testing WRITE ===" << endl;
    project.WRITE("test_file.txt", docs_dir.string(), "Hello, B-tree!");
    
    // Проверка содержимого файла
    ifstream file_read(docs_dir / "test_file.txt");
    string content;
    getline(file_read, content);
    cout << "File content: " << content << endl;
    file_read.close();


    //part3
    // Тестирование MOVE
    cout << "\n=== Testing MOVE ===" << endl;
    cout << "MOVE from documents1 to photos1" << endl;
    
    bool move_result = project.MOVE(
        "test_file.txt", 
        docs_dir.string(), 
        photos_dir.string()
    );
    
    cout << "Move result: " << (move_result ? "Success" : "Failed") << endl;
    cout << "Structure after move:" << endl;
    project.project.data.print();

    
    //part4
    // Тестирование REMOVE
    cout << "\n=== Testing file REMOVE ===" << endl;
    bool remove_result = project.REMOVE(
        "test_file.txt", 
        photos_dir.string()
    );
    
    cout << "Remove result: " << (remove_result ? "Success" : "Failed") << endl;
    cout << "After remove file structure:" << endl;
    project.project.data.print();

     // Дополнительная проверка существования файлов
    cout << "\n=== File existence check ===" << endl;
    cout << "Original location: " 
         << fs::exists(docs_dir / "test_file.txt") << endl;
    cout << "New location: " 
         << fs::exists(photos_dir / "test_file.txt") << endl;


    //part5
    cout << "\n=== Testing directory REMOVE ===" << endl;
    bool remove_result2 = project.REMOVE(
        "photos1", 
        photos_dir.string()
    );
    
    cout << "Remove result: " << (remove_result2 ? "Success" : "Failed") << endl;
    cout << "Final structure:" << endl;
    project.project.data.print();

    // Дополнительная проверка существования файлов
    cout << "\n=== Directory existence check ===" << endl;
    cout << "Original location: " 
         << fs::exists(user_dir / "photos1") << endl;
    cout << "New location: " 
         << fs::exists(user_dir / "photos1") << endl;
    

    return 0;
}