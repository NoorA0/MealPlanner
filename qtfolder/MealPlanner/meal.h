#ifndef MEAL_H
#define MEAL_H

#include "tag.h"

class Meal
{
public:
    Meal();
    ~Meal();

    // SETTERS
    void setName(const QString& new_name);
    void setPrice(const double& new_price);
    void setTags(const QVector<Tag*>& new_tags);
    void addTag(Tag* newTag);
    bool removeTag(const Tag* tagPtr);
    void clearTags();
    void setDisabled(const bool& disabled);
    void setMealDuration(const unsigned int& mealDuration);
    void setDaysBetweenOccurrences(const unsigned int& daysBetweenOccurrences);
    void addDayScheduled(const unsigned int& scheduledDay) { daysScheduled.push_back(scheduledDay); }
    void setDaysScheduled(const QVector<unsigned int>& daysScheduled);

    // GETTERS
    QString getName() const;
    double getPrice() const;
    QVector<Tag*> getTags() const;
    bool isDisabled() const;
    unsigned int getMealDuration() const;
    unsigned int getDaysBetweenOccurrences() const;
    QVector<unsigned int> getDaysScheduled() const;

    // OTHER FUNCTIONS
    QMap<DaysOfTheWeek, bool> getEnabledDays() const;

private:
    QString name;
    QVector<Tag*> tags; // tags that food is a part of
    double price; // how much it costs to make
    bool disabled; // user can disable meals
    unsigned int mealDuration; // how long this meal lasts in days (useful for large batches)
    unsigned int daysBetweenOccurrences; // how many days before this meal can be rescheduled
    QVector<unsigned int> daysScheduled; // logs each number of the day this meal was last scheduled
};

#endif // MEAL_H
