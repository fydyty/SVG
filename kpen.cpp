#include "kpen.h"
#include <QPainter>
#include <QDebug>

KPen::KPen(QObject* parent)
	: KShape(parent)
{
}

KPen::~KPen()
{
}

void KPen::drawShape(QPaintDevice* parent, QPainter* painter)
{
	bool ownPainter = false;
	if (painter == nullptr)
	{
		painter = new QPainter(parent);
		ownPainter = true;
	}

	// painter 使用逻辑坐标绘制，先 scale 到设备坐标
	const qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();

	painter->save();
	painter->scale(scale, scale);

	painter->setPen(m_pen);
	painter->setBrush(m_brush);

	// 如果已经有 m_path 就直接绘制，保证连贯与抗锯齿
	if (m_path.isEmpty() && !m_points.isEmpty()) {
		rebuildPath();
	}

	painter->drawPath(m_path);

	painter->restore();

	if (ownPainter)
	{
		painter->end();
		delete painter;
	}
}

KShapeType KPen::getShapeType()
{
	return KShapeType::LineShapeType;
}

void KPen::setPoints(QVector<QPoint> points)
{
	m_points.clear();
	m_normPoints.clear();
	m_path = QPainterPath();

	if (points.isEmpty()) {
		// 清空 start/end
		setStartPoint(QPoint());
		setEndPoint(QPoint());
		return;
	}

	// 转为 QPointF 存储（浮点）
	for (const QPoint& p : points) {
		m_points.append(QPointF(p));
	}

	// 构建路径与外接矩形，并计算规范化坐标
	rebuildPath();
	computeNormalizedPoints();
}

void KPen::rebuildPath()
{
	m_path = QPainterPath();
	if (m_points.isEmpty()) return;

	m_path.moveTo(m_points.first());
	for (int i = 1; i < m_points.size(); ++i) {
		m_path.lineTo(m_points.at(i));
	}

	// 更新外接矩形（use integer start/end as before）
	QRectF br = m_path.boundingRect();
	setStartPoint(QPoint(int(std::floor(br.left())), int(std::floor(br.top()))));
	setEndPoint(QPoint(int(std::ceil(br.right())), int(std::ceil(br.bottom()))));
}

void KPen::computeNormalizedPoints()
{
	m_normPoints.clear();
	if (m_points.isEmpty()) return;

	QRectF br = m_path.boundingRect();
	qreal w = br.width();
	qreal h = br.height();

	if (w <= 0.0 || h <= 0.0) {
		// 退化场景：所有点集中，使用 0.5,0.5 填充
		for (int i = 0; i < m_points.size(); ++i) m_normPoints.append(QPointF(0.5, 0.5));
		return;
	}

	for (const QPointF& p : m_points) {
		qreal nx = (p.x() - br.left()) / w;
		qreal ny = (p.y() - br.top()) / h;
		m_normPoints.append(QPointF(nx, ny));
	}
}

void KPen::move(QPoint offset)
{
	if (m_points.isEmpty()) return;

	// offset 在逻辑坐标系（整数），但是 m_points 是 QPointF，保留浮点
	QPointF off(offset);
	for (QPointF& p : m_points) p += off;

	// 重建路径并更新外接矩形与规范化点
	rebuildPath();
	computeNormalizedPoints();
}

// 依据新的外接矩形，对点集做“基于规范化坐标”的重构（避免累积误差）
void KPen::scaleToRect(const QPoint& newStart, const QPoint& newEnd)
{
	// 若没有规范化坐标，则先计算（兼容旧数据）
	if (m_normPoints.size() != m_points.size()) {
		// 保证 m_path 与 m_normPoints 一致
		if (m_path.isEmpty()) rebuildPath();
		computeNormalizedPoints();
	}

	QRectF newRect=QRectF(QPointF(newStart), QPointF(newEnd));
	// 允许宽/高为 0 的情况（退化）但避免除以 0
	qreal w = newRect.width();
	qreal h = newRect.height();

	if (w <= 0.0 || h <= 0.0) {
		// 退化：把所有点放置在 newRect 的中心
		QPointF center = newRect.center();
		for (int i = 0; i < m_points.size(); ++i) m_points[i] = center;
	}
	else {
		// 根据规范化坐标重建点（使用浮点计算，不做 qRound）
		for (int i = 0; i < m_points.size() && i < m_normPoints.size(); ++i) {
			const QPointF& n = m_normPoints.at(i);
			qreal nx = newRect.left() + n.x() * w;
			qreal ny = newRect.top() + n.y() * h;
			m_points[i] = QPointF(nx, ny);
		}
	}

	// 更新外接矩形（整数）与路径与规范化坐标（规范化坐标保持不变）
	setStartPoint(newStart);
	setEndPoint(newEnd);

	rebuildPath();
	// 注意：规范化坐标保持原值（因为我们按规范化坐标重构了点），不需要重新 computeNormalizedPoints()
	// 但若你希望在缩放后把规范化点更新为新 bbox 下的精确值，也可以 uncomment 下一行：
	// computeNormalizedPoints();
}

void KPen::moveTop(QPoint pos)
{
	scaleToRect(QPoint(getStartPoint().x(), pos.y()), getEndPoint());
}

void KPen::moveBottom(QPoint pos)
{
	scaleToRect(getStartPoint(), QPoint(getEndPoint().x(), pos.y()));
}

void KPen::moveLeft(QPoint pos)
{
	scaleToRect(QPoint(pos.x(), getStartPoint().y()), getEndPoint());
}

void KPen::moveRight(QPoint pos)
{
	scaleToRect(getStartPoint(), QPoint(pos.x(), getEndPoint().y()));
}

void KPen::moveTopLeft(QPoint pos)
{
	scaleToRect(pos, getEndPoint());
}

void KPen::moveTopRight(QPoint pos)
{
	scaleToRect(QPoint(getStartPoint().x(), pos.y()), QPoint(pos.x(), getEndPoint().y()));
}

void KPen::moveBottomLeft(QPoint pos)
{
	scaleToRect(QPoint(pos.x(), getStartPoint().y()), QPoint(getEndPoint().x(), pos.y()));
}

void KPen::moveBottomRight(QPoint pos)
{
	scaleToRect(getStartPoint(), pos);
}
