#ifndef MEDIA_H
#define MEDIA_H

#include <QObject>
#include<QMediaPlayer>
#include<QList>
class media : public QMediaPlayer
{
    Q_OBJECT
public:
    explicit media(QObject *parent = nullptr);

signals:

};

#endif // MEDIA_H
