#include "qdynamicauthor.h"
#include "QFrame"
#include "QVBoxLayout"
#include "QLabel"
#include "QGridLayout"
#include "QSizePolicy"
#include "QScrollArea"
#include "QLineEdit"
#include "QPushButton"
#include "QListWidgetItem"
#include "QMessageBox"
#include <QFileDialog>
#include "iostream"


QDynamicAuthor::QDynamicAuthor(QWidget *parent, Author* author):
    QFrame(parent),
    author(author)
{
    this->setFrameShape(Shape::Box);
    this->setFixedSize(240, 500);

    QLayout* l = new QVBoxLayout;

    QWidget* titleW = new QWidget;
    QLayout* titleLayout = new QVBoxLayout;
    titleLayout->setContentsMargins(0,0,0,0);

    QWidget* titleNameWgt = new QWidget;
    QLayout* titleLayout1 = new QHBoxLayout;
    titleLayout1->setContentsMargins(0,0,0,0);
    QLabel * titleName = new QLabel();
    titleName->setText(QString::fromStdString(author->name + "  №") + QString::number(author->id));
    titleLayout1->addWidget(titleName);
    QPushButton* deleteAuthorBtn = new QPushButton( "❌");
    deleteAuthorBtn->setFixedWidth(25);
    connect( deleteAuthorBtn, SIGNAL( clicked() ), SLOT( onDeleteAuthorBtnClicked() ) );
    titleLayout1->addWidget(deleteAuthorBtn);
    titleNameWgt->setLayout(titleLayout1);
    titleLayout->addWidget(titleNameWgt);

    QWidget* titleGanreWgt = new QWidget;
    QLayout* titleLayout2 = new QHBoxLayout;
    titleLayout2->setContentsMargins(0,0,0,0);
    QLabel * titleGanre = new QLabel();
    titleGanre->setText(QString("Жанры:"));
    titleLayout2->addWidget(titleGanre);
    QPushButton* downloadAuthorBtn = new QPushButton( QString::fromUtf8("⬇") );
    downloadAuthorBtn->setFixedWidth(25);
    connect(downloadAuthorBtn, SIGNAL( clicked() ), SLOT( onDownloadAuthorBtnClicked() ) );
    titleLayout2->addWidget(downloadAuthorBtn);
    titleGanreWgt->setLayout(titleLayout2);
    titleLayout->addWidget(titleGanreWgt);

    titleW->setLayout(titleLayout);

    QListWidget* listWgt =new QListWidget;
    lstWgt = listWgt;
    lstWgt->setContentsMargins(0,5,0,5);
    connect(lstWgt, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            SLOT(showDeleteBtn(QListWidgetItem *, QListWidgetItem *)));
    QWidget* pushWgt = new QWidget;
    QLayout* l2 = new QHBoxLayout;
    l2->setContentsMargins(0,5,0,5);
    QLineEdit * input = new QLineEdit();
    l2->addWidget( input );
    QPushButton* pushBtn= new QPushButton( "Добавить" );
    pushBtn->setFixedWidth(75);
    connect( pushBtn, SIGNAL( clicked() ), SLOT( onPushBtnClicked() ) );
    l2->addWidget( pushBtn );
    pushWgt->setLayout( l2 );

    l->addWidget(titleW);
    l->addWidget(lstWgt);
    l->addWidget(pushWgt);

    setLayout( l );

    if(author->genres->GetSize()>0)
    {
        for (int i = 0; i < author->genres->GetBufferSize(); ++i)
        {
                std::string  value =  author->genres->GetValueByKey(i);
                if(value != std::string())
                    AddGenre(QString::fromStdString(value));
        }
    }
}

void QDynamicAuthor::onDeleteAuthorBtnClicked()
{

    emit deleteAuthor(this->author->id);
    delete this;
}

void QDynamicAuthor::AddGenre(const QString& value)
{
    QListWidgetItem* item = new QListWidgetItem( lstWgt );

    QWidget* wgt = new QWidget;
    QLayout* l = new QHBoxLayout;
    QLabel * genre = new QLabel(value);
    genre->setWordWrap(true);
    l->addWidget(genre );
    QPushButton* deleteGanreBtn = new QPushButton( "❌" );
    deleteGanreBtn->setFixedWidth(25);
    deleteGanreBtn->setFixedHeight(25);
    deleteGanreBtn->hide();
    connect( deleteGanreBtn, SIGNAL( clicked() ), SLOT( onDeleteGanreBtnClicked() ) );

    l->addWidget( deleteGanreBtn );
    wgt->setLayout( l );
    item->setSizeHint( wgt->sizeHint() );
    lstWgt->setItemWidget( item, wgt );
}

void QDynamicAuthor::onPushBtnClicked()
{
    if( QPushButton* btn = qobject_cast< QPushButton* >(sender()) )
    {
        QLineEdit* e = btn->parent()->findChild< QLineEdit* >();
        if( e && e->text()!=""  ) {
            bool result = *(this->author->genres) <<e->text().toStdString() ;
            if(result){
                AddGenre(e->text());
                e->setText("");
            }
            else
                QMessageBox::information( this, "", QString("Такой жанр уже существует") );
        }
    }
}

void QDynamicAuthor::showDeleteBtn(QListWidgetItem * itemCur, QListWidgetItem * itemPr )
{
    QWidget *widget1 = qobject_cast<QWidget*>(lstWgt->itemWidget(itemCur));
    QPushButton *btnCur = widget1->findChild<QPushButton*>();
    btnCur->show();

    if(QWidget *widget2 = qobject_cast<QWidget*>(lstWgt->itemWidget(itemPr))){
        QPushButton *btnPr = widget2->findChild<QPushButton*>();
        btnPr->hide();
    }
}

void QDynamicAuthor::onDeleteGanreBtnClicked()
{
    if(lstWgt->count()>1)
    {
        QListWidgetItem* item = lstWgt->currentItem();
        QWidget *widget = qobject_cast<QWidget*>(lstWgt->itemWidget(item));
        QLabel* genre = widget->findChild<QLabel*>();
        author->genres->Remove(genre->text().toStdString());
        delete lstWgt->takeItem(lstWgt->currentRow());
    }
    else{
       QMessageBox::information( this, "",QString("В коллекции должен содержаться как минимум один жанр"));
    }
}

void QDynamicAuthor::onDownloadAuthorBtnClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                QString::fromUtf8("Открыть файл"),
                                QDir::currentPath(),
                                "TXT File(*.txt)");
    if(!(fileName ==""))
    {
        if(!this->author->genres->WriteFile(fileName.toStdString()))
            QMessageBox::information( this, "",QString("Не удалось загрузить колекцию в файл"));
    }
}

QDynamicAuthor::~QDynamicAuthor()
{
    delete author;
    delete lstWgt;
}



