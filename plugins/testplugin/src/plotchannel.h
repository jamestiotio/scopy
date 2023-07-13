#ifndef PLOTCHANNEL_H
#define PLOTCHANNEL_H

#include <QwtPlotCurve>
#include <QwtPlot>

namespace scopy {
class PlotAxis;
class PlotWidget;
class PlotChannel : public QObject {
	Q_OBJECT
public:
	PlotChannel(QString name, QPen pen, PlotWidget *plot, QObject *parent = nullptr);
	~PlotChannel();

	QwtPlotCurve *curve() const;
	void setAxes(PlotAxis *x_axis, PlotAxis *y_axis);

public Q_SLOTS:
	void setEnabled(bool b);
	void enable();
	void disable();

private:
	PlotAxis *x_axis, *y_axis;
	QwtPlotCurve *m_curve;
	PlotWidget *m_plotWidget;
	QwtPlot *m_plot;
	float *m_data;
};
}

#endif // PLOTCHANNEL_H
