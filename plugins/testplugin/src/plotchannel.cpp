#include "plotchannel.h"
#include <QPen>
#include "plot.h"
#include "plotaxis.h"

using namespace scopy;

PlotChannel::PlotChannel(QString name, QPen pen, Plot *plot, QObject *parent)
    : QObject(parent),
      m_plot(plot)
{
	m_curve = new QwtPlotCurve(name);

	m_curve->setStyle( QwtPlotCurve::Lines );
	m_curve->setPen(pen);
	m_curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
	m_curve->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
	// curvefitter (?)

}

PlotChannel::~PlotChannel()
{
	delete m_curve;
}

QwtPlotCurve *PlotChannel::curve() const
{
	return m_curve;
}

void PlotChannel::setAxes(PlotAxis *x_axis, PlotAxis *y_axis)
{
	m_curve->setAxes(x_axis->axisId(), y_axis->axisId());
}

void PlotChannel::setEnabled(bool b)
{
	if(b)
		m_curve->attach(m_plot);
	else
		m_curve->detach();
}

void PlotChannel::enable()
{
	setEnabled(true);
}

void PlotChannel::disable()
{
	setEnabled(false);
}


