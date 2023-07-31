#include "grtimeplotaddon.h"
#include <grlog.h>
#include <grtimechanneladdon.h>
#include <grdeviceaddon.h>
#include <grtopblock.h>
#include <grlog.h>
#include <gr-gui/scope_sink_f.h>
#include <gui/plotwidget.h>
#include <gui/buffer_previewer.hpp>
#include <QTimer>

using namespace scopy;
using namespace scopy::grutil;
GRTimePlotAddon::GRTimePlotAddon(QString name, GRTopBlock *top, QObject *parent) : QObject(parent), m_top(top){

	this->name = name;
	widget = new QWidget();
	m_lay = new QVBoxLayout(widget);
	m_plotWidget = new PlotWidget(widget);
	widget->setLayout(m_lay);

	m_plotWidget->xAxis()->setInterval(0,1);
	m_plotWidget->leftHandlesArea()->setVisible(true);
	m_plotWidget->rightHandlesArea()->setVisible(true);
	m_plotWidget->bottomHandlesArea()->setVisible(true);
//	m_plotWidget->topHandlesArea()->setVisible(true);

//	setupBufferPreviewer();
	m_lay->addWidget(m_plotWidget);
	m_plotTimer = new QTimer(this);
	connect(m_plotTimer, &QTimer::timeout, this, &GRTimePlotAddon::replot);

}

void GRTimePlotAddon::setupBufferPreviewer() {
	AnalogBufferPreviewer* m_bufferPreviewer = new AnalogBufferPreviewer(widget);
	m_bufferPreviewer->setMinimumHeight(20);
	m_bufferPreviewer->setCursorPos(0.5);
	m_bufferPreviewer->setHighlightPos(0.05);
	m_bufferPreviewer->setHighlightWidth(0.2);
	m_bufferPreviewer->setCursorVisible(false);
	m_bufferPreviewer->setWaveformPos(0.1);
	m_bufferPreviewer->setWaveformWidth(0.5);

//	connect(m_bufferPreviewer, &BufferPreviewer::bufferStopDrag, this, [=]() {
//		horiz_offset = m_bufferPreviewer->highlightPos();
//	});
//	connect(m_bufferPreviewer, &BufferPreviewer::bufferMovedBy, this, [=](int value) {
//		qInfo()<<value;
//		double moveTo = 0.0;
//		double min = xAxis()->min();
//		double max = xAxis()->max();
//		int width = m_bufferPreviewer->width();
//		double xA0xisWidth = max - min;

//		moveTo = value * xAxisWidth / width;
//		xAxis()->setInterval(min - moveTo, max - moveTo);
//		m_plot->replot();

//		auto delta = horiz_offset + (value/(float)width);

//		qInfo()<< delta << value << width;
//		m_bufferPreviewer->setHighlightPos(delta);


//		updateBufferPreviewer();
//	} );
	m_lay->addWidget(m_bufferPreviewer);
}

GRTimePlotAddon::~GRTimePlotAddon() { }

QString GRTimePlotAddon::getName() { return name; }

QWidget *GRTimePlotAddon::getWidget() { return widget; }

PlotWidget *GRTimePlotAddon::plot() { return m_plotWidget;}

void GRTimePlotAddon::enable() {}

void GRTimePlotAddon::disable() {}

void GRTimePlotAddon::onStart() {
	QElapsedTimer tim;
	tim.start();
	connect(m_top,SIGNAL(builtSignalPaths()), this, SLOT(connectSignalPaths()));
	connect(m_top,SIGNAL(teardownSignalPaths()), this, SLOT(tearDownSignalPaths()));
	// connect sink stopped ?
	m_top->build();
	m_top->start();

	int timeout = (1.0/60.0)*1000;

	m_plotTimer->setInterval(timeout);
	m_plotTimer->start();

}

void GRTimePlotAddon::replot() {
	// copies data from sink to plot
	time_sink->updateData();
	plot()->replot();
}

 void GRTimePlotAddon::onStop() {
	m_plotTimer->stop();
	m_top->stop();
	m_top->teardown();
	disconnect(m_top,SIGNAL(builtSignalPaths()), this, SLOT(connectSignalPaths()));
	disconnect(m_top,SIGNAL(teardownSignalPaths()), this, SLOT(tearDownSignalPaths()));
}

void GRTimePlotAddon::onAdd() {}

void GRTimePlotAddon::onRemove() {}

void GRTimePlotAddon::onChannelAdded(ToolAddon *t) {
	auto ch = dynamic_cast<GRTimeChannelAddon*> (t);
	if(ch)
		grChannels.append(ch);
}

void GRTimePlotAddon::onChannelRemoved(ToolAddon *t) {
	auto ch = dynamic_cast<GRTimeChannelAddon*> (t);
	if(ch)
		grChannels.removeAll(ch);
}

void GRTimePlotAddon::connectSignalPaths() {
	QList<GRSignalPath*> sigpaths;

	// for through grdevices - get sampleRate;

	for(auto &sigpath : m_top->signalPaths()) {
		qInfo()<<"Trying " << sigpath->name();
		if(!sigpath->enabled())
			continue;
		if(!sigpath->name().startsWith(name))
			continue;
		sigpaths.append(sigpath);
		qInfo()<<"Appended " << sigpath->name();

	}



	time_sink = time_sink_f::make(400,1000,name.toStdString(),sigpaths.count());
	// create and configure time_sink_f
	// allocate memory for data to be plotted - could be done by time_sink ?


	int i=0;
	/*	REGISTER SINK !*/
	for(GRTimeChannelAddon* gr : qAsConst(grChannels)) {
		if(gr->signalPath()->enabled()) {
			m_top->connect(gr->signalPath()->getGrEndPoint(), 0, time_sink, i);
			gr->plotCh()->curve()->setRawSamples(time_sink->time().data(), time_sink->data()[i].data(), 400);
			i++;
		}
	}
}

void GRTimePlotAddon::tearDownSignalPaths() {
	/*for(auto &sink : sinks) {
	}
	sinks.clear();*/

}

void GRTimePlotAddon::onNewData() {
	float sum0 = 0;
	float sum1 = 0;	
}
