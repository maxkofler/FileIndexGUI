#include "log.h"
#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>

#include <QStandardPaths>
#include <QDir>

int main(int argc, char *argv[]){
    hlog = new Log::Log(Log::D);
    int res = 0;
    {
        FUN();

        QApplication::setOrganizationDomain("maxkofler.eu");
        QApplication::setOrganizationName("maxkofler");
        QApplication::setApplicationName("FileIndexGUI");
        QSettings::setDefaultFormat(QSettings::IniFormat);

        QSettings s;
        LOGU("[MainWindow] Testvalue in settings: " + std::to_string(s.value("test/testInt", 0).toInt()));
        s.setValue("test/testInt", s.value("test/testInt", 0).toInt()+1);

        QApplication a(argc, argv);

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
