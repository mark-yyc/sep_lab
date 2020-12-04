#include "snake.h"
#include "ui_snake.h"

Snake::Snake(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Snake)
{
    ui->setupUi(this);

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()),this,SLOT(timeOut()));

    rewardTimer = new QTimer;
    connect(rewardTimer,SIGNAL(timeout()),this,SLOT(rewardTimeOut()));
}

Snake::~Snake()
{
    delete ui;
}

void Snake::timeOut()
{
    int flage = 1;
    for(int i=0; i<rewardNode.length(); i++){
//        if(rewardNode.at(i).contains(snake.at(0).topLeft()+QPointF(snakeNodeWidth/2,snakeNodeHeight/2))){
        if(snake.at(0).contains(rewardNode.at(i).coordinate.x()+rewardNode.at(i).coordinate.width()/2,
                                rewardNode.at(i).coordinate.y()+rewardNode.at(i).coordinate.height()/2)){
            if(rewardNode.at(i).nodeType==1){
                life++;
            }
            flage++;
            rewardNode.removeAt(i);
            break;
        }
    }
    while(flage--){
        switch (moveFlage) {
        case Up:
            addTopRectF();
            break;
        case Down:
            addDownRectF();
            break;
        case Right:
            addRightRectF();
            break;
        case Left:
            addLeftRectF();
            break;
        default:
            break;
        }
    }
    deleteLastRectF();

    update();
}

void Snake::rewardTimeOut()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    if(rewardNode.length() >= 5){
        rewardNode.removeAt(qrand()%5);
    }
    int x=qrand()%(this->width()/20)*20;
    if(x<=120){
        x+=120;
    }
    int y=qrand()%(this->height()/20)*20;
    QRectF coordinate(x,y,snakeNodeWidth,snakeNodeWidth);
    int key=qrand()%10;
    if(key == 0){
        RewardNode new_reward(coordinate,1);
        rewardNode.append(new_reward);
    }
    else if(key==1){
        RewardNode new_reward(coordinate,2);
        rewardNode.append(new_reward);
    }
    else if(key==2){
        RewardNode new_reward(coordinate,3);
        rewardNode.append(new_reward);
    }
    else if(key==3){
        barrier.append(coordinate);
    }
    else{
        RewardNode new_reward(coordinate,0);
        rewardNode.append(new_reward);
    }
}

void Snake::addTopRectF()
{
    if(snake.at(0).y()-snakeNodeHeight < 0){
        snake.insert(0,QRectF(QPointF(snake.at(0).x(),this->height()-snakeNodeHeight),
                              QPointF(snake.at(0).x()+snakeNodeWidth,this->height())));
    }else{
        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(0,-snakeNodeHeight),snake.at(0).topRight()));
    }
}
//向下移动
void Snake::addDownRectF()
{
    if(snake.at(0).y()+snakeNodeHeight*2 > this->height()){
        snake.insert(0,QRectF(QPointF(snake.at(0).x(),snakeNodeHeight),
                              QPointF(snake.at(0).x()+snakeNodeWidth,0)));
    }else{
        snake.insert(0,QRectF(snake.at(0).bottomLeft(),snake.at(0).bottomRight()+QPointF(0,snakeNodeHeight)));
    }
}
//向左移动
void Snake::addLeftRectF()
{
    if(snake.at(0).x()-snakeNodeWidth < 120){
        snake.insert(0,QRectF(QPointF(this->width()-snakeNodeWidth,snake.at(0).y()),
                              QPointF(this->width(),snake.at(0).y()+snakeNodeHeight)));
    }else{
        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(-snakeNodeWidth,0),snake.at(0).bottomLeft()));
    }
}
//向右移动
void Snake::addRightRectF()
{
    if(snake.at(0).x()+snakeNodeWidth*2 > this->width()){
        snake.insert(0,QRectF(QPointF(120,snake.at(0).y()),
                              QPointF(120+snakeNodeWidth,snake.at(0).y()+snakeNodeHeight)));
    }else{
        snake.insert(0,QRectF(snake.at(0).topRight(),snake.at(0).bottomRight()+QPointF(snakeNodeWidth,0)));
    }
}
//删除结尾数据
void Snake::deleteLastRectF()
{
    snake.removeLast();
}

