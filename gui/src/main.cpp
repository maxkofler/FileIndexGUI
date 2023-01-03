#include "log.h"
#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>

#include <QStandardPaths>
#include <QDir>

#include <fstream>

int main(int argc, char *argv[]){
    hlog = new Log::Log(Log::D);

    std::ofstream* traceFile = new std::ofstream("trace.json");
    hlog->setFeature(Log::FEATURE_PROFILE, true);
    hlog->setProfileStream(traceFile);

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
        LOGU("[Main] Current applicatinoDirPath=" + a.applicationDirPath().toStdString());

        QTranslator translator;
        LOGU("[Main] " + QApplication::tr("Starting up...").toStdString());

        QString overrideLang = s.value("startup/overrideLang", "").toString();
        bool loadedOverrideLang = false;
        if (overrideLang != ""){
            if (translator.load("FileIndexGUI_" + overrideLang)){
                a.installTranslator(&translator);
                LOGU("[Main] OVERRIDE Language file " + overrideLang.toStdString() + " loaded");
                loadedOverrideLang = true;
            } else {
                LOGU("[Main] Failed to load override language file " + overrideLang.toStdString() + ", falling back to system language");
            }
        }

        if (!loadedOverrideLang){
            const QStringList uiLanguages = QLocale::system().uiLanguages();
            for (const QString &locale : uiLanguages) {
                const QString baseName = "FileIndexGUI_" + QLocale(locale).name() + "";
                if (translator.load(baseName, a.applicationDirPath())) {
                    LOGU("[Main] Loaded language file " + baseName.toStdString());
                    a.installTranslator(&translator);
                    break;
                } else
                    LOGU("[Main] Failed to load language file " + baseName.toStdString());
            }
        }


        MainWindow w;

        w.show();
        res = a.exec();
    }

    delete hlog;
    delete traceFile;

    return res;
}
