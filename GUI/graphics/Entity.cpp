/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 09/03/2015.
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
#include "Entity.h"

#include <QPainter>
#include <QEvent>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include <QHBoxLayout>
#include <QToolButton>

#include <Application/Settings.h>

#include <GUI/EditEntityDialog.h>

#include <Entity/Type.h>
#include <Entity/Enum.h>
#include <Entity/Union.h>
#include <Entity/Class.h>
#include <Entity/TemplateClass.h>
#include <Entity/ExtendedType.h>
#include <Entity/Scope.h>
#include <Entity/ClassMethod.h>
#include <Entity/field.h>

#include <Utility/helpfunctions.h>

#include <Entity/Property.h>
#include <Entity/GraphicEntityData.h>

#include "Constants.h"
#include "QtHelpers.h"
#include "HeaderEditorEventFilter.h"

namespace Graphics {

    namespace {
        constexpr qreal margin           = 2  ;
        constexpr qreal lineIndentFactor = 2  ;
        constexpr qreal minimumHeight    = 30 ;
        constexpr qreal lineHeight       = 20 ;
        constexpr qreal minimumWidth     = 120;
        constexpr qreal paddingPercent   = 0.1;

        const QSizeF resizeCornerSize(12, 12);
        const Qt::CursorShape defaultCursorShape = Qt::ArrowCursor;

        /// Cuts too long text
        QString cutText(const QString &text, const QFontMetrics &fm, qreal frameWidth)
        {
            static const QString rightSymbols("...");

            QString result(text);
            QSize textSize(fm.size(Qt::TextSingleLine, text));
            int maxTextWidth = frameWidth * (1 - paddingPercent);
            if (textSize.width() > maxTextWidth) {
                int symbolsCount = maxTextWidth / (textSize.width() / result.count());
                symbolsCount -= rightSymbols.count();
                result.chop(result.count() - symbolsCount);
                result.append(rightSymbols);
            }

            return result;
        }

        bool drawSectionText(QPainter * painter, const QString &text, qreal width, int flags,
                             QPointF &topLeft, qreal &availableHeight)
        {
            if (qFuzzyCompare(availableHeight, 0.))
                return false;

            qreal currentLineHeight = availableHeight < lineHeight ? availableHeight : lineHeight;

            QPointF textRectLeft(topLeft + QPointF(lineIndentFactor * margin, 0));
            QRectF textRect(textRectLeft, QSizeF(width, currentLineHeight));
            painter->drawText(textRect, flags, text);

            availableHeight -= currentLineHeight;
            topLeft.ry() += currentLineHeight;

            return true;
        }

        template <class Container>
        void drawSection(QPainter * painter, const QString &sectionName, const Container elements,
                         QPointF &topLeft, qreal &availableHeight, qreal width,
                         const QColor &frameColor)
        {
            if (qFuzzyCompare(availableHeight, 0.) || elements.isEmpty())
                return;

            painter->save();

            painter->setRenderHint(QPainter::TextAntialiasing);

            // Draw section name
            drawSectionText(painter, sectionName, width, Qt::AlignCenter, topLeft, availableHeight);

            // Draw section elemnts
            for (auto &&e : elements) {
                // TODO: full signature should be displayed, not just name like now
                QString name(cutText(e->name(), painter->fontMetrics(), width));
                name.prepend(Util::sectionToSymbol(e->section()) + QChar::Space);
                if (!drawSectionText(painter, name, width, Qt::AlignLeft | Qt::AlignVCenter,
                                     topLeft, availableHeight))
                    break;
            }

            // Draw delimiter line
            if (!qFuzzyCompare(availableHeight, 0.)) {
                painter->setPen(frameColor);
                painter->drawLine(topLeft, QPointF(topLeft.x() + width, topLeft.y()));
            }

            painter->restore();
        }


        QGraphicsProxyWidget * makeHeaderEditor(QGraphicsScene & scene, QObject * parent)
        {
            auto le = new QLineEdit;
            le->setAlignment(Qt::AlignCenter);

            auto filter = new HeaderEditorEventFilter(*le, parent);
            le->installEventFilter(filter);

            return scene.addWidget(le);
        }
    }