void Snake::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    QFont font("方正舒体",12,QFont::ExtraLight,false);

    //反锯齿
    painter.setRenderHint(QPainter::Antialiasing);

    pen.setColor(Qt::black);
    brush.setColor(Qt::green);
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);

    for(int i=0; i<snake.length(); i++){
        painter.drawRect(snake.at(i));
    }
    for(int i=0; i<rewardNode.length(); i++){
        if(rewardNode.at(i).nodeType==1){
            brush.setColor(Qt::red);
            painter.setBrush(brush);
        }
        else if(rewardNode.at(i).nodeType==2){
            brush.setColor(Qt::green);
            painter.setBrush(brush);
        }
        else if(rewardNode.at(i).nodeType==3){
            brush.setColor(Qt::blue);
            painter.setBrush(brush);
        }
        else {
            brush.setColor(Qt::black);
            painter.setBrush(brush);
        }
        painter.drawEllipse(rewardNode.at(i).coordinate);
    }

    for (int i=0;i<barrier.length();i++){
        brush.setColor(Qt::blue);
        painter.setBrush(brush);
        painter.drawRect(barrier.at(i));
        continue;
    }
    ui->Score->setText(QString("%1").arg(snake.length()));

    if(snakeStrike()){
//        QFont font("方正舒体",30,QFont::ExtraLight,false);
//        painter.setFont(font);
//        painter.drawText((this->width()-300)/2,(this->height()-30)/2,QString("GAME OVER!"));
        timer->stop();
        rewardTimer->stop();
        gameOver = true;
        status=false;
    }

    QWidget::paintEvent(event);
}

void Snake::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_W:
        if(moveFlage != Down){
            moveFlage = Up;
        }
        break;
    case Qt::Key_S:
        if(moveFlage != Up){
            moveFlage = Down;
        }
        break;
    case Qt::Key_D:
        if(moveFlage != Left){
            moveFlage = Right;
        }
        break;
    case Qt::Key_A:
        if(moveFlage != Right){
            moveFlage = Left;
        }
        break;
    default:
        break;
    }
}

bool Snake::snakeStrike()
{
    for(int i=1; i<snake.length(); i++){
        if(snake.at(0) == snake.at(i)){
            return true;
        }
//        for(int j=i+1; j<snake.length(); j++){
//            if(snake.at(i) == snake.at(j)){
//                return true;
//            }
//        }
    }
    for(int i=0;i<barrier.length();i++){
        if(snake.at(0) == barrier.at(i)){
            return true;
        }
    }
    return false;

}
void Snake::on_pause_clicked()
{
    status=!status;
    if(status){
        if(!gameStart){
            gameOver=false;
            gameStart=true;
            if(gameOver){
                snake.clear();
                rewardNode.clear();
                barrier.clear();
                moveFlage = Up;
            }
            if(snake.length()==0){
                snake.append(QRectF(200,500,snakeNodeWidth,snakeNodeHeight));
                addTopRectF();
                addTopRectF();
            }
            if(rewardNode.length()==0){
                rewardNode.append(QRectF(200,100,snakeNodeWidth,snakeNodeWidth));
            }
        }
        ui->pause->setText("暂停");
        timer->start(time);
        rewardTimer->start(time*30);
    }
    else{
        ui->pause->setText("继续");
        timer->stop();
        rewardTimer->stop();
    }
}

void Snake::on_restart_clicked()
{
    status=false;
    gameOver=false;
    gameStart=false;
    snake.clear();
    rewardNode.clear();
    barrier.clear();
    moveFlage = Up;
    snake.append(QRectF(200,500,snakeNodeWidth,snakeNodeHeight));
    addTopRectF();
    addTopRectF();
    rewardNode.append(QRectF(200,100,snakeNodeWidth,snakeNodeWidth));
    update();
}

