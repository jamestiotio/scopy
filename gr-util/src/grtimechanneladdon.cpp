#include "grtimechanneladdon.h"
#include "grdeviceaddon.h"
#include <QDebug>

using namespace scopy::grutil;

GRTimeChannelAddon::GRTimeChannelAddon(GRSignalPath *path, GRTimePlotAddon *plotAddon, QPen pen, QObject *parent)
	: QObject(parent), m_signalPath(path), m_plotAddon(plotAddon), m_pen(pen) {
	this->m_grch = dynamic_cast<GRIIOChannel*>(path->path()[0]);
	name = m_grch->getChannelName();
	widget = new QLabel(name);
	auto plot = plotAddon->plot();;

	m_plotAxis = new PlotAxis(QwtAxis::YLeft, plot, this);
	m_plotAxis->setInterval(0,1<<24);
	m_plotCh = new PlotChannel(name, pen, plot, plot->xAxis(), m_plotAxis,this);
	m_plotAxisHandle = new PlotAxisHandle(pen, m_plotAxis,plot,this);
	m_plotCh->setHandle(m_plotAxisHandle);
	plot->addPlotAxisHandle(m_plotAxisHandle);
}

GRTimeChannelAddon::~GRTimeChannelAddon() {}

QString GRTimeChannelAddon::getName() {return name;}

QWidget *GRTimeChannelAddon::getWidget() {return widget;}

void GRTimeChannelAddon::setDevice(GRDeviceAddon *d) { m_dev = d; d->registerChannel(this);}

GRDeviceAddon* GRTimeChannelAddon::getDevice() { return m_dev;}

void GRTimeChannelAddon::enable() {
	qInfo()<<name<<" enabled";
	m_plotCh->attach();
	m_plotAxisHandle->handle()->setVisible(true);
	m_plotAxisHandle->handle()->raise();
	m_signalPath->setEnabled(true);
	m_grch->setEnabled(true);
}


void GRTimeChannelAddon::disable() {
	qInfo()<<name<<" disabled";
	m_plotCh->detach();
	m_plotAxisHandle->handle()->setVisible(false);
	m_signalPath->setEnabled(false);
	m_grch->setEnabled(false);
}

void GRTimeChannelAddon::onStart() {}

void GRTimeChannelAddon::onStop() {}

void GRTimeChannelAddon::onAdd() {  }

void GRTimeChannelAddon::onRemove() {}

void GRTimeChannelAddon::onChannelAdded(ToolAddon *) {}

void GRTimeChannelAddon::onChannelRemoved(ToolAddon *) {}

PlotChannel *GRTimeChannelAddon::plotCh() const
{
	return m_plotCh;
}

GRSignalPath *GRTimeChannelAddon::signalPath() const
{
	return m_signalPath;
}

QPen GRTimeChannelAddon::pen() const {
	return m_pen;
}
