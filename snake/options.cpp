#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
}

Options::~Options()
{
    delete ui;
}

void Options::on_single_clicked()
{
    QFile file(QApplication::applicationDirPath() + "/single_player.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "File open failed!";
        setPlayerNum(1);
        display(2);
        return;
    }
    qDebug() << "File open successfully!";
    int ret = QMessageBox::warning(0, "Warning", QString("save has been changed!\n Do you want to restore it?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        QJsonParseError *error = new QJsonParseError;
        QJsonDocument jdc = QJsonDocument::fromJson(file.readAll(), error);
        //判断文件是否完整
        if (error->error != QJsonParseError::NoError)
        {
            qDebug() << "parseJson:" << error->errorString();
            setPlayerNum(1);
            display(2);
            return;
        }
        loadSave(0);
    }
    else
    {
            setPlayerNum(1);
    }
    display(2);

}

void Options::on_multi_clicked()
{
    QFile file(QApplication::applicationDirPath() + "/multi_player.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "File open failed!";
        setPlayerNum(2);
        display(2);
        return;
    }
    qDebug() << "File open successfully!";
    int ret = QMessageBox::warning(0, "Warning", QString("save has been changed!\n Do you want to restore it?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        QJsonParseError *error = new QJsonParseError;
        QJsonDocument jdc = QJsonDocument::fromJson(file.readAll(), error);
        //判断文件是否完整
        if (error->error != QJsonParseError::NoError)
        {
            qDebug() << "parseJson:" << error->errorString();
            setPlayerNum(2);
            display(2);
            return;
        }
        loadSave(1);
    }
    else
    {
            setPlayerNum(2);
    }
    display(2);
}
