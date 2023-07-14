#ifndef PLOTAXISOFFSETHANDLE_H
#define PLOTAXISOFFSETHANDLE_H

#include "plotwidget.h"
#include <QWidget>
#include "gui/plot_line_handle.h"
#include "scopy-testplugin_export.h"

namespace scopy {
class SCOPY_TESTPLUGIN_EXPORT PlotAxisOffsetHandle : public QObject
{
	Q_OBJECT
public:
	PlotAxisOffsetHandle(QColor color, PlotAxis* ax, PlotWidget *p, QObject *parent = nullptr);
	~PlotAxisOffsetHandle();
	RoundedHandleV *offsetHdl() const;

private:
	PlotWidget *p;
	HorizBar *m_chOffsetBar;
	RoundedHandleV *m_chOffsetHdl;
	SymbolController* m_symbolCtrl;

};
}
#endif // PLOTAXISOFFSETHANDLE_H
