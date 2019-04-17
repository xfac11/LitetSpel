#ifndef MENUITEM_H
#define MENUITEM_H
#include <string>
class MenuItem
{
private:
	std::string name;
	int id;
	int nrOfOptions;
	int prev;
	int *next;
public:
	MenuItem(std::string name, int id, int nrOfOptions);
	~MenuItem();

	void setName(std::string name);
	void setID(int ID);
	void setPrev(int ID); //previous menu item
	void setNext(int pos, int ID); //next option on pos gets ID

	std::string getName()const;
	int getID()const;
	int getNrOfOptions()const;
	int getPrev()const;
	int getNext(int pos)const;
};
#endif