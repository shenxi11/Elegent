#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMediaPlayer>
#include<QList>
#include<QListWidgetItem>
#include<QMap>
#include<QFile>
#include<QTextStream>
#include<QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    QMap<int,QString>lrcMap;
    QList<QUrl>list;

    void setlist();
    void setMusic();
    void setPic();
    QString settime(QMediaPlayer*player);
   // QMap<int,QString> lrcMap;
    QStringList strList;
    QList<QString>lrcPath;
    void setlrcPath();
     int q;
    void Gprogress(QMediaPlayer*player);
     void progress(QMediaPlayer*player);
    QList<QString>strPic;
     QString begin(QMediaPlayer*player);
    void displayLyrics(const QString& lyricsFile);

    ~Widget();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
