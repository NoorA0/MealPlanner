#ifndef BASETAG_H
#define BASETAG_H

#include <QString>
#include <QMap>

enum DaysOfTheWeek
{
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
};

class BaseTag
{
public:
    virtual ~BaseTag() {};

    // SETTERS
    virtual void setName(const QString& _name) = 0;
    void setDescription(const QString& _description) { description = _description; }
    void setEnabledDays(const QMap<DaysOfTheWeek, bool>& _enabledDays) { enabledDays = _enabledDays; }

    // GETTERS
    QString getName() const { return name; }
    QString getDescription() const { return description; }
    QMap<DaysOfTheWeek, bool> getEnabledDays() const { return enabledDays; }
    bool isDisabled() const;

protected:
    QString name;
    QString description;
    QMap<DaysOfTheWeek, bool> enabledDays;
};

#endif // BASETAG_H
