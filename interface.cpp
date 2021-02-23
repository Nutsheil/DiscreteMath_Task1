#include "interface.h"

#include "set.h"

#include <iostream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

enum myenum{
    intersec, uni, diff, symmDiff
};

list<string> split(const string &str)
{
    list<string> elems;
    string item;

    auto i = str.begin();

    for (; i!= str.end(); ++i)
        if (*i < 0)
            throw -2;

    i = str.begin();

    for (; (i != str.end()) && isspace(*i); ++i);

    for (; (i != str.end()) && !isspace(*i); ++i)
        item.push_back(*i);

    if (item.size() != 0)
        elems.push_back(item);
    item.clear();

    for (; (i != str.end()) && isspace(*i); ++i);

    while (i != str.end())
    {
        for (; (i != str.end()) && isspace(*i); ++i);

        if (i == str.end())
            return elems;
        if (*i != '\"')
            throw -1;

        ++i;

        for (; (i != str.end()) && (*i != '\"'); ++i)
            item.push_back(*i);

        if (i == str.end())
            throw -1;
        if (*i != '\"')
            throw -1;

        ++i;

        elems.push_back(item);
        item.clear();
    }

    return elems;
}

auto GetIteratorOfSet(list<set>& sets, const string& name)
{
    auto i = sets.begin();
    for (; i != sets.end() && i->GetName() != name; ++i);

    if (i != sets.end() && i->GetName() == name)
        return i;
    else
        return sets.end();
}

void SetOperation(list<set>& sets, const list<string>& elems, myenum type)
{
    auto a = elems.begin();
    const string setname_a = *a++;
    const string setname_b = *a++;
    const string setname_c = *a;

    auto i = GetIteratorOfSet(sets, setname_a);
    auto j = GetIteratorOfSet(sets, setname_b);
    auto k = GetIteratorOfSet(sets, setname_c);

    if (i == sets.end())
        cout << "Set \"" + setname_a + "\" does not exists" << endl;
    if (j == sets.end())
        cout << "Set \"" + setname_b + "\" does not exists" << endl;


    if (i != sets.end() && j != sets.end())
    {
        if (k != sets.end())
            sets.erase(k);

        auto aaa = sets.begin();
        for (; aaa != sets.end() && aaa->GetName() < setname_c; ++aaa);

        if (aaa == sets.end())
            aaa--;

        switch (type)
        {
        case intersec:
            sets.insert(aaa, i->Intersection(*j, setname_c));
            cout << "\"" + setname_c + "\" = \"" + setname_a + "\" /\\ \"" + setname_b + "\"" << endl;
            break;
        case uni:
            sets.insert(aaa, i->Union(*j, setname_c));
            cout << "\"" + setname_c + "\" = \"" + setname_a + "\" \\/ \"" + setname_b + "\"" << endl;
            break;
        case diff:
            sets.insert(aaa, i->Subtract(*j, setname_c));
            cout << "\"" + setname_c + "\" = \"" + setname_a + "\" \\ \"" + setname_b + "\"" << endl;
            break;
        case symmDiff:
            sets.insert(aaa, i->SymmetricDifference(*j, setname_c));
            cout << "\"" + setname_c + "\" = \"" + setname_a + "\" delta \"" + setname_b + "\"" << endl;
            break;
        }
    }
}

void Create(list<set>& sets, const list<string>& elems)
{
    const string setname = *elems.begin();

    auto i = sets.begin();
    for (; (i != sets.end()) && (i->GetName() < setname); ++i);

    if (i != sets.end() && i->GetName() == setname)
        cout << "Set \"" + setname +"\" already exists" << endl;
    else
    {
        set tmp(setname);

        if (i == sets.end())
            sets.push_back(tmp);
        else
            sets.insert(i, tmp);
        cout << "Set \"" + setname + "\" created" << endl;
    }
}

void Delete(list<set>& sets, const list<string>& elems)
{
    const string setname = *elems.begin();

    auto i = GetIteratorOfSet(sets, setname);

    if (i == sets.end())
        cout << "Set \"" + setname + "\" does not exist" << endl;
    else
    {
        sets.erase(i);
        cout << "Set \"" + setname + "\" deleted" << endl;
    }
}

void PrintSets(list<set>& sets)
{
    for (auto i = sets.begin(); i != sets.end(); ++i)
        cout << "\"" << i->GetName() << "\"" << endl;

    if (sets.size() == 0)
        cout << "No sets" << endl;
}

void Add(list<set>& sets, const list<string>& elems)
{
    auto a = elems.begin();
    const string setname = *a++;
    const string element = *a;

    if (element.size() > 80)
    {
        cout << "Element is too large" <<endl;
        return;
    }

    auto i = GetIteratorOfSet(sets, setname);

    if (i == sets.end())
        cout << "Set \"" + setname + "\" does not exist" << endl;
    else if (i->Check(element))
        cout << "Element \"" + element + "\" already exists in set \"" + setname + "\"" << endl;
    else
    {
        i->Add(element);
        cout << "Element \"" + element + "\" added to set \"" + setname + "\"" << endl;
    }
}

void Remove(list<set>& sets, const list<string>& elems)
{
    auto a = elems.begin();
    const string setname = *a++;
    const string element = *a;

    auto i = GetIteratorOfSet(sets, setname);

    if (i == sets.end())
        cout << "Set \"" + setname + "\" does not exist" << endl;
    else if (!(i->Check(element)))
        cout << "Element \"" + element + "\" already exists in set \"" + setname + "\"" << endl;
    else
    {
        i->Remove(element);
        cout << "Element \"" + element + "\" removed from set \"" + setname + "\"" << endl;
    }
}

