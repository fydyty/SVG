#ifndef __K_PARAMBOX_H_
#define __K_PARAMBOX_H_

#include <QPushButton>
#include <QColorDialog>

#include "kparambox.h"

class KColorBox : public KParamBox
{
	Q_OBJECT

public:
	KColorBox(const QString &title,QString &rgbColorStr,
		QWidget *parent);
	~KColorBox();

	// ����ͷ��¼�,����ͷ�ʱ������ɫѡ��Ի���
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	QString getStr();
signals:
	void pickedColor();
private slots:
	void showPickColorDlg();
private:
	QPushButton* m_pColorBtn;
	QString m_str;
};

#endif
