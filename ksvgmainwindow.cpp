#include "ksvgmainwindow.h"
#include <QApplication>
#include <QWheelEvent>
#include <QDebug>
#include <QShortcut>
KSvgMainWindow::KSvgMainWindow(QWidget *parent)
	: QScrollArea(parent)
	, m_pCanvas(nullptr)
	, m_menu(this)
{
	setFrameShape(QFrame::NoFrame); // �����ޱ߿�

	// �����ݳ�����ʾ����ʱ���ֹ�����
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setStyleSheet("QScrollArea{background-color:#3f3f3c;}");
	setAlignment(Qt::AlignCenter);
	
	m_pCanvas = new KCanvas;
	setWidget(m_pCanvas);// ���õ�ǰ�ؼ�Ϊ����������ӿؼ�
	initMenu();
}

KSvgMainWindow::~KSvgMainWindow()
{
	delete m_pCanvas;
}

void KSvgMainWindow::setCustorStyle(KGlobalData::KDrawFlag drawFlag)
{
	switch (drawFlag)
	{
	case KGlobalData::KDrawFlag::PenDrawFlag:
		setCursor(Qt::ArrowCursor);
		break;
	case KGlobalData::KDrawFlag::LineDrawFlag:
	case KGlobalData::KDrawFlag::RectDrawFlag:
	case KGlobalData::KDrawFlag::CircleDrawFlag:

		setCursor(Qt::CrossCursor);
		break;

	case KGlobalData::KDrawFlag::TextDrawFlag:
		setCursor(Qt::IBeamCursor);
		break;
	default:
		setCursor(Qt::ArrowCursor);
		break;
	}
}

void KSvgMainWindow::enterEvent(QEvent* event)
{
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	setCustorStyle(flag);
	QScrollArea::enterEvent(event);
}

void KSvgMainWindow::wheelEvent(QWheelEvent* event)
{
	if (QApplication::keyboardModifiers() == Qt::ControlModifier)
	{
		qreal scale = 1.0;
		if (event->delta() > 0)
			scale = KGlobalData::getGlobalDataIntance()->getCanvasScale() + 0.05;
		else
			scale = KGlobalData::getGlobalDataIntance()->getCanvasScale() - 0.05;

		qint32 width = KGlobalData::getGlobalDataIntance()->getCanvasWidth();
		qint32 height = KGlobalData::getGlobalDataIntance()->getCanvasHeight();
		m_pCanvas->resize(width * scale, height * scale);
		KGlobalData::getGlobalDataIntance()->setCanvaScale(scale);
		emit changeRate();
	}

}

