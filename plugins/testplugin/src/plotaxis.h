#ifndef PLOTAXIS_H
#define PLOTAXIS_H

#include "scopy-testplugin_export.h"
#include <QwtScaleEngine>
#include <QwtPlot>

class SCOPY_TESTPLUGIN_EXPORT PlotAxis : public QObject {
	Q_OBJECT
public:
	PlotAxis(QwtAxis::Position position, QwtPlot *p, QObject *parent = nullptr);
	~PlotAxis() {}
	void setDivs(double);
	void setInterval(double min, double max);


private:
	QwtPlot *m_plot;
	int axisId;

};
#endif // PLOTAXIS_H
