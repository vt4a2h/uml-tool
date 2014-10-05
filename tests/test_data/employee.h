#pragma once

namespace work {

    class Employee
    {
    public:
        Employee();
        Employee(const std::string &firstName, const std::string &lastName);
        std::string firstName() const;
        void setFirstName(const std::string &firstName);
        std::string lastName() const;
        void setLastName(const std::string &lastName);
        bool isHired() const;
        bool fire();
        bool hire();
        double salary() const;
        void setSalary(double salary);

    private:
        std::string m_firstName;
        std::string m_lastName;
        bool m_status;
        double m_salary;
    };

}