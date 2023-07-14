#ifndef PLOTAXISHANDLE_H
#define PLOTAXISHANDLE_H

#include "plotwidget.h"
#include <QWidget>
#include "gui/plot_line_handle.h"
#include "scopy-testplugin_export.h"

namespace scopy {
class SCOPY_TESTPLUGIN_EXPORT PlotAxisHandle : public QObject
{
	Q_OBJECT
public:
	PlotAxisHandle(QPen pen, PlotAxis* ax, PlotWidget *p, QObject *parent = nullptr);
	~PlotAxisHandle();
	RoundedHandleV *offsetHdl() const;
	PlotAxis *axis() const;

private:
	PlotWidget *m_plotWidget;
	PlotAxis *m_axis;
	QPen m_pen;
	HorizBar *m_chOffsetBar;
	RoundedHandleV *m_chOffsetHdl;
	SymbolController* m_symbolCtrl;

};
}
#endif // PLOTAXISHANDLE_H
