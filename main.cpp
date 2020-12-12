#include "mainwindow.h"
#include "iostream"
#include "hashtable.h"
#include "test.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    HashTableTest<std::string> test ;
    test.StartHashTableTests();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
