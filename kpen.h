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

	// ����
	void drawShape(QPaintDevice* parent = nullptr, QPainter* painter = nullptr) override;
	KShapeType getShapeType() override;

	// ���õ㣨�ⲿ�ӿڱ��ֲ��䣩
	void setPoints(QVector<QPoint> points) override;
	void move(QPoint offset) override;

	// ������Ӿ��Σ��߼����꣩
	void scaleToRect(const QPoint& newStart, const QPoint& newEnd);

	// ����/���϶��ӿڣ�������Ľӿڣ�
	void moveTop(QPoint pos);
	void moveBottom(QPoint pos);
	void moveLeft(QPoint pos);
	void moveRight(QPoint pos);
	void moveTopLeft(QPoint pos);
	void moveTopRight(QPoint pos);
	void moveBottomLeft(QPoint pos);
	void moveBottomRight(QPoint pos);

private:
	void rebuildPath();             // ���� m_points �ؽ� m_path �� start/end
	void computeNormalizedPoints(); // ���ݵ�ǰ m_points �� bbox ���� m_normPoints

private:
	QVector<QPointF> m_points;      // ʹ�ø����߼����꣬���������������
	QVector<QPointF> m_normPoints;  // �淶�����꣨�������Ӿ��Σ���Χ 0..1��
	QPainterPath m_path;
};

#endif // KPEN_H
