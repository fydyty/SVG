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

	// painter ʹ���߼�������ƣ��� scale ���豸����
	const qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();

	painter->save();
	painter->scale(scale, scale);

	painter->setPen(m_pen);
	painter->setBrush(m_brush);

	// ����Ѿ��� m_path ��ֱ�ӻ��ƣ���֤�����뿹���
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
		// ��� start/end
		setStartPoint(QPoint());
		setEndPoint(QPoint());
		return;
	}

	// תΪ QPointF �洢�����㣩
	for (const QPoint& p : points) {
		m_points.append(QPointF(p));
	}

	// ����·������Ӿ��Σ�������淶������
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

	// ������Ӿ��Σ�use integer start/end as before��
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
		// �˻����������е㼯�У�ʹ�� 0.5,0.5 ���
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

	// offset ���߼�����ϵ�������������� m_points �� QPointF����������
	QPointF off(offset);
	for (QPointF& p : m_points) p += off;

	// �ؽ�·����������Ӿ�����淶����
	rebuildPath();
	computeNormalizedPoints();
}

// �����µ���Ӿ��Σ��Ե㼯�������ڹ淶�����ꡱ���ع��������ۻ���
void KPen::scaleToRect(const QPoint& newStart, const QPoint& newEnd)
{
	// ��û�й淶�����꣬���ȼ��㣨���ݾ����ݣ�
	if (m_normPoints.size() != m_points.size()) {
		// ��֤ m_path �� m_normPoints һ��
		if (m_path.isEmpty()) rebuildPath();
		computeNormalizedPoints();
	}

	QRectF newRect=QRectF(QPointF(newStart), QPointF(newEnd));
	// �����/��Ϊ 0 ��������˻������������ 0
	qreal w = newRect.width();
	qreal h = newRect.height();

	if (w <= 0.0 || h <= 0.0) {
		// �˻��������е������ newRect ������
		QPointF center = newRect.center();
		for (int i = 0; i < m_points.size(); ++i) m_points[i] = center;
	}
	else {
		// ���ݹ淶�������ؽ��㣨ʹ�ø�����㣬���� qRound��
		for (int i = 0; i < m_points.size() && i < m_normPoints.size(); ++i) {
			const QPointF& n = m_normPoints.at(i);
			qreal nx = newRect.left() + n.x() * w;
			qreal ny = newRect.top() + n.y() * h;
			m_points[i] = QPointF(nx, ny);
		}
	}

	// ������Ӿ��Σ���������·����淶�����꣨�淶�����걣�ֲ��䣩
	setStartPoint(newStart);
	setEndPoint(newEnd);

	rebuildPath();
	// ע�⣺�淶�����걣��ԭֵ����Ϊ���ǰ��淶�������ع��˵㣩������Ҫ���� computeNormalizedPoints()
	// ������ϣ�������ź�ѹ淶�������Ϊ�� bbox �µľ�ȷֵ��Ҳ���� uncomment ��һ�У�
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
