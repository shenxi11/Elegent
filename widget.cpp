 #include "widget.h"
#include "ui_widget.h"
#include <algorithm>
#include <QStringConverter>
#include<QDebug>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QTimer>
#include<QTime>
#include<QRegularExpression>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{   q=0;
    ui->setupUi(this);
    ui->listWidget->close();
    ui->voice->close();
    setlist();
    setMusic();
    setlrcPath();
    ui->progress->setTracking(false);
    ui->label_3->setText(strList[0]);
    ui->label->setPixmap(QPixmap(":/new/picture/picture/back.png"));
    connect(ui->choose,&QPushButton::clicked,[=](){
        ui->listWidget->show();
        if(!ui->choose->isChecked())
        {
            ui->listWidget->close();
        }
    });

    QAudioOutput * audioOutput;
    audioOutput = new QAudioOutput;
    QMediaPlayer*player=new QMediaPlayer;
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(70);
    player->seekableChanged(1);
    ui->voice->setValue(70);
    //connect(player, &QMediaPlayer::positionChanged, this, &MediaExample::positionChanged);
    player->setSource(list[0]);

    connect(player,&QMediaPlayer::mediaStatusChanged,[=](){
        if(player->mediaStatus()==QMediaPlayer::LoadingMedia)
        {
            //ui->label_8->show();
        }
        if(player->mediaStatus()==QMediaPlayer::LoadedMedia)
        {  //ui->label_8->close();
            ui->label_6->setText(settime(player));
            Gprogress(player);
        }if(player->mediaStatus()==QMediaPlayer::EndOfMedia)
        {q=(q+1)%list.size();player->setSource(list[q]);
            ui->label_3->setText(strList[q]);
            player->play();
        }
    });
    //实时进度条，显示时间
    connect(player,&QMediaPlayer::positionChanged,[=](){
        progress(player);
        ui->label_7->setText( begin(player));
    });
    //调出音量条
    connect(ui->vo,&QPushButton::clicked,[=](){
        ui->voice->show(); ui->label_4->setText(QString::number(ui->voice->value()));
        ui->label_4->show();
        if(!ui->vo->isChecked())
        {
            ui->voice->close();
            ui->label_4->close();
        }
    });
    //调节音量
    connect(ui->voice,&QSlider::valueChanged,[=](){
        qreal linearVolume = QAudio::convertVolume(ui->voice->value()/ qreal(100.0), QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
        audioOutput->setVolume(linearVolume);
        ui->label_4->setText(QString::number(ui->voice->value()));
    });
    //歌曲栏
    connect(ui->listWidget,&QListWidget::itemClicked,[=]()mutable{
        q=ui->listWidget->currentRow();
        player->pause();
        player->setSource(list[q]);
        ui->label_3->setText(strList[q]);
        ui->listWidget->close();
        //        player->play();
        //        if(player->playbackState()==QMediaPlayer::PlayingState)
        //        {
        //            ui->pause->setStyleSheet("border-image:url(:/new/picture/picture/pause.png);");
        //        }
    });
    //获取歌曲状态
    connect(player,&QMediaPlayer::playbackStateChanged,[=](){
        if(player->playbackState()==QMediaPlayer::PlayingState)//显示为播放按钮
        {
            //QString currentUrl=player->
            displayLyrics(lrcPath[0]);
            ui->pause->setStyleSheet("border-image:url(:/new/picture/picture/pause.png);");
        }if(player->playbackState()==QMediaPlayer::PausedState||QMediaPlayer::StoppedState)//显示为暂停按钮
        {
            ui->pause->setStyleSheet("border-image:url(:/new/picture/picture/begin.png);");
        }
    });

    //暂停与开始
    connect(ui->pause,&QPushButton::clicked,[=](){

        player->play();

        if(!ui->pause->isChecked()){
            player->pause();
        }
    });
    //下一首
    connect(ui->next,&QPushButton::clicked,[=](){
        q++;
        if(q>=strList.count()){q=0;}
        player->setSource(list[q]);
        ui->label_3->setText(strList[q]);
        player->pause();

        // ui->label_6->setText(settime(player));

        if
            (player->playbackState()==QMediaPlayer::PausedState||QMediaPlayer::StoppedState)
        {
            ui->pause->setStyleSheet("border-image:url(:/new/picture/picture/begin.png);");
        }
    });
    connect(player,&QMediaPlayer::durationChanged,[=](){
        ui->label_6->setText(settime(player));
    });

    //上一首
    connect(ui->last,&QPushButton::clicked,[=](){
        q--;
        if(q<0){q=strList.count()-1;}
        player->setSource(list[q]);
        ui->label_3->setText(strList[q]);
        player->pause();
        //ui->label_6->setText(settime(player));
        if(player->playbackState()==QMediaPlayer::PausedState)
        {
            ui->pause->setStyleSheet("border-image:url(:/new/picture/picture/begin.png);");
        }
    });
    //进度条移动
    connect(ui->progress,&QSlider::sliderMoved,[=](){
        player->pause();
    });
    connect(ui->progress,&QSlider::sliderReleased,[=](){

        player->setPosition(ui->progress->value());
        player->play();
    });

    QString lrcFilePath = lrcPath[0];
    QFile file(lrcFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开歌词文件";
        return;
    }

}

