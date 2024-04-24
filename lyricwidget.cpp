#include "LyricWidget.h"
#include "ui_LyricWidget.h"

#include <algorithm>
#include <QTextCodec>
#include<QDebug>
LyricWidget::LyricWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LyricWidget)
{
    ui->setupUi(this);
    clear();//清空用于排版时的测试内容(.ui文件中)
    ui->label_i->setStyleSheet("color:LightSkyBlue");
}

LyricWidget::~LyricWidget()
{
    delete ui;
}


//重载比较（歌词按时间排序）
bool operator <(const LyricLine& A, const LyricLine& B)
{
    return A.time<B.time;
}

bool LyricWidget::process(QString filePath)
{
    QFile lyricFile(filePath);
    lyricFile.open(QFile::ReadOnly);
    //QString content(QString::fromLocal8Bit(lyricFile.readAll()));
    QString content(lyricFile.readAll());
    qDebug()<<"歌词\n"<<content;
                    lyricFile.close();

    //先清空歌词
    lines.clear();

    const QRegExp rx("\\[(\\d+):(\\d+(\\.\\d+)?)\\]"); //用来查找时间标签的正则表达式

    // 步骤1
    int pos = rx.indexIn(content);//返回第一个匹配项的位置，如果没有匹配项则返回-1。
    if (pos == -1) {
        return false;
    }
    else {
        int lastPos;//
        QList<int> timeLabels;//时间标签
        do {
            // 步骤2
            timeLabels << (rx.cap(1).toInt() * 60 + rx.cap(2).toDouble()) * 1000;
            lastPos = pos + rx.matchedLength();//返回最后一个匹配字符串的长度，如果没有匹配则返回-1
            pos = rx.indexIn(content, lastPos);
            if (pos == -1) {
                QString text = content.mid(lastPos).trimmed();
                foreach (const int& time, timeLabels)
                    lines.push_back(LyricLine(time, text));
                break;
            }
            // 步骤3
            QString text = content.mid(lastPos, pos - lastPos);
            if (!text.isEmpty()) {
                foreach (const int& time, timeLabels)
                    lines.push_back(LyricLine(time, text.trimmed()));
                timeLabels.clear();
            }
        }
        while (true);
        // 步骤4
        stable_sort(lines.begin(), lines.end());//排序
    }
    if (lines.size()) {
        return true;
    }
    return false;
}

int LyricWidget::getIndex(qint64 position)//获取歌词位置
{
    if(!lines.size()){
        return -1;//如果没歌词
    }
    else{
        if(lines[0].time>=position){
            return 0;
        }
    }
    int i=1;
    for(i=1;i<lines.size();i++){
        if(lines[i-1].time<position && lines[i].time>=position){
            return i-1;
        }
    }
    return lines.size()-1;
}

void LyricWidget::showcontent(qint64 position)//显示当前播放进度的歌词
{
    int index=getIndex(position);
    if(index==-1){
        ui->label_3i->setText("");
        ui->label_2i->setText("");
        ui->label_1i->setText("");
        ui->label_i->setText(u8"当前歌曲无歌词");
        ui->label_i1->setText("");
        ui->label_i2->setText("");
        ui->label_i3->setText("");
    }else{
        ui->label_3i->setText(getLyricText(index-3));
        ui->label_2i->setText(getLyricText(index-2));
        ui->label_1i->setText(getLyricText(index-1));
        ui->label_i->setText(getLyricText(index));
        ui->label_i1->setText(getLyricText(index+1));
        ui->label_i2->setText(getLyricText(index+2));
        ui->label_i3->setText(getLyricText(index+3));
    }
}

QString LyricWidget::getLyricText(int index)//根据下标获得歌词内容
{
    if(index<0 || index>=lines.size()){
        return "";
    }else{
        //qDebug()<<"输出歌词："<<lines[index].text;
        return lines[index].text;
    }
}

void LyricWidget::clear()
{
    ui->label_3i->setText("");
    ui->label_2i->setText("");
    ui->label_1i->setText("");
    ui->label_i->setText("");
    ui->label_i1->setText("");
    ui->label_i2->setText("");
    ui->label_i3->setText("");
}
