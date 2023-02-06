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

#include "Entry.h"

class BusinessLogic : QObject
{
    Q_OBJECT

public:
    BusinessLogic();
    int getItemCount();
    QList<Entry> getItems();
    bool addItem(Entry item);
    bool updateItem(Entry oldItem, Entry newItem);
    bool removeItem(Entry item);
    bool exportAsPDF(QString path, QWidget *parent = nullptr);

private:
    QStringList prepareDataForExport();
};

#endif // BUSINESSLOGIC_H
