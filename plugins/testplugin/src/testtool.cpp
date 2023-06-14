#include "testtool.h"
#include <QHBoxLayout>
#include <gui/menu_anim.hpp>
#include <QDebug>
#include "stylehelper.h"
using namespace scopy;

QMap<QString, QColor> colorMap;

TestTool::TestTool(QWidget *parent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QHBoxLayout *lay = new QHBoxLayout(this);
	setLayout(lay);
	tool = new ToolTemplate(this);
	tool->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	tool->bottomContainer()->setVisible(true);
	tool->topContainer()->setVisible(true);
	tool->rightContainer()->setVisible(true);
	lay->addWidget(tool);

	StyleHelper::GetInstance()->initColorMap();
	btn3 = new OpenLastMenuBtn(dynamic_cast<MenuAnim*>(tool->rightContainer()),true,this);
	tool->setRightContainerWidth(300);

	GearBtn *btn5 = new GearBtn(this);
	RunBtn *runBtn = new RunBtn(this);
	InfoBtn *infoBtn = new InfoBtn(this);
	SingleShotBtn *singleBtn = new SingleShotBtn(this);
	PrintBtn *printBtn = new PrintBtn(this);

	MenuControlButton *ch0 = new MenuControlButton(this);
	ch0->setCheckBoxStyle(MenuControlButton::CS_CIRCLE);
	ch0->setName("Channel 0");
	ch0->setColor(StyleHelper::getColor("CH0"));
	ch0->checkBox()->setVisible(false);
	ch0->setChecked(true);

	MenuControlButton *ch1 = new MenuControlButton(this);
	ch1->setCheckBoxStyle(MenuControlButton::CS_CIRCLE);
	ch1->setName("Channel 1");
	ch1->setColor(StyleHelper::getColor("CH1"));
	ch1->button()->setVisible(false);
	ch1->checkBox()->setChecked(true);
	ch1->setChecked(true);

	MenuControlButton *ch2 = new MenuControlButton(this);
	ch2->setCheckBoxStyle(MenuControlButton::CS_CIRCLE);
	ch2->setName("Channel 2");
	ch2->setColor(StyleHelper::getColor("CH2"));

	MenuControlButton *cursor = new MenuControlButton(this);
	cursor->setName("Cursors");
	cursor->setCheckBoxStyle(MenuControlButton::CS_SQUARE);
	MenuControlButton *measure = new MenuControlButton(this);
	measure->setName("Measure");



	tool->addWidgetToTopContainerMenuControlHelper(btn3,TTA_RIGHT);
	tool->addWidgetToTopContainerMenuControlHelper(btn5,TTA_LEFT);

	tool->addWidgetToTopContainerHelper(singleBtn,TTA_RIGHT);
	tool->addWidgetToTopContainerHelper(runBtn,TTA_RIGHT);

	tool->addWidgetToTopContainerHelper(infoBtn,TTA_LEFT);
	tool->addWidgetToTopContainerHelper(printBtn,TTA_LEFT);

	tool->addWidgetToBottomContainerHelper(ch0, TTA_LEFT);
	tool->addWidgetToBottomContainerHelper(ch1, TTA_LEFT);
	tool->addWidgetToBottomContainerHelper(ch2, TTA_RIGHT);

	tool->addWidgetToBottomContainerHelper(cursor, TTA_RIGHT);
	tool->addWidgetToBottomContainerHelper(measure, TTA_LEFT);



}