void KSvgMainWindow::initMenu()
{
	m_menu.setStyleSheet("QMenu { background-color: black; color: white; } "
		"QMenu::item:selected { background-color: white; color: black; } ");
	QAction* selectAllAction = new QAction(QString::fromLocal8Bit("ȫѡ"), this);
	QShortcut* selectAllShortcut = new QShortcut(QKeySequence("Ctrl+A"), this);
	selectAllAction->setShortcut(QKeySequence("Ctrl+A"));
	m_menu.addAction(selectAllAction);

	QAction* undoAction = new QAction(QString::fromLocal8Bit("����(undo)"), this);
	QShortcut* undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
	undoAction->setShortcut(QKeySequence("Ctrl+Z"));
	m_menu.addAction(undoAction);

	QAction* redoAction = new QAction(QString::fromLocal8Bit("����(redo)"), this);
	QShortcut* redoShortcut = new QShortcut(QKeySequence("Ctrl+Y"), this);
	redoAction->setShortcut(QKeySequence("Ctrl+Y"));
	m_menu.addAction(redoAction);

	QAction* cutAction = new QAction(QString::fromLocal8Bit("����"), this);
	QShortcut* cutShortcut = new QShortcut(QKeySequence("Ctrl+X"), this);
	cutAction->setShortcut(QKeySequence("Ctrl+X"));
	m_menu.addAction(cutAction);

	QAction* copyAction = new QAction(QString::fromLocal8Bit("����"), this);
	QShortcut* copyShortcut = new QShortcut(QKeySequence("Ctrl+C"), this);
	copyAction->setShortcut(QKeySequence("Ctrl+C"));
	m_menu.addAction(copyAction);

	QAction* pasteAction = new QAction(QString::fromLocal8Bit("ճ��"), this);
	QShortcut* pasteShortcut = new QShortcut(QKeySequence("Ctrl+V"), this);
	pasteAction->setShortcut(QKeySequence("Ctrl+V"));
	m_menu.addAction(pasteAction);

	QAction* duplicateAction = new QAction(QString::fromLocal8Bit("��д"), this);
	QShortcut* duplicateShortcut = new QShortcut(QKeySequence("Ctrl+D"), this);
	duplicateAction->setShortcut(QKeySequence("Ctrl+D"));
	m_menu.addAction(duplicateAction);

	QAction* deleteAction = new QAction(QString::fromLocal8Bit("ɾ��"), this);
	QShortcut* deleteShortcut = new QShortcut(QKeySequence("Backspace"), this);
	deleteAction->setShortcut(QKeySequence("Backspace"));
	m_menu.addAction(deleteAction);

	QAction* moveUpLayerAction = new QAction(QString::fromLocal8Bit("����һ��"), this);
	QShortcut* moveUpLayerShortcut = new QShortcut(QKeySequence("Up"), this);
	moveUpLayerAction->setShortcut(QKeySequence("Up"));
	m_menu.addAction(moveUpLayerAction);

	QAction* moveDownLayerAction = new QAction(QString::fromLocal8Bit("����һ��"), this);
	QShortcut* moveDownLayerShortcut = new QShortcut(QKeySequence("Down"), this);
	moveDownLayerAction->setShortcut(QKeySequence("Down"));
	m_menu.addAction(moveDownLayerAction);

	QAction* moveToFrontAction = new QAction(QString::fromLocal8Bit("����ǰ"), this);
	QShortcut* moveToFrontShortcut = new QShortcut(QKeySequence("Left"), this);
	moveToFrontAction->setShortcut(QKeySequence("Left"));
	m_menu.addAction(moveToFrontAction);

	QAction* moveToBackAction = new QAction(QString::fromLocal8Bit("�����"), this);
	QShortcut* moveToBackShortcut = new QShortcut(QKeySequence("Right"), this);
	moveToBackAction->setShortcut(QKeySequence("Right"));
	m_menu.addAction(moveToBackAction);

	(void)connect(selectAllShortcut, &QShortcut::activated, m_pCanvas, &KCanvas::onSelectAll);
	(void)connect(cutShortcut, &QShortcut::activated, m_pCanvas, &KCanvas::onCut);
	(void)connect(copyShortcut, &QShortcut::activated, m_pCanvas, &KCanvas::onCopy);
	(void)connect(pasteShortcut, &QShortcut::activated, m_pCanvas, &KCanvas::onPaste);
	(void)connect(duplicateShortcut, &QShortcut::activated, m_pCanvas, &KCanvas::onDuplicate);
	(void)connect(deleteShortcut, &QShortcut::activated, m_pCanvas, &KCanvas::onDelete);
	(void)connect(moveUpLayerShortcut, &QShortcut::activated, m_pCanvas, &KCanvas::onUpLayer);
	(void)connect(moveDownLayerShortcut, &QShortcut::activated, m_pCanvas, &KCanvas::onDownLayer);
	(void)connect(moveToFrontShortcut, &QShortcut::activated, m_pCanvas, &KCanvas::onMostFront);
	(void)connect(moveToBackShortcut, &QShortcut::activated, m_pCanvas, &KCanvas::onMostBack);

	(void)connect(selectAllAction, &QAction::triggered, m_pCanvas, &KCanvas::onSelectAll);
	(void)connect(cutAction, &QAction::triggered, m_pCanvas, &KCanvas::onCut);
	(void)connect(copyAction, &QAction::triggered, m_pCanvas, &KCanvas::onCopy);
	(void)connect(pasteAction, &QAction::triggered, m_pCanvas, &KCanvas::onPaste);
	(void)connect(duplicateAction, &QAction::triggered, m_pCanvas, &KCanvas::onDuplicate);
	(void)connect(deleteAction, &QAction::triggered, m_pCanvas, &KCanvas::onDelete);
	(void)connect(moveUpLayerAction, &QAction::triggered, m_pCanvas, &KCanvas::onUpLayer);
	(void)connect(moveDownLayerAction, &QAction::triggered, m_pCanvas, &KCanvas::onDownLayer);
	(void)connect(moveToFrontAction, &QAction::triggered, m_pCanvas, &KCanvas::onMostFront);
	(void)connect(moveToBackAction, &QAction::triggered, m_pCanvas, &KCanvas::onMostBack);
}

void KSvgMainWindow::contextMenuEvent(QContextMenuEvent* event)
{
	m_menu.exec(event->globalPos());
}

