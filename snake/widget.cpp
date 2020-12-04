#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    rewardTimer = new QTimer;
    connect(rewardTimer, SIGNAL(timeout()), this, SLOT(rewardTimeOut()));
    paintTimer=new QTimer;
    connect(paintTimer,&QTimer::timeout,this,QOverload<>::of(&Widget::update));

    QMenu *edit_menu=new QMenu;

    QAction *delete_action=new QAction;
    QAction *barrier=new QAction;
    QAction *food_ordinary=new QAction;
    QAction *food_life=new QAction;
    QAction *food_acc=new QAction;
    QAction *food_de=new QAction;
    QAction *food_en=new QAction;

    delete_action->setText("删除");
    barrier->setText("墙壁");
    food_ordinary->setText("食物");
    food_life->setText("食物-加血");
    food_acc->setText("食物-加速");
    food_de->setText("食物-减速");
    food_en->setText("食物-无敌");

    delete_action->setProperty("edit",0);
    delete_action->setProperty("edit_title","删除");
    barrier->setProperty("edit",1);
    barrier->setProperty("edit_title","墙壁");
    food_ordinary->setProperty("edit",2);
    food_ordinary->setProperty("edit_title","食物");
    food_life->setProperty("edit",3);
    food_life->setProperty("edit_title","食物-加血");
    food_acc->setProperty("edit",4);
    food_acc->setProperty("edit_title","食物-加速");
    food_de->setProperty("edit",5);
    food_de->setProperty("edit_title","食物-减速");
    food_en->setProperty("edit",6);
    food_en->setProperty("edit_title","食物-无敌");


    connect(delete_action, SIGNAL(triggered()), this, SLOT(setEditMenu()));
    connect(barrier, SIGNAL(triggered()), this, SLOT(setEditMenu()));
    connect(food_ordinary, SIGNAL(triggered()), this, SLOT(setEditMenu()));
    connect(food_life, SIGNAL(triggered()), this, SLOT(setEditMenu()));
    connect(food_acc, SIGNAL(triggered()), this, SLOT(setEditMenu()));
    connect(food_de, SIGNAL(triggered()), this, SLOT(setEditMenu()));
    connect(food_en, SIGNAL(triggered()), this, SLOT(setEditMenu()));

    edit_menu->addAction(delete_action);
    edit_menu->addAction(barrier);
    edit_menu->addAction(food_ordinary);
    edit_menu->addAction(food_life);
    edit_menu->addAction(food_acc);
    edit_menu->addAction(food_de);
    edit_menu->addAction(food_en);

    ui->edit->setMenu(edit_menu);
}

void Widget::setEditMenu(){
    operation=sender()->property("edit").toInt();
//    qDebug()<<operation;
    ui->edit->setText(sender()->property("edit_title").toString());
}

void Widget::setPlayerNum(int number)
{
    player_num = number;
    for (int i = 0; i < player_num; i++)
    {
        Snake player;
        player.snakeTimer->setProperty("id", i);
        player.buffTimer->setProperty("id", i);
        connect(player.snakeTimer, SIGNAL(timeout()), this, SLOT(timeOut()));
        connect(player.buffTimer, SIGNAL(timeout()), this, SLOT(buffTimeOut()));
        snake.push_back(player);
    }
}

void Widget::buffTimeOut()
{
    snake[sender()->property("id").toInt()].buff = false;
    snake[sender()->property("id").toInt()].buffTimer->stop();
}

