#ifndef TAG_H
#define TAG_H

#include "basetag.h"
#include <QVector>

class Meal;

class Tag : public BaseTag
{
public:
    Tag();
    ~Tag();

    // SETTERS
    void setName(const QString& _name) { name = _name; }
    void setDependency(const bool& _dependency) { dependsOnMultiTag = _dependency; }
    void setConsecutiveLimit(const unsigned int& _consecutiveLimit) { consecutiveLimit = _consecutiveLimit; }
    void addMeal(Meal* mealPtr) { linkedMeals.push_back(mealPtr); }
    bool removeMeal(const Meal* mealPtr);
    void clearLinkedMeals() { linkedMeals.clear(); }

    // GETTERS
    bool getDependency() const { return dependsOnMultiTag; }
    unsigned int getConsecutiveLimit() const { return consecutiveLimit; }
    QVector<Meal*> getLinkedMeals() const { return linkedMeals; }

private:
    QVector<Meal*> linkedMeals; // meals that are linked to this tag
    bool dependsOnMultiTag; // if tag is only used when a MultiTag uses it
    unsigned int consecutiveLimit; // times a meal with a tag can occurr consecutively
};

#endif // TAG_H
