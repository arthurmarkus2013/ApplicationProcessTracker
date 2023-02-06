#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "addentrydialog.h"

void MainWindow::resetTable()
{
    ui->applicationsTable->clear();
    ui->applicationsTable->setShowGrid(true);
    ui->applicationsTable->setColumnCount(columnCount);
    ui->applicationsTable->setColumnWidth(0, (this->width() / columnCount) - 10);
    ui->applicationsTable->setColumnWidth(1, (this->width() / columnCount) - 10);
    ui->applicationsTable->setColumnWidth(2, (this->width() / columnCount) - 10);
    ui->applicationsTable->setColumnWidth(3, (this->width() / columnCount) - 10);
    ui->applicationsTable->setHorizontalHeaderLabels(QStringList({ tr("Company Name"), tr("Applied On"), tr("Latest Status"), tr("Updated On") }));
    ui->applicationsTable->verticalHeader()->hide();
    ui->applicationsTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->applicationsTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->applicationsTable->horizontalHeader()->setStretchLastSection(true);
    ui->applicationsTable->setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindow::populateTable()
{
    auto items = logic.getItems();
    int count = logic.getItemCount();
    ui->applicationsTable->setRowCount(count);
    for(int i = 0; i < count; i++)
    {
        auto item = new QTableWidgetItem(items[i].company_name);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->applicationsTable->setItem(i, 0, item);

        item = new QTableWidgetItem(items[i].applied_on.toString());
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->applicationsTable->setItem(i, 1, item);

        item = new QTableWidgetItem(convertLatestStatusToString(items[i].status));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->applicationsTable->setItem(i, 2, item);

        item = new QTableWidgetItem(items[i].updated_on.toString());
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->applicationsTable->setItem(i, 3, item);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->searchTerm->setPlaceholderText(tr("Search..."));

    resetTable();
    populateTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnAddEntry_clicked()
{
    AddEntryDialog dialog(this);
    if(dialog.showDialog(tr("Add Entry")))
        if(logic.addItem(dialog.getEntry()))
        {
            resetTable();
            populateTable();
        }
}


void MainWindow::on_btnExport_clicked()
{
    QString file_path = QFileDialog::getSaveFileName(this, tr("Export To..."), QDir::homePath() + QDir::separator() + "Documents",
                                                     tr("Portable Document Format (*.pdf);;All Files (*.*)"));

    if(!file_path.isEmpty())
        logic.exportAsPDF(file_path, this);
}


void MainWindow::on_searchTerm_textChanged(const QString&)
{
   auto matches = ui->applicationsTable->findItems(ui->searchTerm->text(), Qt::MatchFlag::MatchContains);
    int count = ui->applicationsTable->rowCount();

    for(int i = 0; i < count; i++)
    {
        ui->applicationsTable->setRowHidden(i, true);
    }

    for(auto &match : matches)
    {
        ui->applicationsTable->setRowHidden(match->row(), false);
    }
}


void MainWindow::on_applicationsTable_customContextMenuRequested(const QPoint &pos)
{
    QMenu ctxMenu(this);

    auto edit = std::make_unique<QAction>(ctxMenu.addAction(tr("Edit")));
    connect(edit.get(), &QAction::triggered, this, [&](bool){
        auto items = ui->applicationsTable->selectedItems();

        Entry entry;
        entry.company_name = items.value(0)->text();
        entry.applied_on = QDate::fromString(items.value(1)->text());
        entry.status = parseLatestStatusFromString(items.value(2)->text());
        entry.updated_on = QDate::fromString(items.value(3)->text());

        AddEntryDialog dialog(this);
        dialog.setEntry(entry);

        if(dialog.showDialog(tr("Edit Entry")))
            if(logic.updateItem(entry, dialog.getEntry()))
            {
                resetTable();
                populateTable();
            }
    });

    auto del = std::make_unique<QAction>(ctxMenu.addAction(tr("Delete")));
    connect(del.get(), &QAction::triggered, this, [&](bool){
        if(QMessageBox::question(this, tr("Delete Row"), tr("Do you really wanna delete this row?")) == QMessageBox::StandardButton::Yes)
        {
            auto items = ui->applicationsTable->selectedItems();
            Entry entry;

            entry.company_name = items[0]->text();
            entry.applied_on = QDate::fromString(items[1]->text());
            entry.status = parseLatestStatusFromString(items[2]->text());
            entry.updated_on = QDate::fromString(items[3]->text());

            if(logic.removeItem(entry))
            {
                resetTable();
                populateTable();
            }
        }
    });

    ctxMenu.exec(ui->applicationsTable->viewport()->mapToGlobal(pos));
}

