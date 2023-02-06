#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QItemEditorFactory>
#include <QDateEdit>
#include <QMenu>
#include <QMessageBox>
#include <QShortcut>

#include "businesslogic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAddEntry_clicked();

    void on_btnExport_clicked();

    void on_searchTerm_textChanged(const QString&);

    void on_applicationsTable_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    int columnCount = 4;
    BusinessLogic logic;
    void resetTable();
    void populateTable();
};
#endif // MAINWINDOW_H
