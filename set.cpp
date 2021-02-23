#include "set.h"
#include <algorithm>

set::set(const string& name) : name_(name), elements()
{}

set::set(const set &b) : name_(b.name_), elements(b.elements)
{}

set& set::operator=(const set &b)
{
    if (this != &b)
    {
        name_ = b.name_;
        elements = b.elements;
    }
    return *this;
}

string set::GetName() const
{
    return name_;
}

int set::GetCardinalis() const
{
    return elements.size();
}

bool set::Check(const string& elem) const
{
    return std::find(elements.begin(), elements.end(), elem) != elements.end();
}

void set::Add(const string& elem)
{   
    auto i = elements.begin();
    for (; (i != elements.end()) && (*i < elem); ++i);

    if (i == elements.end())
    {
        elements.push_back(elem);
        return;
    }

    if (*i == elem)
        return;

    elements.insert(i,elem);
    return;
}

void set::Remove(const string& elem)
{
    elements.remove(elem);
}

void set::PrintAll() const
{
    if (elements.size() == 0)
    {
        cout << "--it's empty--" << endl;
        return;
    }
    for (auto i = elements.begin(); i != elements.end(); ++i)
        cout << "\"" << *i << "\"  -  " << (*i).size() << endl;
}

set set::Intersection(const set &b, const string &name) const
{
    set c(name);

    auto i = this->elements.begin();
    auto j = b.elements.begin();

    while (i != this->elements.end() && j != b.elements.end())
    {
        if (*i < *j)
            i++;
        else if (*i > *j)
            j++;
        else
        {
            c.elements.push_back(*i);
            i++;
            j++;
        }
    }

    return c;
}

set set::Union(const set &b, const string &name) const
{
    set c(name);

    auto i = this->elements.begin();
    auto j = b.elements.begin();

    while (i != this->elements.end() && j != b.elements.end())
    {
        if (*i < *j)
        {
            c.elements.push_back(*i);
            i++;
        }
        else if (*i > *j)
        {
            c.elements.push_back(*j);
            j++;
        }
        else
        {
            c.elements.push_back(*i);
            i++;
            j++;
        }
    }

    for(; i != this->elements.end(); ++i)
        c.elements.push_back(*i);

    for(; j != b.elements.end(); ++j)
        c.elements.push_back(*j);


    return c;
}

set set::Subtract(const set &b, const string &name) const
{
    set c(name);

    auto i = this->elements.begin();
    auto j = b.elements.begin();

    while (i != this->elements.end() && j != b.elements.end())
    {
        if (*i < *j)
        {
            c.elements.push_back(*i);
            i++;
        }
        else if (*i > *j)
        {
            j++;
        }
        else
        {
            i++;
            j++;
        }
    }

    for(; i != this->elements.end(); ++i)
        c.elements.push_back(*i);


    return c;
}

set set::SymmetricDifference(const set &b, const string &name) const
{
    set c(name);

    auto i = this->elements.begin();
    auto j = b.elements.begin();

    while (i != this->elements.end() && j != b.elements.end())
    {
        if (*i < *j)
        {
            c.elements.push_back(*i);
            i++;
        }
        else if (*i > *j)
        {
            c.elements.push_back(*j);
            j++;
        }
        else
        {
            i++;
            j++;
        }
    }

    for(; i != this->elements.end(); ++i)
        c.elements.push_back(*i);

    for(; j != b.elements.end(); ++j)
        c.elements.push_back(*j);


    return c;
}

bool set::CheckSubset(const set &b) const
{
    auto i = this->elements.begin();
    auto j = b.elements.begin();

    while (i != this->elements.end() && j != b.elements.end())
    {
        if (*i < *j)
            i++;
        else if (*i > *j)
            return false;
        else
        {
            i++;
            j++;
        }
    }

    if (j != b.elements.end())
        return false;

    return true;
}
