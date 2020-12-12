#ifndef HASHTABLE_H
#define HASHTABLE_H


#include "vector"
#include "iostream"
#include <fstream>



template <class T>
class HashTable
{
    static const int default_size = 8; // начальный размер нашей таблицы
    constexpr static const double rehash_size = 0.75; // коэффициент, при котором произойдет увеличение таблицы
    struct Node
    {
        T value;
        bool state; // если значение флага state = false, значит элемент массива был удален (deleted)
        Node(const T& value_) : value(value_), state(true) {}
    };
    std::vector<Node*> arr; // соответственно в массиве будут хранится структуры Node*
    int size; // сколько элементов у нас сейчас в массиве (без учета deleted)
    int size_all_non_nullptr; // сколько элементов у нас сейчас в массиве (с учетом deleted)

    int HashFunction( const std::string& s, int table_size, const int key) const;

public:
    HashTable(int buf_size = default_size )
    {
        size = 0;
        size_all_non_nullptr = 0;
        arr.resize(buf_size);
        for (int i = 0; i < arr.size(); ++i)
            arr[i] = nullptr; // заполняем nullptr - то есть если значение отсутствует, и никто раньше по этому адресу не обращался
    }

    HashTable(const HashTable<T>& hashTable)
    {
        size = 0;
        size_all_non_nullptr = 0;
        arr.resize(hashTable.arr.size());
        for (int i = 0; i < hashTable.arr.size(); ++i)
            if (hashTable.arr[i] && hashTable.arr[i]->state)
                *this << hashTable.arr[i]->value; // добавляем элементы в новый массив
    }

    ~HashTable()
    {
        for (int i = 0; i < arr.size(); ++i)
            if (arr[i])
                delete arr[i];
        arr.clear();
    }

    bool GetHashTableFromFile(const std::string & fileName)
    {
        if(size!=0)
            RemoveAll();

        std::ifstream file(fileName);
        std::string buff;
        if (file.is_open())
        {
            while (!file.eof())
            {
                std::getline(file, buff);
                if(!(buff==""))            
                    *this << buff;
            }
            file.close();
        }
        else
            return false;
        return true;
    }

    bool WriteFile(const std::string &fileName) const
    {
        std::ofstream file;
        file.open(fileName);
        if(file.is_open()){
            for (int i = 0; i < arr.size(); ++i){
                if (arr[i] && arr[i]->state)
                    file << arr[i]->value << std::endl;
            }
            file.close();
        }
        else{
            return false;
        }
        return true;

    }

    int GetSize() const
    {
        return size;
    }

    int GetBufferSize() const
    {
        return arr.size();
    }

    const T GetValueByKey(int key) const{
       if(arr[key] && arr[key]->state)
           return arr[key]->value;
       return T();
    }

    void Resize()
    {
    int past_buffer_size = arr.size();
        size_all_non_nullptr = 0;
        size = 0;
        std::vector<Node*> arr2;
        arr2.resize(arr.size() *2);
        for (int i = 0; i < arr.size(); ++i)
            arr2[i] = nullptr;
        std::swap(arr, arr2);
        for (int i = 0; i < past_buffer_size; ++i)
        {
            if (arr2[i] && arr2[i]->state)
                *this << arr2[i]->value;  // добавляем элементы в новый массив
        }
        // удаление предыдущего массива
        for (int i = 0; i < past_buffer_size; ++i)
            if (arr2[i])
                delete arr2[i];
        arr2.clear();
    }

    void Rehash()
    {
        size_all_non_nullptr = 0;
        size = 0;
        std::vector<Node*> arr2;
        arr2.resize(arr.size());
        for (int i = 0; i < arr.size(); ++i)
            arr2[i] = nullptr;
        std::swap(arr, arr2);
        for (int i = 0; i < arr.size(); ++i)
        {
            if (arr2[i] && arr2[i]->state)
                *this << arr2[i]->value;
        }
        // удаление предыдущего массива
        for (int i = 0; i < arr.size(); ++i)
            if (arr2[i])
                delete arr2[i];
        arr2.clear();
    }

