#ifndef TOOLBUILDER_H
#define TOOLBUILDER_H

#include <QWidget>
#include "ui_tooltemplate.h"

namespace Ui {
class ToolTemplate;
}

namespace scopy {

enum ToolTemplateAlignment {
	TTA_LEFT,
	TTA_RIGHT
};
enum ToolTemplateContainer {
	TTC_LEFT,
	TTC_RIGHT,
	TTC_TOP,
	TTC_BOT
};

class ToolTemplate : public QWidget
{
public:
	ToolTemplate(QWidget *parent = nullptr);
	~ToolTemplate();

	QWidget* bottomContainer();
	QWidget* topContainer();
	QWidget* leftContainer();
	QWidget *rightContainer();
	QWidget* topContainerMenuControl();

	void setLeftContainerWidth(int w);
	void setRightContainerWidth(int w);

	void addWidgetToTopContainerHelper(QWidget *w, enum ToolTemplateAlignment);
	void addWidgetToTopContainerMenuControlHelper(QWidget *w, ToolTemplateAlignment a);
	void addWidgetToBottomContainerHelper(QWidget *w, ToolTemplateAlignment a);

	Ui::ToolTemplate *m_ui;
};
}

#endif // TOOLBUILDER_H
