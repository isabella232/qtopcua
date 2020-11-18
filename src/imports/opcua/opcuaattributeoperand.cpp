/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt OPC UA module.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "opcuaattributeoperand.h"
#include "opcuanodeid.h"
#include "universalnode.h"
#include "opcuarelativenodepath.h"
#include <QOpcUaRelativePathElement>

QT_BEGIN_NAMESPACE

/*!
    \qmltype AttributeOperand
    \inqmlmodule QtOpcUa
    \brief The OPC UA AttributeOperand type.
    \since QtOpcUa 5.13

    The AttributeOperand is defined in OPC-UA part 4, 7.4.4.4.
    It has the same purpose as \l SimpleAttributeOperand but has more configurable options.
*/

/*!
    \qmlproperty list<OpcUaNodeId> AttributeOperand::browsePath

    Browse path to the node holding the attribute.

    \code
    import QtOpcUa 5.13 as QtOpcUa

    QtOpcUA.AttributeOperand {
        ...
        browsePath: [
            QtOpcUa.NodeId {
                identifier: "Message"
                ns: "http://opcfoundation.org/UA/"
             }
             ...
        ]
    }
*/

/*!
    \qmlproperty string AttributeOperand::indexRange

    Index range string used to identify a single value or subset of the attribute's value.

    \code
    import QtOpcUa 5.13 as QtOpcUa

    QtOpcUa.AttributeOperand {
        ...
        indexRange: "0:2"
    }
    \endcode
*/

/*!
    \qmlproperty Constants.NodeAttribute AttributeOperand::nodeAttribute

    Attribute of the node \l browsePath is pointing to.
    The default value is \c Constants.NodeAttribute.Value.

    \code
    import QtOpcUa 5.13 as QtOpcUa

    QtOpcUa.AttributeOperand {
        ...
        nodeAttribute: QtOpcUa.Constants.NodeAttribute.Value
    }
    \endcode
*/

/*!
    \qmlproperty string AttributeOperand::typeId

    Node id of the type definition node. The operand will be of the type or one of its subtypes.

    \code
    import QtOpcUa 5.13 as QtOpcUa

    QtOpcUa.AttributeOperand {
        ...
        typeId: "ns=0;i=2041"
    }
    \endcode
*/

/*!
    \qmlproperty string AttributeOperand::alias

    Alias name for the operand. This allows using this instance
    as operand for other operations in the filter.
*/

OpcUaAttributeOperand::OpcUaAttributeOperand(QObject *parent)
    : OpcUaOperandBase(parent)
{
}

OpcUaAttributeOperand::~OpcUaAttributeOperand() = default;

QVariant OpcUaAttributeOperand::toCppVariant(QOpcUaClient *client) const
{
    QOpcUaAttributeOperand value(*this);

    if (m_nodeId) {
        UniversalNode un(m_nodeId);
        un.resolveNamespace(client);
        value.setAlias(un.fullNodeId());
    }

    for (const auto &i : m_browsePath)
        value.browsePathRef().append(i->toRelativePathElement(client));
    return value;
}

QString OpcUaAttributeOperand::indexRange() const
{
    return QOpcUaAttributeOperand::indexRange();
}

void OpcUaAttributeOperand::setIndexRange(const QString &indexRange)
{
    if (indexRange != QOpcUaAttributeOperand::indexRange()) {
        QOpcUaAttributeOperand::setIndexRange(indexRange);
        emit dataChanged();
    }
}

QOpcUa::NodeAttribute OpcUaAttributeOperand::nodeAttribute() const
{
    return QOpcUaAttributeOperand::attributeId();
}

void OpcUaAttributeOperand::setNodeAttribute(QOpcUa::NodeAttribute nodeAttribute)
{
    if (nodeAttribute != QOpcUaAttributeOperand::attributeId()) {
        QOpcUaAttributeOperand::setAttributeId(nodeAttribute);
        emit dataChanged();
    }
}

OpcUaNodeId *OpcUaAttributeOperand::typeId() const
{
    return m_nodeId;
}

void OpcUaAttributeOperand::setTypeId(OpcUaNodeId *typeId)
{
    if (typeId != m_nodeId) {
        m_nodeId = typeId;
        emit dataChanged();
    }
}

QQmlListProperty<OpcUaRelativeNodePath> OpcUaAttributeOperand::browsePath()
{
    return QQmlListProperty<OpcUaRelativeNodePath>(this, this,
                                         &OpcUaAttributeOperand::appendBrowsePathElement,
                                         &OpcUaAttributeOperand::browsePathSize,
                                         &OpcUaAttributeOperand::browsePathElement,
                                         &OpcUaAttributeOperand::clearBrowsePath);
}

void OpcUaAttributeOperand::appendBrowsePathElement(OpcUaRelativeNodePath *nodeId)
{
    m_browsePath.append(nodeId);
    emit dataChanged();
}

int OpcUaAttributeOperand::browsePathSize() const
{
    return m_browsePath.size();
}

OpcUaRelativeNodePath *OpcUaAttributeOperand::browsePathElement(int index) const
{
    return m_browsePath.at(index);
}

void OpcUaAttributeOperand::clearBrowsePath()
{
    m_browsePath.clear();
    emit dataChanged();
}

void OpcUaAttributeOperand::appendBrowsePathElement(QQmlListProperty<OpcUaRelativeNodePath> *list, OpcUaRelativeNodePath *nodeId)
{
    reinterpret_cast<OpcUaAttributeOperand*>(list->data)->appendBrowsePathElement(nodeId);
}

qsizetype OpcUaAttributeOperand::browsePathSize(QQmlListProperty<OpcUaRelativeNodePath> *list)
{
    return reinterpret_cast<OpcUaAttributeOperand*>(list->data)->browsePathSize();
}

OpcUaRelativeNodePath *OpcUaAttributeOperand::browsePathElement(QQmlListProperty<OpcUaRelativeNodePath> *list, qsizetype index)
{
    return reinterpret_cast<OpcUaAttributeOperand*>(list->data)->browsePathElement(index);
}

void OpcUaAttributeOperand::clearBrowsePath(QQmlListProperty<OpcUaRelativeNodePath> *list)
{
    reinterpret_cast<OpcUaAttributeOperand*>(list->data)->clearBrowsePath();
}

QString OpcUaAttributeOperand::alias() const
{
    return QOpcUaAttributeOperand::alias();
}

void OpcUaAttributeOperand::setAlias(const QString &alias)
{
    QOpcUaAttributeOperand::setAlias(alias);
}

QT_END_NAMESPACE
