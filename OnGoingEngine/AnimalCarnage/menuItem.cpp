#include "menuItem.h"

MenuItem::MenuItem(std::string name, int id, int nrOfOptions)
{
	this->name = name;
	this->id = id;
	this->nrOfOptions = nrOfOptions;
	this->prev = -1;
	this->next = new int[nrOfOptions];
	for (int i = 0; i < nrOfOptions; i++)
	{
		this->next[i] = -1;
	}
}

MenuItem::~MenuItem()
{
	delete[] next;
}

void MenuItem::setName(std::string name)
{
	this->name = name;
}

void MenuItem::setID(int ID)
{
	this->id = ID;
}

void MenuItem::setPrev(int ID)
{
	this->prev = ID;
}

void MenuItem::setNext(int pos, int ID)
{
	this->next[pos] = ID;
}

std::string MenuItem::getName() const
{
	return this->name;
}

int MenuItem::getID() const
{
	return this->id;
}

int MenuItem::getNrOfOptions() const
{
	return nrOfOptions;
}

int MenuItem::getPrev() const
{
	return this->prev;
}

int MenuItem::getNext(int pos) const
{
	return this->next[pos];
}
