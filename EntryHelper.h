#ifndef ENTRYHELPER_H
#define ENTRYHELPER_H

#include <QString>
#include <QDate>

class EntryHelper : QObject
{
    Q_OBJECT

public:
    EntryHelper(QObject *parent = nullptr);
    ~EntryHelper();

    QStringList statusText;

    enum class LatestStatus
    {
        Applied,
        InvitedToFirstInterview,
        InvitedToSecondInterview,
        InvitedToThirdInterview,
        GotContractOffer,
        GotDeclined
    };

    struct Entry
    {
        QString company_name;
        QDate applied_on;
        LatestStatus status;
        QDate updated_on;
    };

    QString convertLatestStatusToString(LatestStatus status);

    LatestStatus parseLatestStatusFromString(QString text);
};

#endif // ENTRYHELPER_H
