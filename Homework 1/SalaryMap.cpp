#include "SalaryMap.h"

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;


SalaryMap::SalaryMap()       // Create an empty salary map
{
	Map m_salaryMap;
}

bool SalaryMap::add(std::string name, double salary)
    // If an employee with the given name has not previously been added
    // there is room in the map, and the salary is not negative, add an
    // entry for that employee and salary and return true.  Otherwise
    // make no change to the map and return false.
{
	if(m_salaryMap.contains(name) || salary<0 || size()>=DEFAULT_MAX_ITEMS)
		return false;
	m_salaryMap.insert(name, salary);
	return true;
}

bool SalaryMap::raise(std::string name, double pct)
    // If no employee with the given name is in the map or if pct is less
    // than -100, make no change to the map and return false.  Otherwise,
    // change the salary of the indicated employee by the given
    // percentage and return true.  For example, if pct is 10, the
    // employee gets a 10% raise; if it's -5, the salary is reduced by 5%.
{
	if(!m_salaryMap.contains(name) || pct<-100)
		return false;
	double originalSalary;
	m_salaryMap.get(name, originalSalary);
	double newSalary = originalSalary*(100+pct)/100;

	m_salaryMap.update(name, newSalary);
	return true;
}

double SalaryMap::salary(std::string name) const
    // If an employee with the given name is in the map, return that
    // employee's salary; otherwise, return -1.
{
	double salary;
	bool employeeExists = m_salaryMap.get(name, salary);

	if(!employeeExists)
		return -1;
	return salary;
}

int SalaryMap::size() const  // Return the number of employees in the SalaryMap.
{
	return m_salaryMap.size();
}

void SalaryMap::print() const
    // Write to cout one line for every employee in the map.  Each line
    // has the employee's name, followed by one space, followed by that
    // employee's salary.
{
	for(int i=0; i<size(); i++)
	{
		string name;
		double salary;

		m_salaryMap.get(i, name, salary);

		cout << name << " " << salary << endl;
	}
}
