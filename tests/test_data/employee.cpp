#include "employee.h"

namespace work {

    Employee::Employee()
    {
    }

    Employee::Employee(const std::string &firstName, const std::string &lastName)
    {
    }

    std::string Employee::firstName() const
    {
        return m_firstName;
    }

    void Employee::setFirstName(const std::string &firstName)
    {
        m_firstName = firstName;
    }

    std::string Employee::secondName() const
    {
        return m_secondName;
    }

    void Employee::setSecondName(const std::string &secondName)
    {
        m_secondName = secondName;
    }

    double Employee::salary() const
    {
        return m_salary;
    }

    void Employee::setSalary(double salary)
    {
        m_salary = salary;
    }

} // namespace work