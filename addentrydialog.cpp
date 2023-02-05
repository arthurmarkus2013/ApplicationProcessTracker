#include "addentrydialog.h"
#include "./ui_addentrydialog.h"

AddEntryDialog::AddEntryDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddEntryDialog)
{
    ui->setupUi(this);
    ui->latestStatus->addItems(statusText);
    ui->appliedOnDate->setDate(QDate::currentDate());
}

AddEntryDialog::~AddEntryDialog()
{
    delete ui;
}

bool AddEntryDialog::showDialog(QString title)
{
    this->setWindowTitle(title);
    return this->exec() == QDialog::Accepted;
}

Entry AddEntryDialog::getEntry()
{
    return entry;
}

void AddEntryDialog::setEntry(Entry entry)
{
    ui->companyName->setText(entry.company_name);
    ui->appliedOnDate->setDate(entry.applied_on);
    ui->latestStatus->setCurrentIndex(static_cast<int>(entry.status));

    this->entry = entry;
}

void AddEntryDialog::on_buttonBox_accepted()
{
    entry.company_name = ui->companyName->text();
    entry.applied_on = ui->appliedOnDate->date();
    entry.status = static_cast<LatestStatus>(ui->latestStatus->currentIndex());
    entry.updated_on = QDate::currentDate();

    this->accept();
}


void AddEntryDialog::on_buttonBox_rejected()
{
    this->reject();
}

