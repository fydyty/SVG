#ifndef __K_CANVAS_H_
#define __K_CANVAS_H_

#include <QWidget>
#include <QMenu>
#include <QLabel>
#include "kshape.h"
#include "kglobaldata.h"

enum class KTransType
{
	None = 0,
	TopLeft, 
	Top,
	TopRight,
	Left,
	Contains,
	Right,
	BottomLeft,
	Bottom,
	BottomRight
};


class KCanvas : public QWidget
{
	Q_OBJECT

public:
	KCanvas(QWidget *parent = Q_NULLPTR);
	~KCanvas();

	virtual void paintEvent(QPaintEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	

	KShape* getCurrentShape(const QPoint &pos);
	KTransType getTransType(const QPoint &pos); 
	void updateCursorStyle(KGlobalData::KDrawFlag flag, KTransType transType);
	void dragMoveShape(KTransType transType,const QPoint &pos);
	KShape* getShape();
	QList <KShape*> getShapeList();
	bool getSelected();
	void setSvgImage(QImage &image);
	void clear();

	void onSelectAll();
	void onCut();
	void onCopy();
	void onPaste();
	void onDuplicate();
	void onDelete();
	void onUpLayer();
	void onDownLayer();
	void onMostFront();
	void onMostBack();
private:
	KShape* m_pCurrentShape;
	KShape* m_pasteShape;
	QList <KShape*> m_pShapeList;
	QVector <QVector<QPoint>> m_points;
	QPoint m_lastPos;
	QImage m_image;
	QLabel m_label;
	KTransType m_TransType;
	
	bool m_isLPress;
	bool m_isDrawing;
	bool m_isSelected;
	bool m_onSelectAll;
	bool m_isImage;
};

#endif
