#ifndef TESTTOOL_H
#define TESTTOOL_H

#include <QWidget>
#include "toolbuilder.h"
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <scopy-testplugin_export.h>

using namespace scopy;

class SCOPY_TESTPLUGIN_EXPORT TestTool : public QWidget
{
public:
	TestTool(QWidget *parent = nullptr);

private:
	ToolTemplate *tool;
	QPushButton *btn1, *btn2, *btn3;
};
//};
#endif // TESTTOOL_H
