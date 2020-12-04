#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QRectF>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <iostream>
#include <string>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMenu>


using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void saveGame();
    bool loadGame(int number);
    void setPlayerNum(int number);
    ~Widget();

signals:
    void display(int number);

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *ev);

protected slots:
    void rewardTimeOut();
    void buffTimeOut();
    void timeOut();
    void setEditMenu();

private slots:
    void on_pause_clicked();
    void on_restart_clicked();

private:
    void initGame();
    void snakeTimeout(int index);
    void addTopRectF(int index);
    void addDownRectF(int index);
    void addLeftRectF(int index);
    void addRightRectF(int index);
    void deleteLastRectF(int index);
    bool snakeStrike();
    void restoreSave(QJsonObject obj);

    enum Move{Left=0,Right,Up,Down};
    struct RewardNode{
        QRectF coordinate;
        int nodeType;

        RewardNode(){}
        RewardNode(QRectF c,int type=0){
            nodeType=type;
            coordinate=c;
        }
    };

    struct Snake{
        QList<QRectF> snakeNode;
        int life;
        int moveFlage;
        int nextFlage;
        int time;
        bool buff;
        QTimer *snakeTimer;
        QTimer *buffTimer;

        Snake(int l=1,int m=Up,int t=100,bool b=false){
            life=l;
            moveFlage=m;
            nextFlage=m;
            time=t;
            buff=b;
            snakeTimer=new QTimer;
            buffTimer=new QTimer;
        }
    };

    QTimer *paintTimer;
    QTimer *rewardTimer;
    Ui::Widget *ui;

    int player_num=1;
    int operation=-1;
    bool status=false;
    bool gameStart = true;
    bool gameOver=false;
    bool load=false;

    int snakeNodeWidth = 20;
    int snakeNodeHeight = 20;
    int rewardTime=100;

    QList<Snake> snake;
    QList<RewardNode> rewardNode;
    QList<QRectF>barrier;
};
#endif // WIDGET_H
