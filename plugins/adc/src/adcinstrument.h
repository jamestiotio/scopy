#ifndef ADCINSTRUMENT_H
#define ADCINSTRUMENT_H

#include <QWidget>
#include <adcplugin.h>
#include "gui/tooltemplate.h"
#include <gui/widgets/toolbuttons.h>
#include <QPushButton>

namespace scopy {
class AdcInstrument : public QWidget
{
	Q_OBJECT
public:
	AdcInstrument(PlotProxy *proxy, QWidget *parent = nullptr);
	~AdcInstrument();
	void init();
	void deinit();
public Q_SLOTS:
	void run(bool);
	void stop();
private:
	RunBtn *runBtn;
	SingleShotBtn *singleBtn;
	ToolTemplate *tool;
	PlotProxy* proxy;
	QPushButton *openLastMenuBtn;
};
}
#endif // ADCINSTRUMENT_H
