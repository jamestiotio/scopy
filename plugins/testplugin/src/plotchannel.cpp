#include "plotchannel.h"
#include <QPen>
#include "plotwidget.h"
#include "plotaxis.h"

using namespace scopy;

PlotChannel::PlotChannel(QString name, QPen pen, PlotWidget *plot, PlotAxis *x_axis, PlotAxis *y_axis, QObject *parent)
    : QObject(parent),
      m_plotWidget(plot),
      m_plot(m_plotWidget->plot()),
      x_axis(x_axis),
      y_axis(y_axis)
{
	m_curve = new QwtPlotCurve(name);
	m_curve->setAxes(x_axis->axisId(), y_axis->axisId());
	m_curve->setStyle( QwtPlotCurve::Lines );
	m_curve->setPen(pen);
	m_curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
	m_curve->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );

	m_curve->setItemAttribute(QwtPlotItem::Legend, true);
	symbol = new QwtSymbol(QwtSymbol::NoSymbol, QBrush(pen.color()),
					  QPen(pen.color()), QSize(7,7));
	m_curve->setSymbol(symbol);
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

//void PlotChannel::setAxes(PlotAxis *x_axis, PlotAxis *y_axis)
//{
//	m_curve->setAxes(x_axis->axisId(), y_axis->axisId());
//}

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


