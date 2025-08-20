#ifndef KPEN_H
#define KPEN_H

#include "kshape.h"
#include <QVector>
#include <QPointF>
#include <QPainterPath>

class KPen : public KShape
{
	Q_OBJECT
public:
	explicit KPen(QObject* parent = nullptr);
	~KPen();

	// 绘制
	void drawShape(QPaintDevice* parent = nullptr, QPainter* painter = nullptr) override;
	KShapeType getShapeType() override;

	// 设置点（外部接口保持不变）
	void setPoints(QVector<QPoint> points) override;
	void move(QPoint offset) override;

	// 缩放外接矩形（逻辑坐标）
	void scaleToRect(const QPoint& newStart, const QPoint& newEnd);

	// 四周/角拖动接口（保留你的接口）
	void moveTop(QPoint pos);
	void moveBottom(QPoint pos);
	void moveLeft(QPoint pos);
	void moveRight(QPoint pos);
	void moveTopLeft(QPoint pos);
	void moveTopRight(QPoint pos);
	void moveBottomLeft(QPoint pos);
	void moveBottomRight(QPoint pos);

private:
	void rebuildPath();             // 根据 m_points 重建 m_path 与 start/end
	void computeNormalizedPoints(); // 根据当前 m_points 与 bbox 生成 m_normPoints

private:
	QVector<QPointF> m_points;      // 使用浮点逻辑坐标，避免整型量化误差
	QVector<QPointF> m_normPoints;  // 规范化坐标（相对于外接矩形，范围 0..1）
	QPainterPath m_path;
};

#endif // KPEN_H
