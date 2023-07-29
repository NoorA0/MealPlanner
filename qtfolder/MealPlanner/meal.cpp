#include "meal.h"

Meal::Meal()
{
    name = "UNSET";
    price = 0.0;
    disabled = true;
    mealDuration = 1;
    daysBetweenOccurrences = 0;
}

Meal::~Meal()
{
    for (auto tagIter : tags)
    {
        delete tagIter;
    }
}

void Meal::setName(const QString& new_name)
{
    name = new_name;
}

void Meal::setPrice(const double& new_price)
{
    price = new_price;
}

void Meal::setTags(const QVector<Tag*>& new_tags)
{
    tags = new_tags;
}

void Meal::addTag(Tag* newTag)
{
    tags.push_back(newTag);
}

bool Meal::removeTag(const Tag* tagPtr)
{
    bool notFound = true;

    // find mealPtr
    auto tagIter = tags.begin();
    while (notFound && tagIter != tags.end())
    {
        // if found, erase from vector
        if (*tagIter == tagPtr)
        {
            notFound = false;
            tags.erase(tagIter);
        }
        else
            ++tagIter;
    }

    // if nothing found, return error value
    return notFound;
}

void Meal::clearTags()
{
    tags.clear();
}

void Meal::setDisabled(const bool& disabled)
{
    this->disabled = disabled;
}

void Meal::setMealDuration(const unsigned int& mealDuration)
{
    this->mealDuration = mealDuration;
}

void Meal::setDaysBetweenOccurrences(const unsigned int& daysBetweenOccurrences)
{
    this->daysBetweenOccurrences = daysBetweenOccurrences;
}

void Meal::setDaysScheduled(const QVector<unsigned int>& daysScheduled)
{
    this->daysScheduled = daysScheduled;
}

QString Meal::getName() const
{
    return name;
}

double Meal::getPrice() const
{
    return price;
}

QVector<Tag*> Meal::getTags() const
{
    return tags;
}

bool Meal::isDisabled() const
{
    return disabled;
}

unsigned int Meal::getMealDuration() const
{
    return mealDuration;
}

unsigned int Meal::getDaysBetweenOccurrences() const
{
    return daysBetweenOccurrences;
}

QVector<unsigned int> Meal::getDaysScheduled() const
{
    return daysScheduled;
}

QMap<DaysOfTheWeek, bool> Meal::getEnabledDays() const
{
    // default to always enabled
    QMap<DaysOfTheWeek, bool> enabledDays =
        { {MONDAY, true}, {TUESDAY, true}, {WEDNESDAY, true}, {THURSDAY, true},
         {FRIDAY, true}, {SATURDAY, true}, {SUNDAY, true} };

    // check every tag applied
    for (auto tagIter : tags)
    {
        QMap<DaysOfTheWeek, bool> tagDays = tagIter->getEnabledDays();

        // if tag's day is disabled, then disable day
        if (!tagDays.contains(MONDAY))
            enabledDays[MONDAY] = false;
        if (!tagDays.contains(TUESDAY))
            enabledDays[TUESDAY] = false;
        if (!tagDays.contains(WEDNESDAY))
            enabledDays[WEDNESDAY] = false;
        if (!tagDays.contains(THURSDAY))
            enabledDays[THURSDAY] = false;
        if (!tagDays.contains(FRIDAY))
            enabledDays[FRIDAY] = false;
        if (!tagDays.contains(SATURDAY))
            enabledDays[SATURDAY] = false;
        if (!tagDays.contains(SUNDAY))
            enabledDays[SUNDAY] = false;
    }

    return enabledDays;
}
