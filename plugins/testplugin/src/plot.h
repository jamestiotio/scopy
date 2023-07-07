#ifndef PLOT_H
#define PLOT_H
#include <QwtPlot>
#include "plotchannel.h"
#include <QWidget>
#include <graticule.h>
#include "scopy-testplugin_export.h"


namespace scopy {
class PlotAxis;
class SCOPY_TESTPLUGIN_EXPORT Plot : public QwtPlot {
	Q_OBJECT
public:
	Plot(QWidget *parent = nullptr); // add handleArea, connect handle to SetValue() add ChannelHandles (/w axis), add Zoomer, add cursorHandles
	~Plot();

	void addPlotChannel(PlotChannel *ch);
	void removePlotChannel(PlotChannel *ch);

	void addPlotAxis(PlotAxis *ax);
	void removePlotAxis(PlotAxis *ax);

	bool getDisplayGraticule() const;
	void setDisplayGraticule(bool newDisplayGraticule);
	bool eventFilter(QObject *object, QEvent *event) override;

	const QList<PlotAxis *> &horizontalPlotAxis() const;

	const QList<PlotAxis *> &verticalPlotAxis() const;

Q_SIGNALS:
	void canvasSizeChanged();
	// These are only emitted if you set setMouseTracking(true) (not done by default)
	void mouseButtonPress(const QMouseEvent *event);
	void mouseButtonRelease(const QMouseEvent *event);
	void mouseMove(const QMouseEvent *event);


private:
	QList<PlotChannel*> m_plotChannels;
	QList<QwtPlotScaleItem*> m_scaleItems;
	QList<PlotAxis*> m_horizontalPlotAxis;
	QList<PlotAxis*> m_verticalPlotAxis;

	bool displayGraticule;
	Graticule *graticule;

	void setAxisScalesVisible(bool visible);
	void setupAxisScales();
	void setupOpenGLCanvas();
};
}

#endif // PLOT_H
