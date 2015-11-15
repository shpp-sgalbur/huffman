#include <iostream>
#include "haffmancompres.h"
#include "console.h"
#include <vector>
#include <map>
#include <list>
#include "vector.h"
#include "map.h"
#include <cctype>

#include <fstream>

using namespace std;

struct Node1
   {
       char ch;
       int parent;
       int zero;
       int one;
       bool branch;
   };
struct Node{
    char ch;
    Node * child_0;
    Node * child_1;
    int frequency;
};
void print(Vector <Node*> vec){
    /*Проверяем что в векторе hafmanTree */
    cout << "-------------hafmanTree------------" <<vec.size()<< endl;
    for(int i = 0; i < vec.size(); i++){
        cout << i+1 <<")"<< "frequency - "  << vec[i]->frequency << " symbol - " << vec[i]->ch << endl;
        cout  << "  child_0->"<< vec[i]->child_0 << "  " << "child_1->" << vec[i]->child_1 << endl;
    }
}
/*Карта пар символ - частота*/
map <char, int> creatMapSymbols(string filePath){

    //инициализируем массив частот символов нулями
    int frequency [256];
    for (int i = 0; i < 256; i++)
            frequency[i] = 0;

    //открываем файловый поток
    ifstream f (filePath, ifstream::in);
    if (!f.is_open()) // если файл не открыт
          cout << "The file cannot be opened!\n";

    while (!f.eof()){

        //считываем 1 байт
        unsigned char ch;
        f.read((char *)&ch, sizeof(ch));
        ++frequency[ch];
    }

    //инициализируем карту частот символов
    map <char,int> frequencyMap ;
    for (int i = 0; i < 256; i++){
        if (frequency[i]>0 ){
            frequencyMap[(char)i] = frequency[i];
        }


    }

    //выводим карту на экран
    map <char, int>::iterator itr;
    for (itr = frequencyMap.begin(); itr != frequencyMap.end(); itr++){
        cout << itr->first-0 << " : " << itr->second << endl;
    }
    cout << " ---------------------------" <<  endl;
    f.close();


    return frequencyMap;
}
/* Функция построения дерева хафмана из карты частот символов
 * -----------------------------------
 */
Vector <Node*> buildTree(map <char, int> frequencyMap){
    Vector <Node*> hafmanTree;
    multimap<int /*weight*/, Node /* index in the tree */> sortedMap;

    /*переносим данные из frequencyMap в sortedMap вместо сортировки
      ключ (first) -частота, значение (second) - символ.
    */
    map  <char, int>::iterator itr;
    for (itr = frequencyMap.begin(); itr != frequencyMap.end(); itr++){
        Node  *node = new Node;
        node->ch = itr->first;
        node->frequency = itr->second;
        node->child_0 = node->child_1 = NULL;
        sortedMap.emplace ( node->frequency , *node );
        hafmanTree.add(node);
    }


    /*добавляем узел-лист в дерево, пока не связанным ни с одним другим узлом*

    multimap  <int, char>::iterator itr2;
    for (itr2 = sortedMap.begin(); itr2 != sortedMap.end(); itr2++){
        Node  *node = new Node;
        cout << itr2->first-0 << " : " << itr2->second << endl;
        node->ch = itr2->second;
        node->child_0 = node->child_1 = NULL;
        node->frequency =itr2->first;
        hafmanTree.add(node);
    }*/





    /*Начинаем строить дерево*/
    while (sortedMap.size() > 1){

        //вытаскиваем из нее два элемента с меньшей частотой, сохраняя значение байта и его частоты в переменных

        // вытаскивая два элемента карты
        //создаем над извлеченными элементами родителя (с указателями на них как на потомков)
        Node * parent = new Node;
        parent->child_0= &begin(sortedMap)->second;
        sortedMap.erase(begin(sortedMap));
        parent->child_1 = &begin(sortedMap)->second;
        sortedMap.erase(begin(sortedMap));
        parent->frequency = parent->child_0->frequency + parent->child_1->frequency;

        sortedMap.emplace ( parent->frequency , *parent );



        hafmanTree.add(parent);




    }

    print (hafmanTree);//del
    return  hafmanTree;

    }

/*функция создания карты кодирования*/
map<char, string> buildCodeMap(Node *root){
    map<char, string> codeMap;
    string codeSymbol = "";
    if(root->child_0 != NULL){
        codeSymbol += "0";
        buildCodeMap(root->child_0);
    }
    if(root->child_1 != NULL){
        codeSymbol += "1";
        buildCodeMap(root->child_1);
    }
    if(root->child_0 == NULL && root->child_1 == NULL){
        codeMap.emplace(root->ch,codeSymbol);

    }
    return codeMap;

}

int main()
{
    /*ofstream fout("./cppstudio.txt"); // создаём объект класса ofstream для записи и связываем его с файлом cppstudio.txt
       fout << "Работа с файлами в С++"; // запись строки в файл
       fout.close(); // закрываем файл
       system("pause");

    /**********************
    std::ifstream ifs ("test.txt", ifstream::binary);

    if (!ifs.is_open()) // если файл не открыт
          cout << "The file cannot be opened!\n";

      // get pointer to associated buffer object
      filebuf* pbuf = ifs.rdbuf();

      // get file size using buffer's members
      size_t size = pbuf->pubseekoff (0,ifs.end,ifs.in);
      pbuf->pubseekpos (0,ifs.in);

      // allocate memory to contain file data
      char* buffer=new char[size];

      // get file data
      pbuf->sgetn (buffer,size);

      ifs.close();

      // write content to stdout
      cout.write (buffer,size);

      delete[] buffer;
    //**********************/


    map <char, int>  mapSymbol;//символ и его частота
    string filePath ="test.txt";
    mapSymbol  = creatMapSymbols(filePath);


    Vector <Node*> tree;
    tree = buildTree(mapSymbol);
    //cout << tree.begin()<< "____" <<endl;
    //tree.add( tree );
    cout << "in mane" <<endl;
     print (tree);//del
    for(int i; i< tree.size();i++){
        cout << "Freqancy " << tree[i]->frequency << endl;
    }
    Node * root = tree[tree.size()-1];
    cout << "root" << endl;
    cout << root->frequency <<endl;
    map <char, string> codeMap = buildCodeMap(tree[tree.size()-1]);

    cout << "++++++++codeMap builde++++++++++"<< endl;








       /*вывод карты символов*
       for (itr = mapSymbol.begin(); itr != mapSymbol.end(); itr++){
           cout << itr->first << " : " << itr->second << endl;
       }*/






    return 0;
}

