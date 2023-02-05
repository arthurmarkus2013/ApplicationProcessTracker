#ifndef ADDENTRYDIALOG_H
#define ADDENTRYDIALOG_H

#include "qdatetime.h"
#include <QDialog>

#include "Entry.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AddEntryDialog; }
QT_END_NAMESPACE

class AddEntryDialog : QDialog
{
    Q_OBJECT

public:
    AddEntryDialog(QWidget *parent = nullptr);
    ~AddEntryDialog();
    bool showDialog(QString title);
    Entry getEntry();
    void setEntry(Entry entry);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AddEntryDialog *ui;
    Entry entry;
};

#endif // ADDENTRYDIALOG_H
