#ifndef __SELECT_HPP__
#define __SELECT_HPP__

#include <cstring>

using namespace std;
#include <iostream>


class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.
class Select_Column: public Select
{
protected:
    int column;
public:
    Select_Column(const Spreadsheet* sheet, const std::string& name)
    {
        column = sheet->get_column_by_name(name);
    }

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
};


class Select_And: public Select{
   protected:
        Select* sel1;
        Select* sel2;
   public:
	~Select_And() {
		delete sel1;
		delete sel2;
}

        Select_And(Select* s1, Select* s2) {
                sel1 = s1;
                sel2 = s2;
        }
        virtual bool select(const Spreadsheet* sheet, int row) const {
                return (sel1->select(sheet,row) && sel2->select(sheet,row));
        }
};

class Select_Or: public Select{
   protected:
        Select* sel1;
        Select* sel2;
   public:
	~Select_Or() {
	delete sel1;
	delete sel2;
	}

        Select_Or(Select* s1, Select* s2) {
                sel1 = s1;
                sel2 = s2;
        }
        virtual bool select(const Spreadsheet* sheet, int row) const {
                return (sel1->select(sheet,row) || sel2->select(sheet,row));
        }
};



class Select_Contains: public Select{
   protected:
        std::string name;
        int col;
   public:
        Select_Contains(const Spreadsheet* sheet, const std::string& c, const std::string& n) {
                col = sheet->get_column_by_name(c);
                name = n;
        }

        virtual bool select(const Spreadsheet* sheet, int row) const {
                std::string s = sheet->cell_data(row, col);
                if(strstr(s.c_str(),name.c_str())) {
                        return true;
                }
                return false;
        }
};

class Select_Not: public Select{
   protected:
        Select* sel;

   public:
	~Select_Not() {
		delete sel;
	} 

        Select_Not(Select* s) {
                sel = s;
        }

        virtual bool select(const Spreadsheet* sheet, int row) const {
                return !sel->select(sheet,row);
        }
};

#endif //__SELECT_HPP__
