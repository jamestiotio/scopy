#ifndef PLOTCHANNEL_H
#define PLOTCHANNEL_H

#include <QwtPlotCurve>
#include <QwtPlot>

class PlotChannel : public QObject {
	Q_OBJECT
public:
	PlotChannel(QString name, QPen pen, QwtPlot *plot, QObject *parent = nullptr);
	~PlotChannel();

	QwtPlotCurve *curve() const;

public Q_SLOTS:
	void setEnabled(bool b);
	void enable();
	void disable();

private:
	QwtPlotCurve *m_curve;
	QwtPlot *m_plot;
	float *m_data;
};

#endif // PLOTCHANNEL_H
