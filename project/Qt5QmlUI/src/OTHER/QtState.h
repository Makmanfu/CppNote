

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>

class QGraphicsRectWidget : public QGraphicsWidget
{
public:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*,QWidget*) Q_DECL_OVERRIDE
    {
        painter->fillRect(rect(), Qt::blue);
    };
};

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QGraphicsScene* scene, QWidget* parent = NULL) : QGraphicsView(scene, parent) {};
protected:
    virtual void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE
    {
        fitInView(scene()->sceneRect());
        QGraphicsView::resizeEvent(event);
    };
};

class StateSwitchEvent: public QEvent
{
public:
    enum { StateSwitchType = QEvent::User + 256 };
    StateSwitchEvent(): QEvent(Type(StateSwitchType)) {};
    explicit StateSwitchEvent(int rand): QEvent(Type(StateSwitchType)), m_rand(rand) {};
public:
    inline int rand() const
    {
        return m_rand;
    };
private:
    int m_rand;
};

class StateSwitchTransition: public QAbstractTransition
{
public:
    StateSwitchTransition(int rand);
protected:
    virtual bool eventTest(QEvent* event) Q_DECL_OVERRIDE;
    virtual void onTransition(QEvent*) Q_DECL_OVERRIDE;
private:
    int m_rand;
};

class StateSwitcher : public QState
{
    Q_OBJECT
public:
    StateSwitcher(QStateMachine* machine);
public:
    virtual void onEntry(QEvent*) Q_DECL_OVERRIDE;
    virtual void onExit(QEvent*) Q_DECL_OVERRIDE;
    void addState(QAbstractState* state);
private:
    int m_stateCount;
    int m_lastIndex;
};