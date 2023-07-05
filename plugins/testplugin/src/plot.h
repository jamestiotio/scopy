#ifndef PLOT_H
#define PLOT_H
#include <QwtPlot>
#include "plotchannel.h"
#include <QWidget>
#include <graticule.h>
#include "plotaxis.h"
#include "scopy-testplugin_export.h"


namespace scopy {

class SCOPY_TESTPLUGIN_EXPORT Plot : public QwtPlot {
	Q_OBJECT
public:
	Plot(QWidget *parent = nullptr); // openGL canvas, add PlotAxis, add handleArea, add ChannelHandles (/w axis), add Zoomer, add cursorHandles
	~Plot();

	void addPlotChannel(PlotChannel *ch);
	void removePlotChannel(PlotChannel *ch);

	bool getDisplayGraticule() const;
	void setDisplayGraticule(bool newDisplayGraticule);
	bool eventFilter(QObject *object, QEvent *event) override;

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
};
}

#endif // PLOT_H