void Check(list<set>& sets, const list<string>& elems)
{
    auto a = elems.begin();
    const string setname = *a++;
    const string element = *a;

    auto i = GetIteratorOfSet(sets, setname);

    if (i == sets.end())
        cout << "Set \"" + setname + "\" does not exist" << endl;
    else if (i->Check(element))
        cout << "This element exists in set \"" + setname + "\"" << endl;
    else
        cout << "This element does not exists in set \"" + setname + "\"" << endl;
}

void Cardinality(list<set>& sets, const list<string>& elems)
{
    const string setname = *elems.begin();

    auto i = GetIteratorOfSet(sets, setname);

    if (i == sets.end())
        cout << "Set \"" + setname + "\" does not exist" << endl;
    else
        cout << "Cardinality of set \"" + setname + "\" is "<< i->GetCardinalis() << endl;
}

void Print(list<set>& sets, const list<string>& elems)
{
    const string setname = *elems.begin();

    auto i = GetIteratorOfSet(sets, setname);

    if (i == sets.end())
        cout << "Set \"" + setname + "\" does not exist" << endl;
    else
        i->PrintAll();
}

void CheckSubset(list<set>& sets, const list<string>& elems)
{
    auto a = elems.begin();
    const string setname_a = *a++;
    const string setname_b = *a;

    auto i = GetIteratorOfSet(sets, setname_a);
    auto j = GetIteratorOfSet(sets, setname_b);

    if (i == sets.end())
        cout << "Set \"" + setname_a + "\" does not exists" << endl;
    if (j == sets.end())
        cout << "Set \"" + setname_b + "\" does not exists" << endl;

    if (i != sets.end() && j != sets.end())
    {
        if (i->CheckSubset(*j))
            cout << "Set \"" + setname_b + "\" is subset of set \"" + setname_a + "\"" << endl;
        else
            cout << "Set \"" + setname_b + "\" isn't subset of set \"" + setname_a + "\"" << endl;
    }
}

void Help()
{
    ifstream file;
    file.open(".\\interface.txt");

    if (!file)
    {
        cout << "File not found" << endl;
        return;
    }

    string s;
    cout << "----------HELP----------" << endl;
    while (!file.eof())
    {
        getline(file, s);
        cout << s << endl;
    }

    file.close();
}

void Dialog()
{
    cout << "Hello!" << endl;
    cout << "Use \"Help\" for more information" << endl;
    cout << "----------" << endl;

    string s;
    list<set> sets;

    while(true)
    {
        getline(cin, s);

        list<string> elems;

        try {
            elems = split(s);
        } catch (int n)
        {
            if (n == -1)
                cout << "Wrong input (Check quotes)" << endl;
            if (n == -2)
                cout << "Wrong input (Use only English letters)" << endl;
            cout << "----------" << endl;
            continue;
        }

        if (elems.size() == 0)
        {
            cout << "Nothing input" << endl;
            cout << "----------" << endl;
            continue;
        }

        string command = *elems.begin();
        elems.pop_front();

        if (command == "Create") //------------------------------------------------------------------------------
        {
            if (elems.size() != 1)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                Create(sets, elems);
        }
        else if (command == "Delete") //------------------------------------------------------------------------------
        {
            if (elems.size() != 1)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                Delete(sets, elems);
        }
        else if (command == "Sets") //------------------------------------------------------------------------------
        {
            if (elems.size() != 0)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                PrintSets(sets);
        }
        else if (command == "Add") //------------------------------------------------------------------------------
        {
            if (elems.size() != 2)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                Add(sets, elems);
        }
        else if (command == "Remove") //------------------------------------------------------------------------------
        {
            if (elems.size() != 2)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                Remove(sets, elems);
        }
        else if (command == "Check") //------------------------------------------------------------------------------
        {
            if (elems.size() != 2)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                Check(sets, elems);
        }
        else if (command == "Cardinality") //------------------------------------------------------------------------------
        {
            if (elems.size() != 1)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                Cardinality(sets, elems);
        }
        else if (command == "Print") //------------------------------------------------------------------------------
        {
            if (elems.size() != 1)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                Print(sets, elems);
        }
        else if (command == "Intersection") //------------------------------------------------------------------------------
        {
            if (elems.size() != 3)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                SetOperation(sets, elems, intersec);
        }
        else if (command == "Union") //------------------------------------------------------------------------------
        {
            if (elems.size() != 3)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                SetOperation(sets, elems, uni);
        }
        else if (command == "Diff") //------------------------------------------------------------------------------
        {
            if (elems.size() != 3)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                SetOperation(sets, elems, diff);
        }
        else if (command == "SymmDiff") //------------------------------------------------------------------------------
        {
            if (elems.size() != 3)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                SetOperation(sets, elems, symmDiff);
        }
        else if (command == "CheckSubset") //------------------------------------------------------------------------------
        {
            if (elems.size() != 2)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                CheckSubset(sets, elems);
        }
        else if (command == "Exit") //------------------------------------------------------------------------------
        {
            if (elems.size() != 0)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                return;
        }
        else if (command == "Help") //------------------------------------------------------------------------------
        {
            if (elems.size() != 0)
                cout << "Wrong input (wrong number of arguments)" << endl;
            else
                Help();
        }
        else
            cout << "There is no such command" << endl;

        cout << "----------" << endl;
    }

    cout << "----------" << endl;
    cout << "Bye my friend" << endl;
    cout << "Have a nice day and be happy!!" << endl;
}