void Widget::snakeTimeout(int index)
{
    int flage = 1;
    for (int i = 0; i < rewardNode.length(); i++)
    {
        if (snake.at(index).snakeNode.at(0).contains(rewardNode.at(i).coordinate.x() + rewardNode.at(i).coordinate.width() / 2,
                                                     rewardNode.at(i).coordinate.y() + rewardNode.at(i).coordinate.height() / 2))
        {
            if (rewardNode.at(i).nodeType == 1)
            {
                snake[index].life++;
            }
            if (rewardNode.at(i).nodeType == 2)
            {
                int time = snake[index].time - 20;
                if (time <= 20)
                {
                    time = 20;
                }
                snake[index].snakeTimer->start(time);
            }
            if (rewardNode.at(i).nodeType == 3)
            {
                int time = snake[index].time + 20;
                if (time >= 200)
                {
                    time = 200;
                }
                snake[index].snakeTimer->start(time);
            }
            if (rewardNode.at(i).nodeType == 4)
            {
                snake[index].buff = true;
                snake[index].buffTimer->start(snake[index].time * 50);
            }
            flage++;
            rewardNode.removeAt(i);
            break;
        }
    }
    while (flage--)
    {
        switch (snake.at(index).nextFlage)
        {
        case Up:
            if(snake.at(index).moveFlage!=Down){
                snake[index].moveFlage=Up;
                            addTopRectF(index);
            }
            break;
        case Down:
            if(snake.at(index).moveFlage!=Up){
                snake[index].moveFlage=Down;
                            addDownRectF(index);
            }
            break;
        case Right:
            if(snake.at(index).moveFlage!=Left){
                snake[index].moveFlage=Right;
                                        addRightRectF(index);
            }

            break;
        case Left:
            if(snake.at(index).moveFlage!=Right){
                snake[index].moveFlage=Left;
                            addLeftRectF(index);
            }

            break;
        default:
            break;
        }
    }
    deleteLastRectF(index);

//    update();
}
void Widget::timeOut()
{
    snakeTimeout(sender()->property("id").toInt());
}

void Widget::rewardTimeOut()
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    if (rewardNode.length() >= 5)
    {
        rewardNode.removeAt(qrand() % 5);
    }
    int x = qrand() % (this->width() / 20) * 20;
    if (x <= 120)
    {
        x += 120;
    }
    int y = qrand() % (this->height() / 20) * 20;
    QRectF coordinate(x, y, snakeNodeWidth, snakeNodeWidth);
    int key = qrand() % 10;
    if (key == 0)
    {
        RewardNode new_reward(coordinate, 1);
        rewardNode.append(new_reward);
    }
    else if (key == 1)
    {
        RewardNode new_reward(coordinate, 2);
        rewardNode.append(new_reward);
    }
    else if (key == 2)
    {
        RewardNode new_reward(coordinate, 3);
        rewardNode.append(new_reward);
    }
    else if (key == 3)
    {
        RewardNode new_reward(coordinate, 4);
        rewardNode.append(new_reward);
    }
    else if (key == 4 || key == 5)
    {
        barrier.append(coordinate);
    }
    else
    {
        RewardNode new_reward(coordinate, 0);
        rewardNode.append(new_reward);
    }
}

