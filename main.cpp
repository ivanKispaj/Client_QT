#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <unistd.h>
#include <stdio.h>
#include <filesystem>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string path = currentPath;
    if (!translator.load("Chat_translations/my_ru.qm"))
    {
        QString message = "Failed to upload translations. The 'Chat_translations' folder with translations should be located in:  " + QString::fromStdString(path);
        QMessageBox::critical(nullptr, "Error", message);
        qDebug() << "Falid load translations";
        qDebug() << "The translations folder should be located in : " + path ;
    } else
    {
        a.installTranslator(&translator);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
