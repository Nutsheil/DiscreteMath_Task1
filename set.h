#ifndef SETS_H
#define SETS_H

#include <iostream>
#include <string>
#include <list>

using namespace std;

class set
{
public:
    set(const string& name);
    set(const set& b);
    set& operator=(const set& b);
    string GetName() const;
    int GetCardinalis() const;
    bool Check(const string& elem) const;
    void Add(const string& elem);
    void Remove(const string& elem);
    void PrintAll() const;
    set Intersection(const set& b, const string& name) const;
    set Union(const set& b, const string& name) const;
    set Subtract(const set& b, const string& name) const;
    set SymmetricDifference(const set& b, const string& name) const;
    bool CheckSubset(const set& b) const;

private:
    string name_;
    list<string> elements;
};

#endif // SETS_H
