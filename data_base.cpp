#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <iomanip>

using namespace std;

class Date {
public:
  Date () {}
  Date (const int& new_year, const int& new_month, const int& new_day) {
	  year = new_year;
	  month = new_month;
	  day = new_day;
  }
  int GetYear() const {
	  return year;
  }
  int GetMonth() const {
	  return month;
  }
  int GetDay() const {
	  return day;
  }

private:
  int year;
  int month;
  int day;
};

// Below there are 3 operator overloadings for Date class
bool operator<(const Date& lhs, const Date& rhs) {
	if (lhs.GetYear() != rhs.GetYear())
	{
		return lhs.GetYear() < rhs.GetYear();
	}
	else if (lhs.GetMonth() != rhs.GetMonth())
	{
		return lhs.GetMonth() < rhs.GetMonth();
	}
	else
	{
		return lhs.GetDay() < rhs.GetDay();
	}
}

istream& operator>> (istream& stream, Date& date) {
	int year, month, day;
	string s1, s2;
	char tmp;
	int trigger = 0;
	stream >> s1;
	s2 = s1;
	stringstream ss (s2);
	if (!isdigit(ss.peek()))
	{
		ss.get(tmp);
		if (tmp != '-' || (!isdigit(ss.peek())))
		{
			trigger++;
		}
		ss.unget();
	}
	ss >> year;
	if (ss.peek() != '-')
	{
		trigger++;
	}
	ss.ignore(1);
	if (!isdigit(ss.peek()))
	{
		ss.get(tmp);
		if (tmp != '-' || (!isdigit(ss.peek())))
		{
			trigger++;
		}
		ss.unget();
	}
	ss >> month;
	if (ss.peek() != '-')
	{
		trigger++;
	}
	ss.ignore(1);
	if (!isdigit(ss.peek()))
	{
		ss.get(tmp);
		if (tmp != '-' || (!isdigit(ss.peek())))
		{
			trigger++;
		}
		ss.unget();
	}
	ss >> day;
	date = Date (year, month, day);
	if (ss.peek() != EOF)
	{
		trigger++;
	}
	if (trigger != 0)
	{
		throw runtime_error ("Wrong date format: " + s1);
	}
	if (month < 1 || month > 12)
	{
		throw runtime_error ("Month value is invalid: " + to_string(month));
	}
	else if (day < 1 || day > 31)
	{
		throw runtime_error ("Day value is invalid: " + to_string(day));
	}
	return stream;
}

ostream& operator<< (ostream& stream, const Date& date) {
	stream << setfill('0');
	stream << setw(4) << date.GetYear() << '-' << setw(2) << date.GetMonth()
			<< '-' << setw(2) << date.GetDay();
	return stream;
}

class Database {
public:
  void AddEvent (const Date& date, const string& event)
  {
	  d_map[date].insert(event);
  }
  
  bool DeleteEvent (const Date& date, const string& event)
  {
	  if (d_map.count(date) > 0)
	  {
		  for (const string& item : d_map[date])
		  {
			  if (item == event)
			  {
				  d_map[date].erase(item);
				  if (d_map[date].empty())
				  {
					  d_map.erase(date);
				  }
				  return true;
			  }
		  }
		  return false;
	  }
	  return false;
  }
  
  int DeleteDate (const Date& date)
  {
	  size_t counter = d_map[date].size();
	  if (d_map.count(date) > 0)
	  {
		  d_map.erase(date);
	  }
	  return counter;
  }

  void Find (const Date& date) const
  {
	  if (d_map.count(date) > 0)
	  {
		  for (const auto& item : d_map.at(date))
		  {
			  cout << item << endl;
		  }
	  }
  }
  
  void Print() const
  {
	  for (const auto& item : d_map)
	  {
		  for (const string& item_2 : item.second)
		  {
			  cout << item.first << " " << item_2 << endl;
		  }
	  }
  }

private:
map <Date, set<string>> d_map; // Date -> events
};

int main() {
  Database db;

  string command;
  while (getline(cin, command)) { // Reading the whole command (i.e. Add 2018-10-10 event)
	  try {
	  stringstream stream (command);
	  string op;
	  stream >> op; // Reading the operation (i.e. Add)
	  if (op == "Add")
	  {
		  Date date;
		  string event;
		  stream >> date;
		  stream >> event;
		  db.AddEvent (date, event);
	  }
	  else if (op == "Del")
	  {
		  Date date;
		  string event;
		  stream >> date;
		  stream >> event;
		  if (event != "")
		  {
			  if (db.DeleteEvent (date, event))
			  {
				  cout << "Deleted successfully" << endl;
			  }
			  else
			  {
				  cout << "Event not found" << endl;
			  }
		  }
		  else
		  {
			  cout << "Deleted " << db.DeleteDate (date) << " events" << endl;
		  }
	  }
	  else if (op == "Find")
	  {
		  Date date;
		  stream >> date;
		  db.Find(date);
	  }
	  else if (op == "Print")
	  {
		  db.Print();
	  }
	  else if (op == "") // Ignoring the empty command
	  {

	  }
	  else if (op == "End") // Exiting the program
	  {
		  return 0;
	  }
	  else
	  {
		  throw runtime_error ("Unknown command: " + op);
	  }
	  } catch (exception& ex) {
		  cout << ex.what() << endl;
	  }
  }
  return 0;
}
