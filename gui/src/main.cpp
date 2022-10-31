#include "log.h"
#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>

int main(int argc, char *argv[]){
    hlog = new Log::Log(Log::MEM);
    int res = 0;
    {
        FUN();
        QApplication a(argc, argv);

        a.setApplicationName("FileIndexGUI");
        QSettings::setDefaultFormat(QSettings::IniFormat);

        QTranslator translator;
        LOGU("[Main] " + QApplication::tr("Starting up...").toStdString());
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale : uiLanguages) {
            const QString baseName = "FileIndexGUI_" + QLocale(locale).name();
            LOGI("[Main] Trying to load translation file " + baseName.toStdString() + "...");
            if (translator.load(":/i18n/" + baseName)) {
                a.installTranslator(&translator);
                break;
            }
        }
        MainWindow w;

        w.show();
        res = a.exec();
    }

    delete hlog;

    return res;
}
