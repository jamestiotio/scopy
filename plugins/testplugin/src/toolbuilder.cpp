#include "toolbuilder.h"

using namespace scopy;
ToolTemplate::ToolTemplate(QWidget *parent) : QWidget(parent)
{
	m_ui = new Ui::ToolTemplate();
	m_ui->setupUi(this);
	m_ui->bottomContainer->setVisible(false);
	m_ui->topContainer->setVisible(false);
	m_ui->leftContainer->setVisible(false);
	m_ui->rightContainer->setVisible(false);
}

ToolTemplate::~ToolTemplate()
{
	delete m_ui;
}

QWidget *ToolTemplate::bottomContainer()
{
	return m_ui->bottomContainer;
}

QWidget *ToolTemplate::topContainer()
{
	return m_ui->topContainer;
}

QWidget *ToolTemplate::rightContainer()
{
	return m_ui->rightContainer;
}

QWidget *ToolTemplate::topContainerMenuControl()
{
	return m_ui->topContainerMenuControl;
}

QWidget *ToolTemplate::leftContainer()
{
	return m_ui->leftContainer;
}

void ToolTemplate::setLeftContainerWidth(int w)
{
	m_ui->leftContainer->setMaximumWidth(w);
	m_ui->leftContainer->setMinimumWidth(w);
}

void ToolTemplate::setRightContainerWidth(int w)
{
	m_ui->topContainerMenuControl->setMaximumWidth(w);
	m_ui->topContainerMenuControl->setMinimumWidth(w);
	dynamic_cast<MenuAnim*>(m_ui->rightContainer)->setAnimMinWidth(0);
	dynamic_cast<MenuAnim*>(m_ui->rightContainer)->setAnimMaxWidth(w);
	m_ui->rightContainer->setMaximumWidth(w);
	m_ui->rightContainer->setMinimumWidth(w);
}

void ToolTemplate::addWidgetToTopContainerHelper(QWidget *w, ToolTemplateAlignment a)
{
	auto lay1 = static_cast<QHBoxLayout*>(topContainer()->layout());
	auto idx = lay1->indexOf(m_ui->topContainerSpacer);
	int offset;
	if(a == TTA_LEFT) {
		offset = 0;
	} else {
		offset = 1;
	}
	lay1->insertWidget(idx+offset,w);
}

void ToolTemplate::addWidgetToTopContainerMenuControlHelper(QWidget *w, ToolTemplateAlignment a) {
	auto lay1 = static_cast<QHBoxLayout*>(topContainerMenuControl()->layout());
	auto idx = lay1->indexOf(m_ui->topContainerMenuControlSpacer);
	int offset;
	if(a == TTA_LEFT) {
		offset = 0;
	} else {
		offset = 1;
	}
	lay1->insertWidget(idx+offset,w);
}

void ToolTemplate::addWidgetToBottomContainerHelper(QWidget *w, ToolTemplateAlignment a)
{
	auto lay1 = static_cast<QHBoxLayout*>(bottomContainer()->layout());
	auto idx = lay1->indexOf(m_ui->bottomContainerSpacer);
	int offset;
	if(a == TTA_LEFT) {
		offset = 0;
	} else {
		offset = 1;
	}
	lay1->insertWidget(idx+offset,w);
}




