#include "kvaluebox.h"
#include <QRegExpValidator>

KValueBox::KValueBox(const QString &title,const int val,QWidget *parent)
	: KParamBox(title,parent)
	, m_pParamEdit(Q_NULLPTR)
{
	m_pParamEdit = new QLineEdit(this);
	m_pParamEdit->setText(QString::number(val));
	m_pParamEdit->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	m_pParamEdit->setStyleSheet("background-color:#3f3f3c; font-size:20px; color:#4f80ff; font-family: Microsoft YaHei; font-weight: bold; border-width:0; border-style:outset");
	m_pParamEdit->setMaxLength(4);

	QRegularExpression regex("^\\d+\\.?\\d*$");
	QValidator* validator = new QRegularExpressionValidator(regex, this);
	m_pParamEdit->setValidator(validator);

	m_pVLayout->addWidget(m_pParamEdit);
}

KValueBox::~KValueBox()
{
}

void KValueBox::focusInEvent(QFocusEvent* event)
{
	m_pParamEdit->selectAll();// 全选文本
}
