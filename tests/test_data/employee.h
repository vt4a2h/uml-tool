#pragma once

namespace work {

    class Employee
    {
    public:
        Employee();
        Employee(const std::string &firstName, const std::string &lastName);
        std::string firstName() const;
        void setFirstName(const std::string &firstName);
        std::string secondName() const;
        void setSecondName(const std::string &secondName);
        bool isHired() const;
        bool fire();
        bool hire();
        double salary() const;
        void setSalary(double salary);

    private:
        std::string m_firstName;
        std::string m_secondName;
        bool m_status;
        double m_salary;
    };

}