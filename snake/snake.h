#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
#include <QKeyEvent>
#include <QRectF>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <string>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace Ui {
class Snake;
}

class Snake : public QWidget
{
    Q_OBJECT

public:
    explicit Snake(QWidget *parent = nullptr);
    ~Snake();

    void saveGame();
    bool loadGame();

signals:
    void display(int number);

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

protected slots:
    void timeOut();
    void rewardTimeOut();

private slots:
    void on_pause_clicked();

    void on_restart_clicked();

private:
    void addTopRectF();
    void addDownRectF();
    void addLeftRectF();
    void addRightRectF();
    void deleteLastRectF();
    bool snakeStrike();
    void restoreSave(QJsonObject obj);

    struct RewardNode{
        QRectF coordinate;
        int nodeType;

        RewardNode(){}
        RewardNode(QRectF c,int type=0){
            nodeType=type;
            coordinate=c;
        }
    };

    struct {

    };

    enum Move{Left=0,Right,Up,Down};

    QTimer *timer;
    QTimer *rewardTimer;
    Ui::Snake *ui;

    bool status=false;
    bool gameOver = false;
    bool gameStart = false;
    bool single=true;
    int life=1;
    int moveFlage=Up;

    int snakeNodeWidth = 20;
    int snakeNodeHeight = 20;
    int time = 100;

    QList<QRectF> snake;
    QList<RewardNode>rewardNode;
    QList<QRectF>barrier;
};

#endif // SNAKE_H