    bool Remove(const T& value)
    {
        int h1 = HashFunction(value, arr.size(),arr.size()-1);
        int h2 = HashFunction(value, arr.size(),arr.size()+1);
        int i = 0;
        while (arr[h1] != nullptr && i < arr.size())
        {
            if (arr[h1]->value == value && arr[h1]->state)
            {
                arr[h1]->state = false;
                --size;
                return true;
            }
            h1 = (h1 + h2) % arr.size();
            ++i;
        }
        return false;
    }

    void RemoveAll()
    {
        arr.resize(default_size);
        for (int i = 0; i < arr.size(); ++i)
            arr[i] = nullptr;
        size = 0;
        size_all_non_nullptr = 0;
    }

    friend bool operator == (const HashTable<T> &ht1, const HashTable<T> &ht2)
    {
        if(ht1.size != ht2.size)
            return false;

        HashTable<T> minHashTable = ht1;
        HashTable<T> maxHashTable = ht2;
        for (int i = 0; i < minHashTable.arr.size(); ++i)
        {
            if (minHashTable.arr[i] && !maxHashTable[minHashTable.arr[i]->value])
                return false;
        }
        return true;
    }

    bool operator [] (const T& value) const
    {
        int h1 = HashFunction(value, arr.size(),arr.size()-1);  // значение, отвечающее за начальную позицию
        int h2 = HashFunction(value, arr.size(),arr.size()+1); // значение, ответственное за "шаг" по таблице
        int i = 0;
        while (arr[h1] != nullptr && i < arr.size())
        {
            if (arr[h1]->value == value && arr[h1]->state)
                return true; // такой элемент есть
            h1 = (h1 + h2) % arr.size();
            ++i; // если у нас i >=  buffer_size, значит мы уже обошли абсолютно все ячейки, именно для этого мы считаем i, иначе мы могли бы зациклиться.
        }
        return false;
    }

    friend bool operator<<(HashTable<T>& ht,const T& value)
    {
        if (ht.size + 1 > int(rehash_size * ht.arr.size()))
            ht.Resize();
        else if (ht.size_all_non_nullptr > 2 * ht.size)
            ht.Rehash(); // происходит рехеш, так как слишком много deleted-элементов
        int h1 = ht.HashFunction(value, ht.arr.size(),ht.arr.size()-1);  // значение, отвечающее за начальную позицию
        int h2 = ht.HashFunction(value, ht.arr.size(),ht.arr.size()+1); // значение, ответственное за "шаг" по таблице
        int i = 0;
        int first_deleted = -1; // запоминаем первый подходящий (удаленный) элемент
        while (ht.arr[h1] != nullptr && i < ht.arr.size())
        {
            if (ht.arr[h1]->value == value && ht.arr[h1]->state)
                return false; // такой элемент уже есть, а значит его нельзя вставлять повторно
            if (!(ht.arr[h1]->state) && first_deleted == -1) // находим место для нового элемента
                first_deleted = h1;
            h1 = (h1 + h2) % ht.arr.size();
            ++i;
        }
        if (first_deleted == -1) // если не нашлось подходящего места, создаем новый Node
        {
            ht.arr[h1] = new Node(value);
            ++(ht.size_all_non_nullptr); // так как мы заполнили один пробел, не забываем записать, что это место теперь занято
        }
        else
        {
            ht.arr[first_deleted]->value = value;
            ht.arr[first_deleted]->state = true;
        }
        ++(ht.size); // и в любом случае мы увеличили количество элементов
        return true;
    }

    friend HashTable<T>* operator &&(const HashTable<T> &ht1, const HashTable<T> &ht2)
    {
        HashTable<T> minHashTable = ht1;
        HashTable<T> maxHashTable = ht2;
        if(ht1.arr.size() > ht2.arr.size()){
            maxHashTable = ht1;
            minHashTable = ht2;
        }

        HashTable<T>* resHashTable = new HashTable<T>(minHashTable.arr.size());
        for (int i = 0; i < minHashTable.arr.size(); ++i)
        {
            if (minHashTable.arr[i] && maxHashTable[minHashTable.arr[i]->value])
                *resHashTable << ht2.arr[i]->value;
        }
        return resHashTable;
    }
};


template <class T>
int HashTable<T>::HashFunction(const std::string& s, int table_size, const int key) const
{
    int hash_result = 0;
    for (int i = 0; i != s.length(); ++i){
        hash_result = (key * hash_result + s[i]) % table_size;
    }

    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}




#endif // HASHTABLE_H
