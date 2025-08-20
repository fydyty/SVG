#ifndef __K_GLOBAL_DATA__
#define __K_GLOBAL_DATA__

#include <QObject>

class KGlobalData : public QObject
{
	Q_OBJECT

public:
	enum class KDrawFlag
	{
		NoneDrawFlag = 0,
		MouseDrawFlag,
		PenDrawFlag,
		LineDrawFlag,
		RectDrawFlag,
		PentDrawFlag,
		HexaDrawFlag,
		CircleDrawFlag,
		TextDrawFlag,
		StarDrawFlag
	};

	KGlobalData(QObject *parent = Q_NULLPTR);
	~KGlobalData();

	static KGlobalData *getGlobalDataIntance();

	void setDrawFlag(KGlobalData::KDrawFlag drawflag);
	KDrawFlag getDrawFlag();

	void setCanvasWidth(const int width);
	void setCanvasHeight(const int height);

	int getCanvasWidth() const;
	int getCanvasHeight() const;

	void setShapePenWidth(const int width);
	int getShapePenWidth()const;

	void setShapePenStyle(const Qt::PenStyle value);
	Qt::PenStyle getShapePenStyle()const;

	void setCanvasColor(const QString & colorStr);
	QString getCanvasColor();

	void setPenColor(const QString& colorStr);
	QString getPenColor();

	void setBrushColor(const QString& colorStr);
	QString getBrushColor();

	void setCanvaScale(qreal scale);
	qreal getCanvasScale();

private:
	KGlobalData(const KGlobalData &other) = delete;
	KGlobalData(const KGlobalData &&other) = delete;
	void operator=(const KGlobalData) = delete;

	KDrawFlag m_drawFlag;
	
	int m_canvasWidth;
	int m_canvasHeight;
	int m_shapePenWidth;
	QString m_canvasColor;
	QString m_penColor;
	QString m_brushColor;
	Qt::PenStyle m_penStyle;
	
	qreal m_scale;
};


#endif
