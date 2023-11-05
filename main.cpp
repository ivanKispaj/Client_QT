#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <unistd.h>
#include <stdio.h>
#include <QMessageBox>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    QString cwd = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    if (!translator.load(cwd + "/Chat_translations/my_ru.qm"))
    {
        QString message = "Failed to upload translations. The 'Chat_translations' folder with translations should be located in:  " + cwd + "/";
        QMessageBox::critical(nullptr, "Error", message);;
    } else
    {
        a.installTranslator(&translator);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
