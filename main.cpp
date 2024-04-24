#include "widget.h"

#include <QMediaPlayer>
#include<QFile>
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/style.qss");
    if(file.exists()){
        file.open(QFile::ReadOnly);
        QString styleSheet=QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
        file.close();
    }
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "untitled23_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    Widget w;

    w.show();

    return a.exec();
}
