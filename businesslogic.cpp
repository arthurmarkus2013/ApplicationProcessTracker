#include "businesslogic.h"

#include "hpdf.h"

BusinessLogic::BusinessLogic(QObject *parent) : QObject(parent)
{

}

int BusinessLogic::getItemCount()
{
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM Applications");

    query.first();

    return qvariant_cast<int>(query.value(0));
}

QList<EntryHelper::Entry> BusinessLogic::getItems()
{
    QList<EntryHelper::Entry> retVal;
    QSqlQuery query;
    query.exec("SELECT company_name, applied_on, latest_status, updated_on FROM Applications");

    while(query.next())
    {
        EntryHelper::Entry item;
        item.company_name = query.value(0).toString();
        item.applied_on = QDate::fromJulianDay(query.value(1).toLongLong());
        item.status = static_cast<EntryHelper::LatestStatus>(query.value(2).toInt());
        item.updated_on = QDate::fromJulianDay(query.value(3).toLongLong());
        retVal.append(item);
    }

    return retVal;
}

bool BusinessLogic::addItem(EntryHelper::Entry item)
{
    bool result = false;

    QSqlQuery query;
    if(query.prepare("INSERT INTO Applications (company_name, applied_on, latest_status, updated_on) "
                  "VALUES (:company_name, :applied_on, :latest_status, :updated_on)"))
    {
        query.bindValue(":company_name", item.company_name);
        query.bindValue(":applied_on", item.applied_on.toJulianDay());
        query.bindValue(":latest_status", static_cast<int>(item.status));
        query.bindValue(":updated_on", item.updated_on.toJulianDay());
        result = query.exec();
    }

    return result;
}

bool BusinessLogic::updateItem(EntryHelper::Entry oldItem, EntryHelper::Entry newItem)
{
    bool result = false;

    QSqlQuery query;
    if(query.prepare("UPDATE Applications SET company_name = :new_company_name, applied_on = :new_applied_on, "
                     "latest_status = :new_latest_status, updated_on = :new_updated_on "
                     "WHERE company_name = :old_company_name AND applied_on = :old_applied_on "
                     "AND latest_status = :old_latest_status AND updated_on = :old_updated_on"))
    {
        query.bindValue(":new_company_name", newItem.company_name);
        query.bindValue(":new_applied_on", newItem.applied_on.toJulianDay());
        query.bindValue(":new_latest_status", static_cast<int>(newItem.status));
        query.bindValue(":new_updated_on", newItem.updated_on.toJulianDay());

        query.bindValue(":old_company_name", oldItem.company_name);
        query.bindValue(":old_applied_on", oldItem.applied_on.toJulianDay());
        query.bindValue(":old_latest_status", static_cast<int>(oldItem.status));
        query.bindValue(":old_updated_on", oldItem.updated_on.toJulianDay());

        result = query.exec();
    }

    return result;
}

bool BusinessLogic::removeItem(EntryHelper::Entry item)
{
    bool result = false;

    QSqlQuery query;
    if(query.prepare("DELETE FROM Applications WHERE company_name = :company_name AND applied_on = :applied_on "
                     "AND latest_status = :latest_status AND updated_on = :updated_on"))
    {
        query.bindValue(":company_name", item.company_name);
        query.bindValue(":applied_on", item.applied_on.toJulianDay());
        query.bindValue(":latest_status", static_cast<int>(item.status));
        query.bindValue(":updated_on", item.updated_on.toJulianDay());

        result = query.exec();
    }

    return result;
}

void pdf_error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
    auto msg = BusinessLogic::tr("ERROR") + ": error_no = " + QString::number((unsigned int) error_no)
            + " detail_no = " + QString::number((int) detail_no);
    throw std::exception(msg.toUtf8()); /* throw exception on error */
}

QStringList BusinessLogic::prepareDataForExport()
{
    QStringList retVal;

    auto items = getItems();
    QRegularExpression re("[ ]+");

    for(auto &item : items)
    {
        auto entry = item.company_name + " : " + tr("Applied on") + " " + QLocale().toString(item.applied_on);

        if(item.status != EntryHelper::LatestStatus::Applied)
        {
            entry += " - " + EntryHelper().convertLatestStatusToString(item.status) + " " + tr("on") + " " + QLocale().toString(item.updated_on);
        }

        retVal.append(entry.replace(re, " "));
    }

    return retVal;
}

bool BusinessLogic::exportAsPDF(QString path, QWidget *parent)
{
    HPDF_Doc pdf = HPDF_New(pdf_error_handler, nullptr);

    if (!pdf) {
        QMessageBox::critical(parent, tr("Fatal Error"), tr("ERROR: Can't create PDF object"));
        return false;
    }

    int font_size = 8;
    int page_margin = 30;
    float line_spacing = 1.3f;

    try
    {
        HPDF_UseUTFEncodings(pdf);
        HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);
        HPDF_SetPageMode(pdf, HPDF_PAGE_MODE_USE_NONE);
        HPDF_SetCurrentEncoder(pdf, "UTF-8");
        HPDF_SetInfoAttr(pdf, HPDF_INFO_TITLE, tr("My Applications").toUtf8());

        HPDF_Date info {0};
        auto now = QDateTime::currentDateTime();
        info.seconds = now.time().second();
        info.minutes = now.time().minute();
        info.hour = now.time().hour();
        info.day = now.date().day();
        info.month = now.date().month();
        info.year = now.date().year();
        info.ind = 'Z';
        HPDF_SetInfoDateAttr(pdf, HPDF_INFO_CREATION_DATE, info);

        HPDF_Font font = HPDF_GetFont(pdf, HPDF_LoadTTFontFromFile(
                                          pdf, QStandardPaths::locate(QStandardPaths::FontsLocation, "Arial.ttf").toUtf8(), true), "UTF-8");
        auto items = prepareDataForExport();
        HPDF_Page page = HPDF_AddPage(pdf);
        HPDF_Page_SetFontAndSize(page, font, font_size);
        HPDF_Page_BeginText(page);
        HPDF_Page_SetTextLeading(page, font_size * line_spacing);
        int num_lines = 0;

        for(auto &item : items)
        {
            float pos = font_size * num_lines * line_spacing;

            if((HPDF_Page_GetHeight(page) - pos) < page_margin)
            {
                HPDF_Page_EndText(page);
                page = HPDF_AddPage(pdf);
                HPDF_Page_SetFontAndSize(page, font, font_size);
                HPDF_Page_BeginText(page);
                HPDF_Page_SetTextLeading(page, font_size * line_spacing);
            }

            HPDF_Page_TextRect(page, page_margin, HPDF_Page_GetHeight(page) - (page_margin + pos + font_size),
                               HPDF_Page_GetWidth(page) - page_margin, page_margin,
                               item.toUtf8(), HPDF_TALIGN_LEFT, nullptr);

            num_lines += std::ceil(HPDF_Page_TextWidth(page, item.toUtf8()) / (HPDF_Page_GetWidth(page) - (page_margin * 2)));
        }

        HPDF_Page_EndText(page);

        HPDF_SaveToFile(pdf, path.toUtf8());
    }
    catch(std::exception ex)
    {
        QMessageBox::critical(parent, BusinessLogic::tr("Failed To Export PDF"), ex.what());

        HPDF_Free(pdf);
        return false;
    }

    HPDF_Free(pdf);

    return true;
}