void Widget::addTopRectF(int index)
{
    if (snake.at(index).snakeNode.at(0).y() - snakeNodeHeight < 0)
    {
        snake[index].snakeNode.insert(0, QRectF(QPointF(snake.at(index).snakeNode.at(0).x(), this->height() - snakeNodeHeight),
                                                QPointF(snake.at(index).snakeNode.at(0).x() + snakeNodeWidth, this->height())));
    }
    else
    {
        snake[index].snakeNode.insert(0, QRectF(snake.at(index).snakeNode.at(0).topLeft() + QPointF(0, -snakeNodeHeight), snake.at(index).snakeNode.at(0).topRight()));
    }
}
//向下移动
void Widget::addDownRectF(int index)
{
    if (snake.at(index).snakeNode.at(0).y() + snakeNodeHeight * 2 > this->height())
    {
        snake[index].snakeNode.insert(0, QRectF(QPointF(snake.at(index).snakeNode.at(0).x(), snakeNodeHeight),
                                                QPointF(snake.at(index).snakeNode.at(0).x() + snakeNodeWidth, 0)));
    }
    else
    {
        snake[index].snakeNode.insert(0, QRectF(snake.at(index).snakeNode.at(0).bottomLeft(), snake.at(index).snakeNode.at(0).bottomRight() + QPointF(0, snakeNodeHeight)));
    }
}
//向左移动
void Widget::addLeftRectF(int index)
{
    if (snake.at(index).snakeNode.at(0).x() - snakeNodeWidth < 120)
    {
        snake[index].snakeNode.insert(0, QRectF(QPointF(this->width() - snakeNodeWidth, snake.at(index).snakeNode.at(0).y()),
                                                QPointF(this->width(), snake.at(index).snakeNode.at(0).y() + snakeNodeHeight)));
    }
    else
    {
        snake[index].snakeNode.insert(0, QRectF(snake.at(index).snakeNode.at(0).topLeft() + QPointF(-snakeNodeWidth, 0), snake.at(index).snakeNode.at(0).bottomLeft()));
    }
}
//向右移动
void Widget::addRightRectF(int index)
{
    if (snake.at(index).snakeNode.at(0).x() + snakeNodeWidth * 2 > this->width())
    {
        snake[index].snakeNode.insert(0, QRectF(QPointF(120, snake.at(index).snakeNode.at(0).y()),
                                                QPointF(120 + snakeNodeWidth, snake.at(index).snakeNode.at(0).y() + snakeNodeHeight)));
    }
    else
    {
        snake[index].snakeNode.insert(0, QRectF(snake.at(index).snakeNode.at(0).topRight(), snake.at(index).snakeNode.at(0).bottomRight() + QPointF(snakeNodeWidth, 0)));
    }
}
//删除结尾数据
void Widget::deleteLastRectF(int index)
{
    snake[index].snakeNode.removeLast();
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    QFont font("方正舒体", 12, QFont::ExtraLight, false);

    //反锯齿
    painter.setRenderHint(QPainter::Antialiasing);

    pen.setColor(Qt::black);
    brush.setColor(Qt::green);
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);

    for (int k = 0; k < snake.length(); k++)
    {
        if (snake.at(k).buff){
            brush.setColor(Qt::red);
            painter.setBrush(brush);
        }
        else{
            brush.setColor(Qt::green);
            painter.setBrush(brush);
        }
        for (int i = 0; i < snake.at(k).snakeNode.length(); i++)
        {
            painter.drawRect(snake.at(k).snakeNode.at(i));
        }
    }
    for (int i = 0; i < rewardNode.length(); i++)
    {
        if (rewardNode.at(i).nodeType == 1)
        {
            brush.setColor(Qt::red);
            painter.setBrush(brush);
        }
        else if (rewardNode.at(i).nodeType == 2)
        {
            brush.setColor(Qt::green);
            painter.setBrush(brush);
        }
        else if (rewardNode.at(i).nodeType == 3)
        {
            brush.setColor(Qt::blue);
            painter.setBrush(brush);
        }
        else if (rewardNode.at(i).nodeType == 4)
        {
            brush.setColor(Qt::yellow);
            painter.setBrush(brush);
        }
        else
        {
            brush.setColor(Qt::black);
            painter.setBrush(brush);
        }
        painter.drawEllipse(rewardNode.at(i).coordinate);
    }

    for (int i = 0; i < barrier.length(); i++)
    {
        brush.setColor(Qt::blue);
        painter.setBrush(brush);
        painter.drawRect(barrier.at(i));
        continue;
    }
    if(snake[0].life>=0){
        ui->life->setText(QString("%1").arg(snake[0].life));
    }

    if (snakeStrike())
    {
        QFont font("方正舒体", 30, QFont::ExtraLight, false);
        painter.setFont(font);
        painter.drawText((this->width() - 300) / 2, (this->height() - 30) / 2, QString("GAME OVER!"));
        for (int k = 0; k < snake.length(); k++)
        {
            snake[k].snakeTimer->stop();
        }
        ui->pause->setText("开始");
        rewardTimer->stop();
        paintTimer->stop();
        status = false;
        gameOver=true;
    }

    QWidget::paintEvent(event);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        if (snake[0].moveFlage != Down)
        {
            snake[0].nextFlage = Up;
        }
        break;
    case Qt::Key_S:
        if (snake[0].moveFlage != Up)
        {
            snake[0].nextFlage = Down;
        }
        break;
    case Qt::Key_D:
        if (snake[0].moveFlage != Left)
        {
            snake[0].nextFlage = Right;
        }
        break;
    case Qt::Key_A:
        if (snake[0].moveFlage != Right)
        {
            snake[0].nextFlage = Left;
        }
        break;
    default:
        break;
    }
    if (player_num > 1)
    {
        switch (event->key())
        {
        case Qt::Key_I:
            if (snake[1].moveFlage != Down)
            {
                snake[1].nextFlage = Up;
            }
            break;
        case Qt::Key_K:
            if (snake[1].moveFlage != Up)
            {
                snake[1].nextFlage = Down;
            }
            break;
        case Qt::Key_L:
            if (snake[1].moveFlage != Left)
            {
                snake[1].nextFlage = Right;
            }
            break;
        case Qt::Key_J:
            if (snake[1].moveFlage != Right)
            {
                snake[1].nextFlage = Left;
            }
            break;
        default:
            break;
        }
    }
}

