#ifndef TIMEYAUTOSCALE_H
#define TIMEYAUTOSCALE_H

#include <QObject>
#include <QWidget>
#include <plotchannel.h>
#include <spinbox_a.hpp>
#include "scopy-gr-util_export.h"

namespace scopy::grutil {

class SCOPY_GR_UTIL_EXPORT TimeYAutoscale : public QObject {
	Q_OBJECT
public:
	TimeYAutoscale(QList<PlotChannel*>, QObject *parent = nullptr);
	~TimeYAutoscale();
Q_SIGNALS:
	void newMin(double);
	void newMax(double);

public Q_SLOTS:
	void start();
	void stop();
	void autoscale();

private:
	QTimer *m_autoScaleTimer;
	QList<PlotChannel*> m_channels;
};
}
#endif // TIMEYAUTOSCALE_H
