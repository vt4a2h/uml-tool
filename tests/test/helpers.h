#pragma once

#define search_circle(table_name, method_name, type)   \
for (auto key : table_name.keys()) {\
    p = _d->method_name(table_name[key]->id());\
    \
    EXPECT_TRUE(p.operator bool())\
            << QString(#method_name "() should return valid ptr for " #type " \"%1\"")\
               .arg(table_name[key]->name()).toStdString();\
    EXPECT_EQ(p, table_name[key])\
            << QString(#method_name "() should find " #type " \"%1\"")\
               .arg(table_name[key]->name()).toStdString();\
}

#define invalid_case(method_name, invalid_id) \
p = _d->method_name(invalid_id);\
EXPECT_EQ(p, nullptr) << #method_name "() should return nullptr for invalid id";

#define json_eq(basic_obj, comp_obj, name)\
    ASSERT_TRUE(comp_obj->readFromFile(m_JsonFileName))\
            << "Data for " #name " should be a correct";\
    EXPECT_EQ(*basic_obj, *comp_obj)\
            << "Read/write for " #name " should work correctly";
