#include "router.h"
#include "ui_router.h"

Router::Router(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Router)
{
    ui->setupUi(this);
    home=new Home;
    widget=new Widget;
    options=new Options;
    setFixedSize(900,600);
    setWindowTitle("snake");
    stackLayout = new QStackedLayout;
    stackLayout->addWidget(home);
    stackLayout->addWidget(options);
    stackLayout->addWidget(widget);
    connect(home, &Home::display, stackLayout, &QStackedLayout::setCurrentIndex);
    connect(options,&Options::display,stackLayout,&QStackedLayout::setCurrentIndex);
    connect(widget, &Widget::display, stackLayout, &QStackedLayout::setCurrentIndex);
    connect(options,&Options::loadSave,widget,&Widget::loadGame);
    connect(options,&Options::setPlayerNum,widget,&Widget::setPlayerNum);

    this->setLayout(stackLayout);
}

Router::~Router()
{
    delete ui;
}

void Router::closeEvent(QCloseEvent *event){
    if(stackLayout->currentIndex()!=2){
        qApp->quit();
        return;
    }
    int ret = QMessageBox::warning(0,"Warning",QString("%1 has been changed!\n Do you want to save it?").arg("snake"),QMessageBox::Yes |QMessageBox::No|QMessageBox::Cancel,QMessageBox::No);
    if(ret == QMessageBox::Yes) {
        qDebug()<<"save game";
        widget->saveGame();
    }
    else if(ret == QMessageBox::Cancel){
        event->ignore();
    }
    else{
//        if(QDir().exists("./single_player.json")){
//            qDebug() << "delete save";
//            QFile::remove("./single_player.json");
//        }
        qApp->quit();
    }
}
