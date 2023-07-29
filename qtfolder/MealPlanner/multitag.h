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
    void setLinkedTags(const QMap<Tag*, unsigned int>& _linkedTags) { linkedTags = _linkedTags; }
    void addLinkedTag(Tag* new_tag, const unsigned int& amount);
    bool removeLinkedTag(const Tag* tagPtr);
    void clearLinkedTags() { linkedTags.clear(); }

    // GETTERS
    bool getRequireFulfillment() const { return requireFulfillment; }
    bool getPriority() const { return highestPriority; }
    QMap<Tag*, unsigned int> getLinkedTags() const { return linkedTags; }

private:
    bool highestPriority; // whether MultiTag should be prioritized above Tags or not
    bool requireFulfillment; // all tags linked must produce <requested meals> amount of meals, otherwise don't use multiTag

    // ints are used to indicate requested amount of Meals of Tag
    // requesting 2 Sandwiches (tags) would mean the Tag called Sandwiches is loaded into linkedTags with an int of 2
    QMap<Tag*, unsigned int> linkedTags;
};

#endif // MULTITAG_H
