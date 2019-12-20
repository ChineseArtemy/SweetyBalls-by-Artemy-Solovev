#include <iostream>
#ifndef listlib
#define listlib

class Note;
class List;

#endif



class Note
{
	public:
		int value;
		Note* next;
};
		

class List
{
	public:
		Note* head;

		List();
		~List();
		void push_front(int val);
		int pop_front();
		void clear();
		int len();
		void push_back(int val);
		int  pop_back();
		void erase_index (int index);
		void erase_element (int key);
		void insert (int index, int val);
		bool find(int key);
		int& operator[](int index);
		List& operator=(List& sample);
};

List::List()
{
	head = nullptr;
}

List::~List()
{
	Note* current = head;
	while (head)
	{
		current = head;
		head = head->next;
		delete current;
	}	
}

void List::push_front(int val)
{
	Note* first = new Note;
	first->value = val;
	first->next = head;
	head = first;
}

int List::pop_front()
{
	int pop = head->value;
	Note* first = head;
	head = head->next;
	delete first;
	return pop;
}

void List::clear()
{
	        Note* current = head;
        while (head->next)
        {
                current = head;
                head = head->next;
                delete current;
        }
	head->value = 0;

}

int List::len()
{
	int count = 0;
	Note* current = head;
	while (current)
	{
		count++;
		current = current->next;
	}
	return count;
}

void List::push_back(int val)
{
	Note* current = head;
	while (current->next)
	{
		current = current->next;
	}
	Note* last = new Note;
	last->value = val;
	current->next = last;
	last->next = nullptr;
}

int List::pop_back()
{
	Note* current = head;
        while (current->next)
        {
                current = current->next;
        }
	int pop = current->value;
	delete current;
	return pop;
}

void List::erase_index(int index)
{
	if (index == 0)
		int trash = this->pop_front();
	else
		if (index == this->len())
			int trash = this->pop_back();
		else
		{
			Note* current = head;
			int count = 0;
			while ((current) && (count != index - 1))
			{
				count++;
				current = current->next;
			}
			Note* trash = current->next;
			current->next = current->next->next;
			delete trash;			
		}				
}

void List::erase_element(int key)
{
	if (head->value == key)
		int trash = this->pop_front();
	else
	{
		Note* current = head;
                while (current->next)
                {
			if (current->next->value == key)
			{
				Note* trash = current->next;
		                current->next = current->next->next;
				delete trash;
			}
			current = current->next;
               	}
	}
}

void List::insert(int index, int val)
{
	if (index == 0)
		this->push_front(val);
	else
		if (index == this->len())
			this->push_back(val);
		else
		{
			int count = 0;
			Note* current = head;
			while ((count != index - 1) && (current))
			{
				count++;
				current = current->next;
			}
			Note* insert = new Note;
			insert->value = val;
			insert->next = current->next;
			current->next = insert;
		}
}

bool List::find(int key)
{
	Note* current = head;
	while (current->next)
	{
		if (current->value == key)
			return true;
		return false;
	}	
}

int& List::operator[](int index)
{
	if (index > this->len())
	{
		std::cout << "Error. Index is oit of range" << std::endl;
	       	int* ptr(0);	
		return *ptr;
	}
	int count = 0;
	Note* current = head;
	while ((count != index) && (current)) 
	{
		count++;
		current = current->next;
	}
	return current->value;
}

List& List::operator=(List& sample)
{
	Note* new_note = new Note; 
	Note* current = sample.head;
	new_note = current;
	this->head = new_note;
	if (!current)
		return *this;
       	while(current->next){
		current = current->next;
		new_note->next = new Note;
		new_note->next = current;
		new_note = new_note->next;	
	}
	return *this;	
}



