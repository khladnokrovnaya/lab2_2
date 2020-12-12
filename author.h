#ifndef AUTHOR_H
#define AUTHOR_H

#include "iostream"
#include "hashtable.h"


class Author
{

private:
    static int countAuthor;

public:
    Author(const std::string& name);
    Author(const std::string& name, const HashTable<std::string>* ht);
    ~Author();
    int id;
    std::string name;
    HashTable<std::string> *genres;

};

#endif // AUTHOR_H
