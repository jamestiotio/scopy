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
	PlotChannel(QString name, QPen pen, PlotWidget *plot, PlotAxis* x_Axis, PlotAxis *y_Axis, QObject *parent = nullptr);
	~PlotChannel();

	QwtPlotCurve *curve() const;
public Q_SLOTS:
	void setEnabled(bool b);
	void enable();
	void disable();

private:
	PlotAxis *x_axis, *y_axis;
	QwtPlotCurve *m_curve;
	QwtSymbol *symbol;
	PlotWidget *m_plotWidget;
	QwtPlot *m_plot;
	float *m_data;
};
}

#endif // PLOTCHANNEL_H
