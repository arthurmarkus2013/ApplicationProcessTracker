#ifndef BUSINESSLOGIC_H
#define BUSINESSLOGIC_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlError>
#include <QStringList>
#include <QDate>
#include <QMessageBox>
#include <QStandardPaths>

#include "EntryHelper.h"

class BusinessLogic : QObject
{
    Q_OBJECT

public:
    BusinessLogic(QObject *parent = nullptr);
    int getItemCount();
    QList<EntryHelper::Entry> getItems();
    bool addItem(EntryHelper::Entry item);
    bool updateItem(EntryHelper::Entry oldItem, EntryHelper::Entry newItem);
    bool removeItem(EntryHelper::Entry item);
    bool exportAsPDF(QString path, QWidget *parent = nullptr);

private:
    QStringList prepareDataForExport();
};

#endif // BUSINESSLOGIC_H
