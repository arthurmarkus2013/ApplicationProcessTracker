#include "EntryHelper.h"

EntryHelper::EntryHelper(QObject *parent) : QObject(parent)
{
    statusText = QStringList({ tr("Applied"),
                                    tr("Got invited to 1st interview"),
                                    tr("Got invited to 2nd interview"),
                                    tr("Got invited to 3rd interview"),
                                    tr("Got a contract"),
                                    tr("Got declined") });
}

EntryHelper::~EntryHelper()
{
    //
}

QString EntryHelper::convertLatestStatusToString(LatestStatus status)
{
    return statusText.value(static_cast<int>(status));
}

EntryHelper::LatestStatus EntryHelper::parseLatestStatusFromString(QString text)
{
    int index = statusText.indexOf(text);
    return static_cast<LatestStatus>((index >= 0) ? index : 0);
}
