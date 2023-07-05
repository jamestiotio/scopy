#include "plot.h"
#include <gui/BasicPlot.h>

#include <gui/DisplayPlot.h>

#include <QwtPlot>
#include <QwtPlotCurve>
#include <QwtPlotLayout>
#include <QwtPlotSeriesItem>
#include <gui/customqwtscaledraw.hpp>
#include <QMouseEvent>
#include <osc_scale_engine.h>
#include <QObject>

using namespace scopy;


Plot::Plot(QWidget *parent) : QwtPlot(parent) {

	setAxisScale(QwtAxisId(QwtAxis::YLeft,0),-5,5,(5- (-5))/10.0); // set Divs, limits
	OscScaleDraw *xScaleDraw = new OscScaleDraw(new TimePrefixFormatter(),"");    // set Formatter (?)
	setAxisScaleDraw(QwtAxisId(QwtAxis::XBottom,0),xScaleDraw);
	OscScaleEngine *xScaleEngine = new OscScaleEngine();
	this->setAxisScaleEngine(QwtAxisId(QwtAxis::XBottom,0), (QwtScaleEngine *)xScaleEngine);
	setAxisVisible(QwtAxisId(QwtAxis::XBottom,0), true);


	setAxisScale(QwtAxisId(QwtAxis::XBottom,0),0,0.1,((0.1-0)/16.0)); // set divs , limits	
	OscScaleDraw *yScaleDraw = new OscScaleDraw(new MetricPrefixFormatter(),"V"); //
	setAxisScaleDraw(QwtAxisId(QwtAxis::YLeft,0), yScaleDraw);
	OscScaleEngine *yScaleEngine = new OscScaleEngine();
	this->setAxisScaleEngine(QwtAxisId(QwtAxis::YLeft,0), (QwtScaleEngine *)yScaleEngine);
	setAxisVisible(QwtAxisId(QwtAxis::YLeft,0), true);

	plotLayout()->setAlignCanvasToScales( true );
	axisWidget(QwtAxisId(QwtAxis::YLeft,0))->setMargin(0);

	setupAxisScales();
	setAxisScalesVisible(true);
	// Plot needs a grid
	EdgelessPlotGrid *d_grid = new EdgelessPlotGrid();

	QColor majorPenColor("#353537");
	d_grid->setMajorPen(majorPenColor, 1.0, Qt::DashLine);
	d_grid->attach(this);

//	QwtPlotMarker *d_origin = new QwtPlotMarker();
//	d_origin->setLineStyle( QwtPlotMarker::Cross );
//	d_origin->setValue( 0, 0.0 );
//	d_origin->setLinePen( Qt::gray, 0.0, Qt::DashLine );
//	d_origin->attach( this );

	graticule = new Graticule(this);
	connect(this, SIGNAL(canvasSizeChanged()),graticule,SLOT(onCanvasSizeChanged()));
	setDisplayGraticule(false);

}

Plot::~Plot() {

}

void Plot::setupAxisScales() {
	for (unsigned int i = 0; i < 4; i++) {
		QwtScaleDraw::Alignment scale =
		    static_cast<QwtScaleDraw::Alignment>(i);
		auto scaleItem = new EdgelessPlotScaleItem(scale);

		scaleItem->scaleDraw()->setAlignment(scale);
		scaleItem->scaleDraw()->enableComponent(QwtAbstractScaleDraw::Backbone, false);
		scaleItem->scaleDraw()->enableComponent(QwtAbstractScaleDraw::Labels, false);
		scaleItem->setFont(this->axisWidget(0)->font());

		QPalette palette = scaleItem->palette();
		palette.setBrush(QPalette::WindowText, QColor(0x6E6E6F));
		palette.setBrush(QPalette::Text, QColor(0x6E6E6F));
		scaleItem->setPalette(palette);
		scaleItem->setBorderDistance(0);
		scaleItem->attach(this);
		m_scaleItems.push_back(scaleItem);
		scaleItem->setZ(200);
	}
}

void Plot::setAxisScalesVisible(bool visible) {
	for(QwtPlotScaleItem* scale : qAsConst(m_scaleItems)){
		if(visible){
			scale->attach(this);
		} else {
			scale->detach();
		}
	}
}

void Plot::addPlotChannel(PlotChannel *ch)
{
	m_plotChannels.append(ch);
}

void Plot::removePlotChannel(PlotChannel *ch)
{
	m_plotChannels.removeAll(ch);
}

bool Plot::getDisplayGraticule() const
{
	return displayGraticule;
}

void Plot::setDisplayGraticule(bool newDisplayGraticule)
{
	displayGraticule = newDisplayGraticule;
	setAxisScalesVisible(!displayGraticule);
	graticule->enableGraticule(displayGraticule);
	replot();
}

bool Plot::eventFilter(QObject *object, QEvent *event)
{
	if (object == canvas()) {
		switch (event->type()) {
		case QEvent::MouseMove: {
			Q_EMIT mouseMove(static_cast< QMouseEvent* >( event ));
			break;
		}
		case QEvent::MouseButtonPress: {
			Q_EMIT mouseButtonPress(static_cast< QMouseEvent* >( event ));
			break;
		}
		case QEvent::MouseButtonRelease: {
			Q_EMIT mouseButtonRelease(static_cast< QMouseEvent* >( event ));
			break;
		}
		case QEvent::Resize: {
//			updateHandleAreaPadding(d_labelsEnabled);

			//force cursor handles to emit position changed
			//when the plot canvas is being resized
//			d_hCursorHandle1->triggerMove();
//			d_hCursorHandle2->triggerMove();
//			d_vCursorHandle1->triggerMove();
//			d_vCursorHandle2->triggerMove();

			/* update the size of the gates when the plot canvas is resized */
//			updateGateMargins();

			Q_EMIT canvasSizeChanged();
			break;
		}
		default:
			break;
		}
	}
	return QObject::eventFilter(object, event);
}

