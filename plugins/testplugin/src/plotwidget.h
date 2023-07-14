#ifndef PLOT_H
#define PLOT_H
#include <QwtPlot>
#include "plotchannel.h"
#include <QWidget>
#include <graticule.h>
#include "scopy-testplugin_export.h"
#include <symbol_controller.h>
#include "handles_area.hpp"
#include <QGridLayout>
#include "plotaxisoffsethandle.h"

namespace scopy {
class PlotAxis;
class PlotAxisOffsetHandle;

class SCOPY_TESTPLUGIN_EXPORT PlotWidget : public QWidget {
	Q_OBJECT
public:
	PlotWidget(QWidget *parent = nullptr); // add handleArea, connect handle to SetValue() add ChannelHandles (/w axis), add Zoomer, add cursorHandles
	~PlotWidget();

	void addPlotChannel(PlotChannel *ch);
	void removePlotChannel(PlotChannel *ch);

	void addPlotAxis(PlotAxis *ax);
	void removePlotAxis(PlotAxis *ax);

	bool getDisplayGraticule() const;
	void setDisplayGraticule(bool newDisplayGraticule);
	bool eventFilter(QObject *object, QEvent *event) override;

	const QList<PlotAxis *> &horizontalPlotAxis() const;
	const QList<PlotAxis *> &verticalPlotAxis() const;
	QwtPlot *plot() const;
	SymbolController *symbolCtrl() const;

	HorizHandlesArea *bottomHandlesArea() const;
	HorizHandlesArea *topHandlesArea() const;
	VertHandlesArea *rightHandlesArea() const;
	VertHandlesArea *leftHandlesArea() const;

	void addPlotAxisHandle(PlotAxisOffsetHandle *ax);
	void removePlotAxisHandle(PlotAxisOffsetHandle *ax);
public Q_SLOTS:
	void replot();

Q_SIGNALS:
	void canvasSizeChanged();
	// These are only emitted if you set setMouseTracking(true) (not done by default)
	void mouseButtonPress(const QMouseEvent *event);
	void mouseButtonRelease(const QMouseEvent *event);
	void mouseMove(const QMouseEvent *event);


private:

	QwtPlot *m_plot;
	QGridLayout *m_layout;
	QList<PlotChannel*> m_plotChannels;
	QList<QwtPlotScaleItem*> m_scaleItems;
	QList<PlotAxis*> m_horizontalPlotAxis;
	QList<PlotAxis*> m_verticalPlotAxis;
	QList<PlotAxisOffsetHandle*> m_horizontalPlotAxisHandles;
	QList<PlotAxisOffsetHandle*> m_verticalPlotAxisHandles;

	bool displayGraticule;
	Graticule *graticule;

	SymbolController *m_symbolCtrl;

	/* Adjacent areas */
	HorizHandlesArea *m_bottomHandlesArea;
	VertHandlesArea *m_rightHandlesArea;
	HorizHandlesArea *m_topHandlesArea;
	VertHandlesArea *m_leftHandlesArea;

	void setAxisScalesVisible(bool visible);
	void setupAxisScales();
	void setupOpenGLCanvas();
	void setupHandlesArea();
};


}

#endif // PLOT_H
