#include "plotaxis.h"

using namespace scopy;
PlotAxis::PlotAxis(int position, Plot *p, QObject *parent) :
      QObject(parent), m_plot(p), m_position(position), m_axisId(QwtAxisId(position))
{
	m_min = -1;
	m_max = 1;

	if(isHorizontal()) {
		m_divs = 16.0;
		m_id = m_plot->horizontalPlotAxis().count();
	} else {
		m_divs = 10;
		m_id = m_plot->verticalPlotAxis().count();
	}

	m_axisId = QwtAxisId(m_position, m_id);

	m_xScaleDraw = new OscScaleDraw(new MetricPrefixFormatter(),"");    // set Formatter (?)
	m_plot->setAxisScaleDraw(m_axisId,m_xScaleDraw);

	m_xScaleEngine = new OscScaleEngine();
	m_plot->setAxisScaleEngine(m_axisId, (QwtScaleEngine *)m_xScaleEngine);
	updateAxisScale();
	setVisible(true);

}

int PlotAxis::position()
{
	return m_position;
}

bool PlotAxis::isHorizontal()
{
	return (m_position == QwtAxis::XBottom || m_position == QwtAxis::XTop);
}

bool PlotAxis::isVertical()
{
	return (m_position == QwtAxis::YLeft || m_position == QwtAxis::YRight);
}

void PlotAxis::setDivs(double divs) {
	m_divs = divs;
	updateAxisScale();

}

void PlotAxis::setInterval(double min, double max) {
	m_min = min;
	m_max = max;
	updateAxisScale();
}

void PlotAxis::setVisible(bool val)
{
	m_plot->setAxisVisible(m_axisId, val);
}

void PlotAxis::updateAxisScale() {
	m_plot->setAxisScale(m_axisId, m_min, m_max, (m_max - m_min)/m_divs); // set Divs, limits
}

const QwtAxisId &PlotAxis::axisId() const
{
	return m_axisId;
}
