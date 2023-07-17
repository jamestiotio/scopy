#include "grtimeplotaddon.h"
#include <grlog.h>
#include <grtimechanneladdon.h>
#include <grdeviceaddon.h>
#include <grtopblock.h>
#include <grlog.h>
#include <gr-gui/scope_sink_f.h>
#include <gui/plotwidget.h>

using namespace scopy;
using namespace scopy::grutil;
GRTimePlotAddon::GRTimePlotAddon(QString name, GRTopBlock *top, QObject *parent) : QObject(parent), m_top(top){

	this->name = name;
	m_plotWidget = new PlotWidget();
	m_plotWidget->leftHandlesArea()->setVisible(true);
	m_plotWidget->bottomHandlesArea()->setVisible(true);
	widget = m_plotWidget;
}

GRTimePlotAddon::~GRTimePlotAddon() { }

QString GRTimePlotAddon::getName() { return name; }

QWidget *GRTimePlotAddon::getWidget() { return widget; }

PlotWidget *GRTimePlotAddon::plot() { return m_plotWidget;}

void GRTimePlotAddon::enable() {}

void GRTimePlotAddon::disable() {}

void GRTimePlotAddon::onStart() {
	connect(m_top,SIGNAL(builtSignalPaths()), this, SLOT(connectSignalPaths()));
	connect(m_top,SIGNAL(teardownSignalPaths()), this, SLOT(tearDownSignalPaths()));
	// connect sink stopped ?

	m_top->build();
	m_top->start();

	// connect timer to time sink to get data - or data driven - connect to sink_new data
	// start timer


}

/* void GRTimePlotAddon::getData() {
	data = sink.getData(); // copies data from sink to plot
	replot();
	// Q_EMIT replot ?

*/



 void GRTimePlotAddon::onStop() {
	m_top->stop();
	m_top->teardown();
	disconnect(m_top,SIGNAL(builtSignalPaths()), this, SLOT(connectSignalPaths()));
	disconnect(m_top,SIGNAL(teardownSignalPaths()), this, SLOT(tearDownSignalPaths()));
}

void GRTimePlotAddon::onAdd() {}

void GRTimePlotAddon::onRemove() {}

void GRTimePlotAddon::onChannelAdded(ToolAddon *t) {

}

void GRTimePlotAddon::onChannelRemoved(ToolAddon *) {
}

void GRTimePlotAddon::connectSignalPaths() {
	QList<GRSignalPath*> sigpaths;
	for(auto &sigpath : m_top->signalPaths()) {
		qInfo()<<sigpath->name();
		if(!sigpath->enabled())
			continue;
		if(!sigpath->name().startsWith(name))
			continue;
		sigpaths.append(sigpath);


		qInfo()<<"created scope_sink_f with name" << sigpath->name();

	}



	// create and configure time_sink_f
	// allocate memory for data to be plotted - could be done by time_sink ?
	// 	for(int i=0;i<sigpaths.count();i++) {
	//	QwtCurve[].setRawSamples(data[i]) // just get pointer from time_sink
	// }

	//auto sink = scope_sink_f::make(1024,1000,name.toStdString(),sigpaths.count(),m_plot);


	int i=0;
	for(auto &sigpath : sigpaths) {
		//m_top->connect(sigpath->getGrEndPoint(), 0, sink, i);
		i++;
	}

	for(int i=0;i<sigpaths.count();i++) {


	}
	// TO DO: Give user the option to make these axes visible
	//	m_plot->setUsingLeftAxisScales(false);

}

void GRTimePlotAddon::tearDownSignalPaths() {
	for(auto &sink : sinks) {
	}
	sinks.clear();

}

void GRTimePlotAddon::onNewData() {
	float sum0 = 0;
	float sum1 = 0;	
}
