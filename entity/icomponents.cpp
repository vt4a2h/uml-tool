/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 07/02/2015.
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
#include "icomponents.h"

#include <entity/enum.h>

namespace entity {

    /**
     * @brief IComponents::~IComponents
     */
    IComponents::~IComponents()
    {
    }

    /**
     * @brief IComponents::addNewMethod
     */
    SharedMethod IComponents::addNewMethod()
    {
        return entity::SharedMethod();
    }

    /**
     * @brief IComponents::addExistsMethod
     * @param method
     */
    void IComponents::addExistsMethod(const SharedMethod &method, int pos)
    {
        Q_UNUSED(method);
        Q_UNUSED(pos);
    }

    /**
     * @brief IComponents::removeMethod
     * @param method
     * @return
     */
    int IComponents::removeMethod(const SharedMethod &method)
    {
        Q_UNUSED(method);
        return -1;
    }

    /**
     * @brief IComponents::methods
     * @return
     */
    MethodsList IComponents::methods() const
    {
        return MethodsList();
    }

    /**
     * @brief IComponents::addNewFiled
     * @return
     */
    SharedField IComponents::addNewFiled()
    {
        return SharedField();
    }

    /**
     * @brief IComponents::addExistsFiled
     * @param field
     * @param pos
     */
    void IComponents::addExistsFiled(const SharedField &field, int pos)
    {
        Q_UNUSED(field);
        Q_UNUSED(pos);
    }

    /**
     * @brief IComponents::removeField
     * @param field
     * @return
     */
    int IComponents::removeField(const SharedField &field)
    {
        Q_UNUSED(field);
        return -1;
    }

    /**
     * @brief IComponents::fields
     * @return
     */
    FieldsList IComponents::fields() const
    {
        return FieldsList();
    }

    /**
     * @brief IComponents::addNewElement
     * @return
     */
    SharedElement IComponents::addNewElement()
    {
        return SharedElement();
    }

    /**
     * @brief IComponents::addExistsElement
     * @param element
     * @param pos
     */
    void IComponents::addExistsElement(const SharedElement &element, int pos)
    {
        Q_UNUSED(element);
        Q_UNUSED(pos);
    }

    /**
     * @brief IComponents::removeElement
     * @param element
     * @return
     */
    int IComponents::removeElement(const SharedElement &element)
    {
        Q_UNUSED(element);
        return -1;
    }

    /**
     * @brief IComponents::variables
     * @return
     */
    ElementsList IComponents::elements() const
    {
        return ElementsList();
    }

    /**
     * @brief IComponents::addNewProperty
     * @return
     */
    SharedProperty IComponents::addNewProperty()
    {
        return SharedProperty();
    }

    /**
     * @brief IComponents::addExistsProperty
     * @param property
     * @param pos
     */
    void IComponents::addExistsProperty(const SharedProperty &property, int pos)
    {
        Q_UNUSED(property);
        Q_UNUSED(pos);
    }

    /**
     * @brief IComponents::removeProperty
     * @param property
     * @return
     */
    int IComponents::removeProperty(const SharedProperty &property)
    {
        Q_UNUSED(property);
        return -1;
    }

    /**
     * @brief IComponents::properties
     * @return
     */
    PropertiesList IComponents::properties() const
    {
        return PropertiesList();
    }

} // end of namespace entity
