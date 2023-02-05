#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include <QDate>

inline QStringList statusText({ "Applied", "Got invited to 1st interview",
                                "Got invited to 2nd interview",
                                "Got invited to 3rd interview",
                                "Got a contract offer", "Got declined" });

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

inline QString convertLatestStatusToString(LatestStatus status)
{
    return statusText.value(static_cast<int>(status));
}

inline LatestStatus parseLatestStatusFromString(QString text)
{
    int index = statusText.indexOf(text);
    return static_cast<LatestStatus>((index >= 0) ? index : 0);
}

#endif // ENTRY_H
