#include "grtimeplotaddon.h"
#include <grlog.h>
#include <grtimechanneladdon.h>
#include <grdeviceaddon.h>

using namespace scopy;
using namespace scopy::grutil;
GRTimePlotAddon::GRTimePlotAddon(QString name, QObject *parent) : QObject(parent){

	this->name = name;
	QWidget *m_plotWidget = new QWidget();

	m_plot = new CapturePlot(m_plotWidget);
	m_plotWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_plot->disableLegend();


	QGridLayout *gridPlot = new QGridLayout(m_plotWidget);
	gridPlot->setVerticalSpacing(0);
	gridPlot->setHorizontalSpacing(0);
	gridPlot->setContentsMargins(9, 0, 9, 0);
	m_plotWidget->setLayout(gridPlot);

	QSpacerItem *plotSpacer = new QSpacerItem(0, 5,
						  QSizePolicy::Fixed, QSizePolicy::Fixed);

	gridPlot->addWidget(m_plot->topArea(), 1, 0, 1, 4);
	gridPlot->addWidget(m_plot->leftHandlesArea(), 1, 0, 4, 1);
	gridPlot->addWidget(m_plot, 3, 1, 1, 1);
	gridPlot->addItem(plotSpacer, 5, 0, 1, 4);

	m_plot->setSampleRate(100, 1, "");
	m_plot->enableTimeTrigger(false);
	m_plot->setActiveVertAxis(0, true);
	m_plot->setAxisScale(QwtAxisId(QwtAxis::XBottom, 0), 0, 100);
	m_plot->setAllYAxis(0, 20000);
	m_plot->setOffsetInterval(-__DBL_MAX__, __DBL_MAX__);
	widget = m_plotWidget;
}

GRTimePlotAddon::~GRTimePlotAddon() { }

QString GRTimePlotAddon::getName() { return name; }

QWidget *GRTimePlotAddon::getWidget() { return widget; }

CapturePlot *GRTimePlotAddon::plot() { return m_plot;}

void GRTimePlotAddon::enable() {}

void GRTimePlotAddon::disable() {}

void GRTimePlotAddon::onStart() {}

void GRTimePlotAddon::onStop() {}

void GRTimePlotAddon::onAdd() {}

void GRTimePlotAddon::onRemove() {}

void GRTimePlotAddon::onChannelAdded(ToolAddon *t) {
	GRTimeChannelAddon *ch = dynamic_cast<GRTimeChannelAddon*>(t);
	m_plot->registerSink((ch->getDevice()->getName() +  t->getName()).toStdString(),1,1024);
}

void GRTimePlotAddon::onChannelRemoved(ToolAddon *) {}
