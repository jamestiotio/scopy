#ifndef PLOTAXIS_H
#define PLOTAXIS_H

#include "scopy-testplugin_export.h"
#include <QwtScaleEngine>
#include "plot.h"
#include <DisplayPlot.h>
#include "osc_scale_engine.h"

namespace scopy {
class SCOPY_TESTPLUGIN_EXPORT PlotAxis : public QObject {
	Q_OBJECT
public:
	PlotAxis(int position, Plot *p, QObject *parent = nullptr);
	~PlotAxis() {}

	int position();
	bool isHorizontal();
	bool isVertical();
	void setDivs(double);
	void setInterval(double min, double max);

	const QwtAxisId &axisId() const;

public Q_SLOTS:
	void setVisible(bool);
	void updateAxisScale();


private:
	QwtAxisId m_axisId;
	Plot *m_plot;
	int m_position;
	OscScaleDraw *m_xScaleDraw;
	OscScaleEngine *m_xScaleEngine;
	int m_id;
	double m_divs;
	double m_min;
	double m_max;

};
}
#endif // PLOTAXIS_H
