#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace Ui {
class Options;
}

class Options : public QWidget
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();

signals:
    void display(int number);
    void loadSave(int number);
    void setPlayerNum(int number);

private slots:
    void on_single_clicked();

    void on_multi_clicked();

private:
    Ui::Options *ui;
};

#endif // OPTIONS_H
