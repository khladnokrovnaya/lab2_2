#include "author.h"
#include "iostream"

int Author::countAuthor = 0;

Author::Author(const std::string& name)
{
    countAuthor++;
    id = countAuthor;
    this->name = name;
    genres = new HashTable<std::string>();
}


Author::Author(const std::string& name, const HashTable<std::string>* ht){
    countAuthor++;
    id = countAuthor;
    this->name = name;
    genres = new HashTable<std::string>(*ht);
}

Author::~Author()
{
    delete genres;
}