void Widget::setlist()
{
    list.append(QUrl("qrc:/new/sound/music/ai_qing_fei_chai.mp3"));
    list.append(QUrl("qrc:/new/sound/music/ge_qian.mp3"));
    list.append(QUrl("qrc:/new/sound/music/hua_hai.mp3"));
    list.append(QUrl("qrc:/new/sound/music/long_juan_feng.mp3"));
    list.append(QUrl("qrc:/new/sound/music/qi_li_xiang.ogg"));
    list.append(QUrl("qrc:/new/sound/music/qing_hua_ci.mp3"));
    list.append(QUrl("qrc:/new/sound/music/ye_qu.mp3"));
    list.append(QUrl("qrc:/new/sound/music/yi_lu_xiang_bei.mp3"));
}
void Widget::setMusic()
{

    strList<< "爱情废柴"<<"搁浅"<< "花海"<<"龙卷风"<<"七里香"<<"青花瓷"<<"夜曲"<<"一路向北";
    ui->listWidget->addItems(strList);
}
void Widget::setPic()
{
    strPic.append(":/new/picture/picture/ai_qing_fei_chai.png");
    strPic.append(":/new/picture/picture/ge_qian.png");
    strPic.append(":/new/picture/picture/hua_hai.png");
    strPic.append(":/new/picture/picture/long_juan_feng.png");
    strPic.append(":/new/picture/picture/qi_li_xiang.png");
    strPic.append(":/new/picture/picture/qing_hua_ci.png");
    strPic.append(":/new/picture/picture/ye_qu.png");
    strPic.append(":/new/picture/picture/yi_lu_xiang_bei.png");
}
QString Widget::settime(QMediaPlayer*player)
{
    QTime totalTime=QTime(0,0).addMSecs(player->duration());
    QString totalTimeText=totalTime.toString("mm:ss");
    return totalTimeText;
}
void Widget::Gprogress(QMediaPlayer*player)
{
    ui->progress->setRange(0,player->duration());
    ui->progress->setSingleStep(1000);
    ui->progress->setPageStep(10000);
    ui->progress->setTracking(true);
}
void Widget::progress(QMediaPlayer*player)
{
    ui->progress->setValue(player->position());
}
QString Widget::begin(QMediaPlayer*player)
{
    QTime totalTime=QTime(0,0).addMSecs(player->position());
    QString totalTimeText=totalTime.toString("mm:ss");
    return totalTimeText;
}
void Widget::setlrcPath()
{
    lrcPath.append(":/new/sound/music/ai_qing_fei_chai.lrc");
    lrcPath.append(":/new/sound/music/ge_qian.lrc");
    lrcPath.append(":/new/sound/music/hua_hai.lrc");
    lrcPath.append(":/new/sound/music/long_juan_feng.lrc");
    lrcPath.append(":/new/sound/music/qi_li_xiang.lrc");
    lrcPath.append(":/new/sound/music/qing_hua_ci.lrc");
    lrcPath.append(":/new/sound/music/ye_qu.lrc");
    lrcPath.append(":/new/sound/music/yi_lu_xiang_bei.lrc");
}

void Widget::displayLyrics(const QString& lyricsFile)
{
    QString lrcFilePath = lyricsFile;
    QFile file(lrcFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开歌词文件";
        return;
    }
    QTextStream in(&file);
    QList<int> timeLabels;
    QList<QString> lyrics;
    while (!in.atEnd()) {
        QString line = in.readLine();
        // 解析每一行的LRC内容
        // 例如，提取时间标签和歌词文本
        // 假设时间标签的格式为：[mm:ss.xx]
        QRegularExpression rx("\\[([0-9]{2}):([0-9]{2})\\.([0-9]{2})\\]");
        QRegularExpressionMatch match = rx.match(line);
        if (match.hasMatch()) {
            int minutes = match.captured(1).toInt();
            int seconds = match.captured(2).toInt();
            int milliseconds = match.captured(3).toInt();
            QString lyricsText = line.mid(match.capturedLength());
            // 将时间标签转换为毫秒
            int timeLabel = minutes * 60000 + seconds * 1000 + milliseconds;
            // 将时间标签和歌词文本保存到列表中
            timeLabels.append(timeLabel);
            lyrics.append(lyricsText);
        }
    }
    file.close();
    QTimer timer;
    int currentIndex = 0;
    int elapsedTime = 0;
    connect(&timer, &QTimer::timeout, [&]() {
        elapsedTime += timer.interval();
        while (currentIndex < timeLabels.size() && elapsedTime >= timeLabels[currentIndex]) {
            ui->label_9->setText(QString("%1 %2").arg(timeLabels[currentIndex]).arg(lyrics[currentIndex]));
            currentIndex++;
        }
        if (currentIndex >= timeLabels.size()) {
            timer.stop();
        }
    });
    timer.setInterval(100);  // 每0.1秒检查一次进度
    timer.start();
}
Widget::~Widget()
{
    delete ui;
}

