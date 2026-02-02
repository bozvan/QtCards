#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QKeyEvent>
#include <iostream>

MainWindow::MainWindow(DatabaseManager &db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , count(0)
{
    ui->setupUi(this);
    btn = new QPushButton("My button", this);
    btn->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));

    //connect(btn, &QPushButton::clicked, this, &MainWindow::handleButton);
    connect(btn, &QPushButton::clicked, this, [this, &db](){
        db.insertDeck("test");
        qDebug() << "Insert test data line!";
    });
    btn->installEventFilter(this);
    //connect(btn, SIGNAL(clicked()), SLOT(handleButton()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == (QObject*)btn) {
        if (event->type() == QEvent::Enter)
        {
            std::cout << "aga" << std::endl;
            btn->setText("aga");
            return true;
        }
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Space)
            {
                std::cout << "space" << std::endl;
                btn->setText("space");
                return true;
            }
        }
        return QWidget::eventFilter(obj, event);
    }else {
        return QWidget::eventFilter(obj, event);
    }
}

void MainWindow::handleButton()
{
    btn->setText(QString::number(count++));
    std::cout << btn->text().toStdString() << std::endl;
}
