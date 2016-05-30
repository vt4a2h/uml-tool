/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 23/10/2014.
**
** This file is part of Q-UML (UML tool for Qt).
**
** Q-UML is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Q-UML is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.

** You should have received a copy of the GNU Lesser General Public License
** along with Q-UML.  If not, see <http://www.gnu.org/licenses/>.
**
*****************************************************************************/

#pragma once

#define search_circle(table_name, method_name, type, ...)   \
for (auto key : table_name.keys()) {\
    p = m_ProjectDb->method_name(table_name[key]->id(), ##__VA_ARGS__);\
    \
    EXPECT_TRUE(p.operator bool())\
            << QString(#method_name "() should return valid ptr for " #type " \"%1\"")\
               .arg(table_name[key]->name()).toStdString();\
    EXPECT_EQ(p, table_name[key])\
            << QString(#method_name "() should find " #type " \"%1\"")\
               .arg(table_name[key]->name()).toStdString();\
}

#define invalid_case(method_name, invalid_id, ...) \
p = m_ProjectDb->method_name(invalid_id, ##__VA_ARGS__);\
EXPECT_EQ(p, nullptr) << #method_name "() should return nullptr for invalid id";

#define json_eq(basic_obj, comp_obj, name)\
    ASSERT_TRUE(comp_obj->readFromFile(m_JsonFileName))\
            << "Data for " #name " should be a correct";\
    EXPECT_EQ(*basic_obj, *comp_obj)\
            << "Read/write for " #name " should work correctly"; \

#define test_relation(type, actions)\
    auto relation(std::make_shared<relationship::type>(m_FirstClass->id(), m_SecondClass->id(), \
                                                       db::WeakTypeSearchers({m_GlobalDb, m_ProjectDb})));\
    \
    actions();\
    \
    relation->writeToFile(m_JsonFileName);\
    \
    auto relation_comp(std::make_shared<relationship::Relation>());\
    relation_comp->setTypeSearchers({m_GlobalDb, m_ProjectDb});\
    json_eq(relation, relation_comp, #type);

#define read_from(var_name, file_name, file_path)\
    QString var_name;\
    QFile file_name(file_path);\
    if (file_name.open(QIODevice::ReadOnly | QIODevice::Text)) {\
        QTextStream s(&file_name);\
        var_name = s.readAll();\
    }\
    file_name.close();\
    EXPECT_FALSE(var_name.isEmpty())\
            << "Test data for " #file_path " shouldn't be empty";