bool Widget::snakeStrike()
{
    if (player_num > 1)
    {
        for (int i = 0; i < snake.length(); i++)
        {
            for (int j = 0; j < snake.length(); j++)
            {
                for (int k = 1; k < snake[j].snakeNode.length(); k++)
                {
                    if (snake[i].snakeNode.at(0) == snake[j].snakeNode.at(k))
                    {
                        snake[i].life--;
                    }
                }
            }
        }
    }
    else{
        for (int i = 1; i < snake[0].snakeNode.length(); i++)
        {
            if (snake[0].snakeNode.at(0) == snake[0].snakeNode.at(i))
            {
                snake[0].life--;
                if (snake[0].life == 0)
                {
                    return true;
                }
            }
        }
    }
    for(int k=0;k<snake.length();k++){
        for (int i = 0; i < barrier.length(); i++)
        {
            if (snake[k].snakeNode.at(0) == barrier.at(i))
            {
                if (snake[k].buff)
                {
                    barrier.removeAt(i);
                }
                else
                {
                    snake[k].life--;
                    if (snake[k].life == 0&&player_num==1)
                    {
                        return true;
                    }
                }
            }
        }
    }
   if(player_num>1){
       int sum=0;
       for(int i=0;i<snake.length();i++){
           if(snake[i].life>0){
               sum++;
           }
       }
       if(sum<=1){
           return true;
       }
   }
    return false;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pause_clicked()
{
        if(gameStart){
            gameStart=false;
            status=true;
            ui->pause->setText("暂停");
            if(load){
                for(int i=0;i<snake.length();i++){
                    snake[i].snakeTimer->start(snake[i].time);
                }
                paintTimer->start(20);
                rewardTimer->start(rewardTime * 30);
            }
            else{
                initGame();
                for(int i=0;i<snake.length();i++){
                    snake[i].snakeTimer->start(snake[i].time);
                }
                paintTimer->start(20);
                rewardTimer->start(rewardTime * 30);
            }
            return;
        }
        if(gameOver){
            status=true;
            initGame();
            ui->pause->setText("暂停");
            update();
            for(int i=0;i<snake.length();i++){
                snake[i].snakeTimer->start(snake[i].time);
            }
            paintTimer->start(20);
            rewardTimer->start(rewardTime * 30);
            return;
        }
        status = !status;
        if (status)
        {
            ui->pause->setText("暂停");
            for(int i=0;i<snake.length();i++){
                snake[i].snakeTimer->start(snake[i].time);
            }
            paintTimer->start(20);
            rewardTimer->start(rewardTime * 30);
        }
        else
        {
            ui->pause->setText("继续");
            for(int i=0;i<snake.length();i++){
                snake[i].snakeTimer->stop();
            }
            paintTimer->stop();
            rewardTimer->stop();
        }
}

void Widget::initGame(){
    rewardNode.clear();
    barrier.clear();
    rewardNode.append(QRectF(200, 100, snakeNodeWidth, snakeNodeWidth));
    rewardTimer->stop();
    paintTimer->stop();
    for(int i=0;i<snake.length();i++){
        snake[i].snakeNode.clear();
        snake[i].buff=false;
        snake[i].buffTimer->stop();
        snake[i].life=1;
        snake[i].time=100;
        snake[i].snakeTimer->stop();
        snake[i].moveFlage = Up;
        snake[i].moveFlage=Up;
        snake[i].snakeNode.append(QRectF(200+i*100, 500, snakeNodeWidth, snakeNodeHeight));
        addTopRectF(i);
        addTopRectF(i);
    }
}

void Widget::on_restart_clicked()
{
    ui->pause->setText("开始");
    status = false;
    gameStart = true;
    gameOver=false;
    initGame();
    update();
}

void Widget::saveGame()
{
    QJsonDocument jdoc;
    QJsonObject obj;
    QJsonObject snakeObj;
    QJsonArray snakeArr;
    if(player_num==1){
        obj["name"] = "single_player";
    }
    else{
        obj["name"] = "multi_player";
    }
    obj["player_num"]=player_num;
    for(int index=0;index<snake.length();index++){
        QJsonArray snakeNodeArr;
        for (int i = 0; i < snake.at(index).snakeNode.length(); i++)
        {
            QJsonObject point;
            point["x"] = snake.at(index).snakeNode.at(i).x();
            point["y"] = snake.at(index).snakeNode.at(i).y();
            snakeNodeArr.append(point);
        }
        snakeObj["node"]=snakeNodeArr;
        snakeObj["life"]=snake[index].life;
        snakeObj["moveFlage"]=snake[index].moveFlage;
        snakeObj["time"]=snake[index].time;
        snakeObj["buff"]=snake[index].buff;
        snakeArr.append(snakeObj);
    }
    obj["snake"]=snakeArr;
    QJsonArray rewardArr;
    for (int i = 0; i < rewardNode.length(); i++)
    {
        QJsonObject point;
        point["x"] = rewardNode.at(i).coordinate.x();
        point["y"] = rewardNode.at(i).coordinate.y();
        point["type"] = rewardNode.at(i).nodeType;
        rewardArr.append(point);
    }
    obj["rewardNode"] = rewardArr;
    QJsonArray barrierArr;
    for (int i = 0; i < barrier.length(); i++)
    {
        QJsonObject point;
        point["x"] = barrier.at(i).x();
        point["y"] = barrier.at(i).y();
        barrierArr.append(point);
    }
    obj["barrier"] = barrierArr;

    jdoc.setObject(obj);
    if (player_num == 1)
    {
        QFile file(QApplication::applicationDirPath() + "/single_player.json");
        if (!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "File open failed!";
        }
        else
        {
            qDebug() << "File open successfully!";
            file.write(jdoc.toJson(QJsonDocument::Indented)); //Indented:表示自动添加/n回车符
            file.close();
        }
    }
    else
    {
        QFile file(QApplication::applicationDirPath() + "/multi_player.json");
        if (!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "File open failed!";
        }
        else
        {
            qDebug() << "File open successfully!";
            file.write(jdoc.toJson(QJsonDocument::Indented)); //Indented:表示自动添加/n回车符
            file.close();
        }
    }
}

void Widget::restoreSave(QJsonObject obj)
{
    QStringList list = obj.keys();
    foreach (QString key, list)
    {
        if(key=="player_num"){
            setPlayerNum(obj["player_num"].toInt());
        }
        if (obj[key].isArray() && key == "snake")
        {
            QJsonArray arr = obj[key].toArray();
            for (int i = 0; i < arr.count(); i++)
            {
                QJsonObject arrObj = arr[i].toObject();
                snake[i].buff=arrObj["buff"].toBool();
                snake[i].life=arrObj["life"].toInt();
                snake[i].moveFlage=arrObj["moveFlage"].toInt();
                snake[i].time=arrObj["time"].toInt();
                QJsonArray nodeArr=arrObj["node"].toArray();
                for(int j=0;j<nodeArr.count();j++){
                    QJsonObject node=nodeArr[j].toObject();
                    QRectF snakeNode(node["x"].toInt(), node["y"].toInt(), snakeNodeWidth, snakeNodeHeight);
                    snake[i].snakeNode.push_back(snakeNode);
                }
            }
        }
        if (obj[key].isArray() && key == "rewardNode")
        {
            QJsonArray arr = obj[key].toArray();
            for (int i = 0; i < arr.count(); i++)
            {
                QJsonObject arrObj = arr[i].toObject();
                QRectF reward(arrObj["x"].toInt(), arrObj["y"].toInt(), snakeNodeWidth, snakeNodeHeight);
                RewardNode tmpNode(reward, arrObj["type"].toInt());
                rewardNode.push_back(tmpNode);
            }
        }
        if (obj[key].isArray() && key == "barrier")
        {
            QJsonArray arr = obj[key].toArray();
            for (int i = 0; i < arr.count(); i++)
            {
                QJsonObject arrObj = arr[i].toObject();
                QRectF barrierNode(arrObj["x"].toInt(), arrObj["y"].toInt(), snakeNodeWidth, snakeNodeHeight);
                barrier.push_back(barrierNode);
            }
        }
    }
    load=true;
    update();
}

bool Widget::loadGame(int number)
{
    if (number==1)
    {
        QFile file(QApplication::applicationDirPath() + "/multi_player.json");
        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "File open failed!";
            return false;
        }
        qDebug() << "File open successfully!";
            QJsonParseError *error = new QJsonParseError;
            QJsonDocument jdc = QJsonDocument::fromJson(file.readAll(), error);
            //判断文件是否完整
            if (error->error != QJsonParseError::NoError)
            {
                qDebug() << "parseJson:" << error->errorString();
                return false;
            }
            QJsonObject obj = jdc.object(); //获取对象
            restoreSave(obj);
            return true;


    }
    QFile file(QApplication::applicationDirPath() + "/single_player.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

        QJsonParseError *error = new QJsonParseError;
        QJsonDocument jdc = QJsonDocument::fromJson(file.readAll(), error);
        //判断文件是否完整
        if (error->error != QJsonParseError::NoError)
        {
            qDebug() << "parseJson:" << error->errorString();
            return false;
        }
        QJsonObject obj = jdc.object(); //获取对象
        restoreSave(obj);
        return true;
}

void Widget::mousePressEvent(QMouseEvent *ev){
//    qDebug()<<ev->x()<<" "<<ev->y();
    if(ev->x()>=120&&operation>=0){
        for(int i=0;i<snake.length();i++){
            for (int j=0;j<snake[i].snakeNode.length();j++){
                if(snake[i].snakeNode[j].contains(ev->x(),ev->y())){
                    return;
                }
            }
        }
        for(int i=0;i<barrier.length();i++){
            if(barrier[i].contains(ev->x(),ev->y())){
                barrier.removeAt(i);
            }
        }
        for(int i=0;i<rewardNode.length();i++){
            if(rewardNode[i].coordinate.contains(ev->x(),ev->y())){
                rewardNode.removeAt(i);
            }
        }
        int x=(ev->x())/20*20;
        int y=(ev->y())/20*20;
        QRectF coordinate(x, y, snakeNodeWidth, snakeNodeWidth);
        if(operation==1){
            barrier.push_back(coordinate);
        }
        if(operation>1){
            RewardNode tmp(coordinate,operation-2);
            rewardNode.push_back(tmp);
        }
        update();
    }
}
