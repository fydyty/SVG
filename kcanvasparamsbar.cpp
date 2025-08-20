#include "kcanvasparamsbar.h"

KCanvasParamsBar::KCanvasParamsBar(const QString &title,QWidget *parent)
	: QWidget(parent)
	, m_pWidthBox(Q_NULLPTR)
	, m_pHeightBox(Q_NULLPTR)
	, m_pColorBox(Q_NULLPTR)
	, m_pVBoxLayout(Q_NULLPTR)
	, m_pGridLayout(Q_NULLPTR)
	, m_pTitleLabel(Q_NULLPTR)
{
	setMaximumWidth(170);
	setFocusPolicy(Qt::ClickFocus);//������ý���
	setStyleSheet("background-color:#2f2f2c");

	m_pVBoxLayout = new QVBoxLayout(this);
	m_pVBoxLayout->setContentsMargins(10, 0, 10, 0);
	m_pVBoxLayout->setAlignment(Qt::AlignTop);

	m_pTitleLabel = new QLabel(title, this);
	m_pTitleLabel->setStyleSheet("background-color:#2f2f2c; font-size:14px; color:#FFFFFF; font-family: Microsoft YaHei");

	m_pGridLayout = new QGridLayout(this);
	m_pGridLayout->setSpacing(10);

	m_pWidthBox = new KValueBox(QStringLiteral("���"), KGlobalData::getGlobalDataIntance()->getCanvasWidth(), this);
	m_pHeightBox = new KValueBox(QStringLiteral("�߶�"), KGlobalData::getGlobalDataIntance()->getCanvasHeight(), this);
	m_pScaleBox = new KValueBox(QStringLiteral("����"), KGlobalData::getGlobalDataIntance()->getCanvasScale(), this);
	m_pPenWidthBox = new KValueBox(QStringLiteral("���"), KGlobalData::getGlobalDataIntance()->getShapePenWidth(), this);
	m_pColorBox = new KColorBox(QStringLiteral("������ɫ"), KGlobalData::getGlobalDataIntance()->getCanvasColor(), this);
	m_pColorPenBox = new KColorBox(QStringLiteral("�߿���ɫ"), KGlobalData::getGlobalDataIntance()->getPenColor(), this);
	m_pColorBrushBox = new KColorBox(QStringLiteral("�����ɫ"), KGlobalData::getGlobalDataIntance()->getBrushColor(), this);
	m_pStyleBox = new KStyleBox(QStringLiteral("��ʽ"), this);

	// ��ӵ����񲼾�
	m_pGridLayout->addWidget(m_pWidthBox, 0, 0);// �� 0 �� �� 0 ��
	m_pGridLayout->addWidget(m_pHeightBox, 0, 1);// �� 0 �� �� 1 ��
	m_pGridLayout->addWidget(m_pScaleBox, 1, 0);
	m_pGridLayout->addWidget(m_pColorBox, 1, 1);
	m_pGridLayout->addWidget(m_pPenWidthBox, 2, 0);
	m_pGridLayout->addWidget(m_pStyleBox, 2, 1);
	m_pGridLayout->addWidget(m_pColorPenBox, 3, 0);
	m_pGridLayout->addWidget(m_pColorBrushBox, 3, 1);

	m_pVBoxLayout->addWidget(m_pTitleLabel);
	m_pVBoxLayout->addLayout(m_pGridLayout);

	setLayout(m_pVBoxLayout);
}

KCanvasParamsBar::~KCanvasParamsBar()
{
}
