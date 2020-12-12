#ifndef QDYNAMICAUTHOR_H
#define QDYNAMICAUTHOR_H

#include <QFrame>
#include <author.h>
#include "QListWidget"


class QDynamicAuthor : public QFrame
{
    Q_OBJECT

public:
    ~QDynamicAuthor();
    QDynamicAuthor(QWidget *parent =0,Author* author = 0);
    Author* author;
    void AddGenre(const QString& value);


private:
    QListWidget * lstWgt;


signals:
    void deleteAuthor(int id);

public slots:
    void onDeleteAuthorBtnClicked();

    void onPushBtnClicked();

    void onDeleteGanreBtnClicked();

    void showDeleteBtn(QListWidgetItem *current, QListWidgetItem *previous);

    void onDownloadAuthorBtnClicked();

};

#endif // QDYNAMICAUTHOR_H