void Snake::saveGame(){
    QFile file(QApplication::applicationDirPath()+"/single_player.json");
       if(!file.open(QIODevice::WriteOnly)) {
           qDebug() << "File open failed!";
       } else {
           qDebug() <<"File open successfully!";
       }

       QJsonDocument jdoc;
       QJsonObject obj;
       QJsonArray snakeArr;
       obj["name"] = "single_player";
       obj["life"]=life;
       obj["moveFlage"]=moveFlage;
       for(int i=0;i<snake.length();i++){
           QJsonObject point;
           point["x"]=snake.at(i).x();
           point["y"]=snake.at(i).y();
           snakeArr.append(point);
       }
       obj["snake"] = snakeArr;
       QJsonArray rewardArr;
       for(int i=0;i<rewardNode.length();i++){
           QJsonObject point;
           point["x"]=rewardNode.at(i).coordinate.x();
           point["y"]=rewardNode.at(i).coordinate.y();
           point["type"]=rewardNode.at(i).nodeType;
           rewardArr.append(point);
       }
       obj["rewardNode"] = rewardArr;
       QJsonArray barrierArr;
       for(int i=0;i<barrier.length();i++){
           QJsonObject point;
           point["x"]=barrier.at(i).x();
           point["y"]=barrier.at(i).y();
           barrierArr.append(point);
       }
       obj["barrier"] = barrierArr;

       jdoc.setObject(obj);
       file.write(jdoc.toJson(QJsonDocument::Indented)); //Indented:表示自动添加/n回车符
       file.close();
}

void Snake::restoreSave(QJsonObject obj){
    QStringList list =  obj.keys();
    foreach (QString key, list) {
        if(key =="life"){
            life=obj[key].toInt();
        }
        if(key =="moveFlage"){
            moveFlage=obj[key].toInt();
        }
        if(obj[key].isArray() && key =="snake")
        {
            QJsonArray arr =  obj[key].toArray();
            for(int i=0;i<arr.count();i++)
            {
               QJsonObject arrObj= arr[i].toObject();
               QRectF snakeNode(arrObj["x"].toInt(),arrObj["y"].toInt(),snakeNodeWidth,snakeNodeHeight);
               snake.push_back(snakeNode);
            }
        }
        if(obj[key].isArray() && key =="rewardNode")
        {
            QJsonArray arr =  obj[key].toArray();
            for(int i=0;i<arr.count();i++)
            {
               QJsonObject arrObj= arr[i].toObject();
               QRectF reward(arrObj["x"].toInt(),arrObj["y"].toInt(),snakeNodeWidth,snakeNodeHeight);
               RewardNode tmpNode(reward,arrObj["type"].toInt());
               rewardNode.push_back(tmpNode);
            }
        }
        if(obj[key].isArray() && key =="barrier")
        {
            QJsonArray arr =  obj[key].toArray();
            for(int i=0;i<arr.count();i++)
            {
               QJsonObject arrObj= arr[i].toObject();
               QRectF barrierNode(arrObj["x"].toInt(),arrObj["y"].toInt(),snakeNodeWidth,snakeNodeHeight);
               barrier.push_back(barrierNode);
            }
        }
    }
    update();
}

bool Snake::loadGame(){
    QFile file(QApplication::applicationDirPath()+"/single_player.json");
         if(!file.open(QIODevice::ReadOnly)) {
             qDebug() << "File open failed!";
             return false;
         }
         qDebug() <<"File open successfully!";
         int ret = QMessageBox::warning(0,"Warning",QString("save has been changed!\n Do you want to restore it?"),QMessageBox::Yes |QMessageBox::No,QMessageBox::No);
         if(ret == QMessageBox::Yes) {
             QJsonParseError *error=new QJsonParseError;
             QJsonDocument jdc=QJsonDocument::fromJson(file.readAll(),error);
             //判断文件是否完整
             if(error->error!=QJsonParseError::NoError)
             {
               qDebug()<<"parseJson:"<<error->errorString();
               return false;
             }
             QJsonObject obj = jdc.object();        //获取对象
             restoreSave(obj);
             return true;
         }
         else{
             return false;
         }
}