    /**
     * @brief Entity::Entity
     * @param parent
     */
    GraphisEntity::GraphisEntity(const Entity::SharedType &type, QGraphicsItem *parent)
        : QGraphicsObject(parent)
        , m_Type(type)
        , m_LastPos(0, 0)
        , m_ResizeMode(false)
        , m_selectedToConnect(false)
    {
        setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsScenePositionChanges);

        if (width() < minimumWidth)
            setWidth(minimumWidth);

        if (height() < minimumHeight)
            setHeight(minimumHeight);

        setPos(dataPos());

        setAcceptHoverEvents(true);
        setCursor(defaultCursorShape);

        connect();
    }

    /**
     * @brief Entity::~Entity
     */
    GraphisEntity::~GraphisEntity()
    {
    }

    /**
     * @brief Entity::boundingRect
     * @return
     */
    QRectF GraphisEntity::boundingRect() const
    {
        return QRectF(-width() / 2 - margin, -height() / 2 - margin,
                      width() + margin, height() + margin);
    }

    /**
     * @brief Entity::paint
     * @param painter
     * @param option
     * @param widget
     */
    void GraphisEntity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        drawFrame(painter);
        drawHeader(painter);
        drawSections(painter);
        drawResizeCorner(painter);

        if (selectedToConnect())
            drawConnectFrame(painter);
    }

    QVariant GraphisEntity::itemChange(GraphicsItemChange change, const QVariant &value)
    {
        if (change == ItemPositionChange && scene() )
            emit positionChanged(pos(), value.toPointF());

        return QGraphicsItem::itemChange(change, value);
    }

    /**
     * @brief Entity::type
     * @return
     */
    int GraphisEntity::type() const
    {
        return Type;
    }

    /**
     * @brief Entity::typeObject
     * @return
     */
    Entity::SharedType GraphisEntity::typeObject() const
    {
        return m_Type;
    }

    /**
     * @brief Entity::setTypeObject
     * @param type
     */
    void GraphisEntity::setTypeObject(const Entity::SharedType &type)
    {
        // Disconnect old type
        if (m_Type)
            connect(false);

        // Setup new one
        m_Type = type;
        connect(true);
        setPos(dataPos());

        // Update
        redraw();
    }

    /**
     * @brief Entity::id
     * @return
     */
    Common::ID GraphisEntity::id() const
    {
        return m_Type ? m_Type->id() : Common::ID::nullID();
    }

    /**
     * @brief Entity::setSelectedToConnect
     * @param status
     */
    void GraphisEntity::setSelectedToConnect(bool status)
    {
        m_selectedToConnect = status;
    }

    /**
     * @brief Entity::selectedToConnect
     * @return
     */
    bool GraphisEntity::selectedToConnect() const
    {
        return m_selectedToConnect;
    }

    /**
     * @brief Entity::rectMargin
     * @return
     */
    qreal GraphisEntity::rectMargin()
    {
        return margin;
    }

    /**
     * @brief Entity::frameRect
     * @return
     */
    QRectF GraphisEntity::frameRect() const
    {
        return QRectF(-width() / 2, -height() / 2, width(), height());
    }

    /**
     * @brief Entity::redraw
     */
    void GraphisEntity::redraw()
    {
        // TODO: add flags and update only needed elements, e.g. header
        update(boundingRect());
    }

    /**
     * @brief Entity::mousePressEvent
     * @param event
     */
    void GraphisEntity::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        m_LastPos = pos();

        m_ResizeMode = resizeCorner().containsPoint(event->pos(), Qt::OddEvenFill);

        QGraphicsItem::mousePressEvent(event);
    }

    /**
     * @brief Entity::mouseReleaseEvent
     * @param event
     */
    void GraphisEntity::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        if (pos() != m_LastPos)
            emit moved(m_LastPos, pos());

        m_ResizeMode = false;

        QGraphicsItem::mouseReleaseEvent(event);
    }

    /**
     * @brief Entity::mouseMoveEvent
     * @param event
     */
    void GraphisEntity::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        if (m_ResizeMode) {
            QPointF pos = event->pos();
            QPointF lastPos = event->lastPos();

            qreal yLen = (QPointF(lastPos.x(), pos.y()) - lastPos).manhattanLength();
            int yFactor = pos.y() > lastPos.y() ? 2 : -2; // Why 2? See coordinate system.

            qreal xLen = (event->pos() - QPointF(lastPos.x(), pos.y())).manhattanLength();
            int xFactor = pos.x() > lastPos.x() ? 2 : -2; // Why 2? See coordinate system.

            prepareGeometryChange();

            auto tmpHeight = height() + yLen * yFactor;
            if (tmpHeight >= minimumHeight)
                setHeight(tmpHeight);

            auto tmpWidth = width() + xLen * xFactor;
            if (tmpWidth >= minimumWidth)
                setWidth(tmpWidth);

            emit sizeChanged();
        }
        else
            QGraphicsItem::mouseMoveEvent(event);
    }

    /**
     * @brief Entity::hoverMoveEvent
     * @param event
     */
    void GraphisEntity::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
    {
        if (resizeCorner().containsPoint(event->pos(), Qt::OddEvenFill) && !selectedToConnect())
            setCursor(Qt::SizeFDiagCursor);
        else
            setCursor(defaultCursorShape);

        QGraphicsItem::hoverMoveEvent(event);
    }

    /**
     * @brief GraphisEntity::mouseDoubleClickEvent
     * @param event
     */
    void GraphisEntity::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
    {
        auto rect = headerRect();
        if (rect.contains(event->pos())) {
            auto wgt = nameEditor();
            wgt->setPos(mapToParent(rect.topLeft()));
            wgt->resize(rect.width(), rect.height());

            nameEditorWgt()->setText(m_Type->name());
            nameEditorWgt()->setFocus();

            wgt->show();
        }

        QGraphicsItem::mouseDoubleClickEvent(event);
    }

    /**
     * @brief Entity::resizeCornerSize
     * @return
     */
    QPolygonF GraphisEntity::resizeCorner() const
    {
        QRectF rect(resizeCornerRect());
        return QPolygonF() << rect.topRight() << rect.bottomLeft() << rect.bottomRight();
    }

    /**
     * @brief Entity::resizeCornerRect
     * @return
     */
    QRectF GraphisEntity::resizeCornerRect() const
    {
        return QRectF(width() / 2  - resizeCornerSize.width(),
                      height() / 2 - resizeCornerSize.height(),
                      resizeCornerSize.width(), resizeCornerSize.height());
    }

    /**
     * @brief GraphisEntity::headerRect
     * @return
     */
    QRectF GraphisEntity::headerRect() const
    {
        return {-width() / 2, -height() / 2, width(), minimumHeight};
    }

    /**
     * @brief Entity::width
     * @return
     */
    qreal GraphisEntity::width() const
    {
        return G_ASSERT(G_ASSERT(m_Type)->graphicEntityData())->width();
    }

    /**
     * @brief Entity::setWidth
     * @param newWidth
     */
    void GraphisEntity::setWidth(qreal newWidth)
    {
        G_ASSERT(G_ASSERT(m_Type)->graphicEntityData())->setWidth(newWidth);
    }

    /**
     * @brief Entity::height
     * @return
     */
    qreal GraphisEntity::height() const
    {
        return G_ASSERT(G_ASSERT(m_Type)->graphicEntityData())->height();
    }

    /**
     * @brief Entity::setHeight
     * @param newHeight
     */
    void GraphisEntity::setHeight(qreal newHeight)
    {
        G_ASSERT(G_ASSERT(m_Type)->graphicEntityData())->setHeight(newHeight);
    }

    /**
     * @brief Entity::dataPos
     * @return
     */
    QPointF GraphisEntity::dataPos() const
    {
        return G_ASSERT(G_ASSERT(m_Type)->graphicEntityData())->pos();
    }

    /**
     * @brief Entity::color
     * @return
     */
    QColor GraphisEntity::typeColor() const
    {
        return App::Settings::elementColor(
                    Entity::kindOfTypeToString(G_ASSERT(m_Type)->kindOfType()));
    }

    /**
     * @brief GraphisEntity::nameEditor
     * @return
     */
    QGraphicsProxyWidget *GraphisEntity::nameEditor()
    {
        if (!m_NameEditor) {
            m_NameEditor = makeHeaderEditor(*G_ASSERT(scene()), this);
            m_NameEditor->setVisible(false);
        }

        return m_NameEditor.data();
    }

    QLineEdit *GraphisEntity::nameEditorWgt()
    {
        return qobject_cast<QLineEdit*>(nameEditor()->widget());
    }

    void GraphisEntity::connect(bool connect)
    {
        auto *data = G_ASSERT(m_Type->graphicEntityData()).get();

        if (connect) {
            G_CONNECT(G_ASSERT(m_Type.get()), &Common::BasicElement::nameChanged,
                      this, &GraphisEntity::redraw);
            G_CONNECT(this, &GraphisEntity::positionChanged,
                      G_ASSERT(data), &Entity::GraphicEntityData::onPosChanged);
        } else {
            G_DISCONNECT(G_ASSERT(m_Type.get()), &Common::BasicElement::nameChanged,
                         this, &GraphisEntity::redraw);
            G_DISCONNECT(this, &GraphisEntity::positionChanged,
                         G_ASSERT(data), &Entity::GraphicEntityData::onPosChanged);
        }
    }

    /**
     * @brief Entity::drawHeader
     * @param painter
     */
    void GraphisEntity::drawHeader(QPainter *painter)
    {
        painter->save();

        QColor color = typeColor();

        // Fill background
        QLinearGradient gradient(0, -height() / 2, 0, -height() / 2 + minimumHeight );
        gradient.setColorAt(0, color);
        gradient.setColorAt(1, Qt::white);
        QRectF headerRect = this->headerRect();
        painter->fillRect(headerRect, QBrush(gradient));

        // Draw frame
        painter->setPen(color);
        painter->drawRect(headerRect);

        // Add element name
        painter->setPen(Qt::black);
        painter->setRenderHint(QPainter::TextAntialiasing);
        QString name(cutText(G_ASSERT(m_Type)->name(), painter->fontMetrics(), width()));
        painter->drawText(headerRect, Qt::AlignCenter, name);

        painter->restore();
    }

    /**
     * @brief Entity::drawFrame
     * @param painter
     */
    void GraphisEntity::drawFrame(QPainter *painter)
    {
        painter->save();

        QColor color = typeColor();
        QRectF rect(frameRect());
        painter->fillRect(rect, Qt::white);

        painter->setPen(color);
        painter->drawRect(rect);

        painter->restore();
    }

    /**
     * @brief Entity::drawResizeCorner
     * @param painter
     */
    void GraphisEntity::drawResizeCorner(QPainter *painter)
    {
        painter->save();

        painter->setRenderHint(QPainter::Antialiasing);

        painter->setPen(typeColor());

        QRectF rect(resizeCornerRect());
        QPointF bottomLeft(rect.bottomLeft());
        QPointF topRight(rect.topRight());

        // Draw hypotenuse
        painter->drawLine(bottomLeft, topRight);

        // Draw middle line
        bottomLeft.rx() += rect.width() / 2;
        topRight.ry() += rect.height() / 2;
        painter->drawLine(bottomLeft, topRight);

        painter->restore();
    }

    /**
     * @brief Entity::drawSections
     * @param painter
     */
    void GraphisEntity::drawSections(QPainter *painter)
    {
        painter->save();

        // Calculate initial parameters
        QColor color = typeColor();
        auto topLeft = boundingRect().topLeft() + QPointF(margin, margin);
        topLeft.ry() += minimumHeight;
        qreal len = height() - minimumHeight;

        // Draw sections
        drawSection(painter, tr("Properties"), m_Type->properties(), topLeft, len, width(), color);
        drawSection(painter, tr("Methods"), m_Type->methods(), topLeft, len, width(), color);
        drawSection(painter, tr("Fields"), m_Type->fields(), topLeft, len, width(), color);
        drawSection(painter, tr("Elements"), m_Type->enumerators(), topLeft, len, width(), color);

        painter->restore();
    }

    /**
     * @brief Entity::drawConnectFrame
     * @param painter
     */
    void GraphisEntity::drawConnectFrame(QPainter *painter)
    {
        painter->save();

        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(QPen(Qt::red, 2));
        painter->drawRect(frameRect());

        painter->restore();
    }

} // grpahics
