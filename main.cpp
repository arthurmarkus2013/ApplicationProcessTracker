#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>

#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;

    QString dbPath = a.applicationDirPath() + QDir::separator() + "store.db";
        QFile file(dbPath);
        if(!file.exists())
        {
            file.open(QIODevice::OpenModeFlag::WriteOnly);
        }
        file.close();

        auto db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbPath);

        if(!db.open())
        {
            QMessageBox::critical(nullptr, translator.tr("Fatal Error"), translator.tr("Failed to open the database file"));
            a.exit(1);
        }

        db.exec("CREATE TABLE IF NOT EXISTS Applications "
                "(application_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "company_name TEXT NOT NULL, "
                "applied_on INTEGER NOT NULL, "
                "latest_status INTEGER NOT NULL, "
                "updated_on INTEGER NOT NULL)");

//#define _LOCALE_TEST

#ifndef _LOCALE_TEST
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ApplicationProcessTracker_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
#else
    auto locale = "ru_RU";
    QLocale::setDefault(QLocale(locale));
    if (translator.load(":/i18n/ApplicationProcessTracker_" + QLocale(locale).name())) {
        a.installTranslator(&translator);
    }
#endif

    MainWindow w;
    w.show();
    return a.exec();
}
