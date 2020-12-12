#ifndef TEST_H
#define TEST_H

#include "cassert"
#include "iostream"
#include "hashtable.h"


template<class T>
class HashTableTest{
public:
    void StartHashTableTests()
    {
        HashTable<std::string> testHT ;
        DefaultConstructorTest(testHT);
        std::string testString1("test1");
        std::string testString2("test2");
        AddValueTest(testHT,testString1);
        AddValueTest(testHT,testString2);
        FindAddedValueTest(testHT,testString1);
        RemoveTest(testHT,testString1);
        FindRemovedValueTest(testHT,testString1);

        HashTable<std::string> defaultHT ;
        RemoveAllTest(testHT,defaultHT);

        testHT<<testString2;
        InnerAnyAndEmptyTest(testHT,defaultHT);

        HashTable<std::string> copyHT(testHT);
        InnerAnyAndAnyTest(testHT,copyHT);

        CopyConstructorTest(testHT,copyHT);


        std::cout<<"All tests completed"<<std::endl;
    }

    void DefaultConstructorTest( const HashTable<T>  ht){
        assert(ht.GetSize() == 0);
    }

    void AddValueTest(HashTable<T> & ht, const T& value){
        int pastSize = ht.GetSize();
        ht<<value;
        assert(pastSize + 1 == ht.GetSize());
        pastSize = ht.GetSize();
        ht<<value;
        assert(pastSize == ht.GetSize());
    }

    void FindAddedValueTest( HashTable<T>& ht,const T& value){
        assert(ht[value]);
    }

    void RemoveTest( HashTable<T>& ht,const T& value){
        int pastSize = ht.GetSize();
        ht.Remove(value);
        assert(pastSize - 1 == ht.GetSize());
        pastSize = ht.GetSize();
        ht.Remove(value);
        assert(pastSize == ht.GetSize());
    }

    void FindRemovedValueTest( HashTable<T>& ht,const T& value){
        assert(!ht[value]);
    }

    void RemoveAllTest(HashTable<T>& ht,HashTable<T>& defaultHT){
        ht.RemoveAll();
        assert(ht == defaultHT);
    }

    void InnerAnyAndEmptyTest(HashTable<T>& ht,HashTable<T>& defaultHT){
        assert((ht && defaultHT)->GetSize() ==0);
    }

    void InnerAnyAndAnyTest(HashTable<T>& ht,HashTable<T>& copyHT){
        HashTable<std::string> * resHT = ht && copyHT;
        assert((*resHT == ht) && (*resHT == copyHT));
    }

    void CopyConstructorTest(HashTable<T>& ht,HashTable<T>& copyHT){
        assert(ht == copyHT);
        copyHT<<"newString";
        assert(!(ht==copyHT));
        copyHT.Remove("newString");
        assert(ht == copyHT);
    }

    void ReadAndWriteFileTests(HashTable<T>& ht){
        std::string path = "/C:/Qt/Implementing-template-hash-table-master/output.txt";
        ht.WriteFile(path);
        HashTable<std::string> resHT;
        resHT.GetHashTableFromFile(path);
        assert(resHT == ht);
    }
};


#endif // TEST_H
