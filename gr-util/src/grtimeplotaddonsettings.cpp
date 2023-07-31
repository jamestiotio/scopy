#include "grtimeplotaddonsettings.h"
#include <gui/widgets/menuheader.h>
#include <gui/widgets/menusectionwidget.h>
#include <gui/widgets/menulineedit.h>
#include <gui/widgets/menucollapsesection.h>
#include <gui/widgets/menuonoffswitch.h>
#include <gui/widgets/menucombo.h>
#include <QVBoxLayout>

using namespace scopy::grutil;


QWidget* GRTimePlotAddonSettings::createMenu(QWidget* parent) {
	QWidget *w = new QWidget(parent);
	QVBoxLayout *lay = new QVBoxLayout(w);
	lay->setMargin(0);
	lay->setSpacing(10);
	w->setLayout(lay);

	m_pen = QPen(StyleHelper::getColor("ScopyBlue"));

	MenuHeaderWidget *header = new MenuHeaderWidget("PLOT", m_pen, w);
	QWidget* xaxismenu = createXAxisMenu(w);
//	QWidget* curvemenu = createCurveMenu(w);

	lay->addWidget(header);
	lay->addWidget(xaxismenu);
//	lay->addWidget(curvemenu);

	lay->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Expanding));


	return w;
}


QWidget* GRTimePlotAddonSettings::createXAxisMenu(QWidget* parent) {
	MenuSectionWidget *xaxiscontainer = new MenuSectionWidget(parent);
	MenuCollapseSection *xaxis = new MenuCollapseSection("X-AXIS",MenuCollapseSection::MHCW_NONE, xaxiscontainer);

	QWidget *bufferPlotSize = new QWidget(xaxis);
	QHBoxLayout *bufferPlotSizeLayout = new QHBoxLayout(bufferPlotSize);
	bufferPlotSizeLayout->setMargin(0);
	bufferPlotSizeLayout->setSpacing(10);
	bufferPlotSize->setLayout(bufferPlotSizeLayout);

	m_bufferSizeSpin = new ScaleSpinButton(
		{
		 {"samples",1e0},
		 {"ksamples",1e3},
		 {"Msamples",1e6},
		 },"Buffer Size",16,(double)((long)1<<31),false,false,bufferPlotSize);


	m_plotSizeSpin = new ScaleSpinButton(
		{
		 {"samples",1e0},
		 {"ksamples",1e3},
		 {"Msamples",1e6},
		 {"Gsamples",1e9},
		 },"Plot Size",16,(double)((long)1<<31),false,false,bufferPlotSize);
	bufferPlotSizeLayout->addWidget(m_bufferSizeSpin);
	bufferPlotSizeLayout->addWidget(m_plotSizeSpin);

	MenuOnOffSwitch *syncBufferPlot = new MenuOnOffSwitch(tr("SYNC BUFFER-PLOT SIZES"), xaxis, false);
	MenuOnOffSwitch *rollingMode = new MenuOnOffSwitch(tr("ROLLING MODE"), xaxis, false);


	QWidget *xMinMax = new QWidget(xaxis);
	QHBoxLayout *xMinMaxLayout = new QHBoxLayout(xMinMax);
	xMinMaxLayout->setMargin(0);
	xMinMaxLayout->setSpacing(10);
	xMinMax->setLayout(xMinMaxLayout);

	m_xmin = new PositionSpinButton(
		{
		 {"V",1e0},
		 {"k",1e3},
		 {"M",1e6},
		 {"G",1e9},
		 },"XMin",(double)((long)(-1<<31)),(double)((long)1<<31),false,false,xMinMax);


	m_xmax = new PositionSpinButton(
		{
		 {"V",1e0},
		 {"k",1e3},
		 {"M",1e6},
		 {"G",1e9},
		 },"XMax",(double)((long)(-1<<31)),(double)((long)1<<31),false,false,xMinMax);

	xMinMaxLayout->addWidget(m_xmin);
	xMinMaxLayout->addWidget(m_xmax);

	MenuCombo *cbb = new MenuCombo("XMode", xaxis);
	auto cb = cbb->combo();

	cb->addItem("Samples", XMODE_SAMPLES);
	if(m_sampleRateAvailable) {
		cb->addItem("Time", XMODE_TIME);
	}
	cb->addItem("Time - override samplerate", XMODE_OVERRIDE);

	m_sampleRateSpin = new PositionSpinButton(
		{
		 {"Hz",1e0},
		 {"kHz",1e3},
		 {"MHz",1e6},
		 {"GHz",1e9},
		 },"SampleRate",1,(double)((long)1<<31),false,false,xaxis);

	xaxiscontainer->contentLayout()->setSpacing(10);
	xaxiscontainer->contentLayout()->addWidget(xaxis);
	xaxis->contentLayout()->addWidget(bufferPlotSize);
	xaxis->contentLayout()->addWidget(syncBufferPlot);
	xaxis->contentLayout()->addWidget(rollingMode);
	xaxis->contentLayout()->addWidget(xMinMax);
	xaxis->contentLayout()->addWidget(cbb);
	xaxis->contentLayout()->addWidget(m_sampleRateSpin);
	xaxis->contentLayout()->setSpacing(10);

	return xaxiscontainer;
}

GRTimePlotAddonSettings::GRTimePlotAddonSettings(GRTimePlotAddon *p, QObject *parent) :
      QObject(parent),p(p) {
	name = p->getName()+"_settings";
	m_sampleRateAvailable = false;
	widget = createMenu();
}

GRTimePlotAddonSettings::~GRTimePlotAddonSettings() {}

QString GRTimePlotAddonSettings::getName() { return name;}

QWidget *GRTimePlotAddonSettings::getWidget() { return widget;}
