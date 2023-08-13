#ifndef MULTITAG_H
#define MULTITAG_H

#include "basetag.h"

class Tag;

class MultiTag : public BaseTag
{
public:
    MultiTag();
    ~MultiTag();

    // SETTERS
    void setName(const QString& _name) { name = _name; }
    void setRequireFulfillment(const bool& _requireFulfillment) { requireFulfillment = _requireFulfillment; }
    void setHighestPriority(const bool& _priority) { highestPriority = _priority; }
    void setLinkedTags(const QVector<QPair<Tag*, unsigned int>>& _linkedTags) { linkedTags = _linkedTags; }
    void addLinkedTag(Tag* new_tag, const unsigned int& amount);
    void setLinkedTagRequestedMeals(Tag* assignedTag, const unsigned int& amount);
    bool removeLinkedTag(const Tag* tagPtr);
    void clearLinkedTags() { linkedTags.clear(); }

    // GETTERS
    bool getRequireFulfillment() const { return requireFulfillment; }
    bool getPriority() const { return highestPriority; }
    QVector<QPair<Tag*, unsigned int>> getLinkedTags() const { return linkedTags; }

private:
    bool highestPriority; // whether MultiTag should be prioritized above Tags or not
    bool requireFulfillment; // all tags linked must produce <requested meals> amount of meals, otherwise don't use multiTag

    // ints are used to indicate requested amount of Meals from its associated Tag
    QVector<QPair<Tag*, unsigned int>> linkedTags;
};

#endif // MULTITAG_H
