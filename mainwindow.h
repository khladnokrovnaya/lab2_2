#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vector"
#include "author.h"
#include "QMessageBox"
#include <QFileDialog>
#include "QHBoxLayout"
#include "qdynamicauthor.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void AddAuthor(Author * author);

private slots:
    void on_pushButton_clicked();

    void on_loadFileBtn_clicked();

    void on_pushButton_3_clicked();

    void on_createCollectionBtn_clicked();

    void onDestroyedAuthor(int id);

private:
    Ui::MainWindow *ui;
    QHBoxLayout* scrollLayout;
    std::vector<Author*> authors;


};
#endif // MAINWINDOW_H
