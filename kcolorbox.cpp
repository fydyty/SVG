#include "kcolorbox.h"


KColorBox::KColorBox(const QString &title,QString &rgbColorStr,QWidget *parent)
	: KParamBox(title,parent)
	, m_pColorBtn(Q_NULLPTR)
{
	m_pColorBtn = new QPushButton(this);
	m_pColorBtn->setMaximumSize(70, 50);
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(rgbColorStr));
	m_pVLayout->addWidget(m_pColorBtn); // ���ӵ�������
	m_str = rgbColorStr;
	(void)connect(m_pColorBtn, &QPushButton::clicked, this, &KColorBox::showPickColorDlg);
}

KColorBox::~KColorBox()
{
}

void KColorBox::mouseReleaseEvent(QMouseEvent* event)
{
	m_pColorBtn->click();
}

QString KColorBox::getStr()
{
	return m_str;
}

void KColorBox::showPickColorDlg()
{
	// QColorDialog : ϵͳʵ����ɫѡ���
	QColor color = QColorDialog::getColor(QColor(m_str.toUInt(NULL, 16)), nullptr, QStringLiteral("ѡ����ɫ"), QColorDialog::ShowAlphaChannel);

	if (!color.isValid())
		return;

	m_str = QString::number(color.rgb(), 16); // �� 16���Ƶ���ɫֵת���� str �ַ���

	// ����ǰ��ɫֵ���浽ȫ�����ݶ�����
	//KGlobalData::getGlobalDataIntance()->setCanvasColor(str);

	// ���ð�ť������ɫΪ��ǰѡ�����ɫ
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(m_str));

	emit pickedColor(); // �����ź�	
}

