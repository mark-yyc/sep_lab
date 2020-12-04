#ifndef ROUTER_H
#define ROUTER_H

#include <QWidget>
#include <QStackedLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QFile>
#include <QDir>

#include "home.h"
#include "options.h"
#include "widget.h"

namespace Ui {
class Router;
}

class Router : public QWidget
{
    Q_OBJECT

public:
    explicit Router(QWidget *parent = nullptr);
    ~Router();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::Router *ui;
    Home *home;
    Widget *widget;
    Options *options;
    QStackedLayout *stackLayout;
};

#endif // ROUTER_H
