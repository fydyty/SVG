#ifndef __K_CANVAS_PARAMS_BAR_H_
#define __K_CANVAS_PARAMS_BAR_H_

#include "kcolorbox.h"
#include "kvaluebox.h"
#include "kstylebox.h"
class KCanvasParamsBar : public QWidget
{
	Q_OBJECT
	friend class KMainWindow;
public:
	KCanvasParamsBar(const QString &title,QWidget *parent);
	~KCanvasParamsBar();
private:
	KValueBox *m_pWidthBox;
	KValueBox *m_pHeightBox;
	KValueBox *m_pScaleBox;
	KValueBox *m_pPenWidthBox;
	KColorBox *m_pColorBox;
	KColorBox *m_pColorPenBox;
	KColorBox *m_pColorBrushBox;
	KStyleBox *m_pStyleBox;

	QGridLayout* m_pGridLayout;
	QVBoxLayout* m_pVBoxLayout;
	//QString m_title;
	QLabel* m_pTitleLabel;
};

#endif
