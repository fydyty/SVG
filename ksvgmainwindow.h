#ifndef __K_SVG_MAINWINDOW_H_
#define __K_SVG_MAINWINDOW_H_

#include <QScrollArea>
#include "kcanvas.h"

class KSvgMainWindow : public QScrollArea
{
	Q_OBJECT
	friend class KMainWindow;

public:
	KSvgMainWindow(QWidget *parent);
	~KSvgMainWindow();
	void setCustorStyle(KGlobalData::KDrawFlag drawFlag);

	void enterEvent(QEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void contextMenuEvent(QContextMenuEvent* event) override;
	void initMenu();
signals:
	void changeRate();
private:
	KCanvas *m_pCanvas;
	QMenu m_menu;
};

#endif
