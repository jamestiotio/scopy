#ifndef TIMEYCONTROL_H
#define TIMEYCONTROL_H

#include <QWidget>
#include "scopy-gr-util_export.h"
#include "plotaxis.h"
#include "spinbox_a.hpp"

namespace scopy::grutil {

class SCOPY_GR_UTIL_EXPORT TimeYControl : public QWidget {
	Q_OBJECT
public:
	TimeYControl(PlotAxis*, QWidget *parent = nullptr);
	~TimeYControl();
	void setMin(double);
	void setMax(double);

private:
	PositionSpinButton *m_ymin;
	PositionSpinButton *m_ymax;

};
}

#endif // TIMEYCONTROL_H
