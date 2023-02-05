#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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
            qDebug() << db.lastError().text();
        }

        db.exec("CREATE TABLE IF NOT EXISTS Applications "
                "(application_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "company_name TEXT NOT NULL, "
                "applied_on INTEGER NOT NULL, "
                "latest_status INTEGER NOT NULL, "
                "updated_on INTEGER NOT NULL)");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ApplicationProcessTracker_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}