#include "plotwidget.h"
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
#include "plotaxis.h"
#include <pluginbase/preferences.h>
#include <QwtPlotOpenGLCanvas>

using namespace scopy;


PlotWidget::PlotWidget(QWidget *parent) : QWidget(parent) {

	m_plot = new QwtPlot(this);
	setupOpenGLCanvas();

	addPlotAxis(new PlotAxis(QwtAxis::XBottom,this,this));
	addPlotAxis(new PlotAxis(QwtAxis::YLeft,this,this));

	m_plot->plotLayout()->setAlignCanvasToScales( false );
	m_plot->axisWidget(QwtAxisId(QwtAxis::YLeft,0))->setMargin(0);

	setupAxisScales();
	setAxisScalesVisible(true);

	// Plot needs a grid
	EdgelessPlotGrid *d_grid = new EdgelessPlotGrid();
	QColor majorPenColor("#353537");
	d_grid->setMajorPen(majorPenColor, 1.0, Qt::DashLine);
	d_grid->attach(m_plot);

	QwtPlotMarker *d_origin = new QwtPlotMarker();
	d_origin->setLineStyle( QwtPlotMarker::Cross );
	d_origin->setValue( 0, 0.0 );
	d_origin->setLinePen( Qt::gray, 0.0, Qt::DashLine );
	d_origin->attach( m_plot );

	graticule = new Graticule(m_plot);
	connect(this, SIGNAL(canvasSizeChanged()),graticule,SLOT(onCanvasSizeChanged()));
	setDisplayGraticule(false);


	///////////////////
	d_symbolCtrl = new SymbolController(m_plot);

	/* Adjacent areas */
	d_bottomHandlesArea = new HorizHandlesArea(m_plot->canvas());
	d_rightHandlesArea = new VertHandlesArea(m_plot->canvas());
	d_topHandlesArea = new HorizHandlesArea(m_plot->canvas());
	d_leftHandlesArea = new VertHandlesArea(m_plot->canvas());

	d_bottomHandlesArea->setMinimumHeight(50);
	d_rightHandlesArea->setMinimumWidth(50);
	//	d_bottomHandlesArea->setLargestChildWidth(60);
	//	d_rightHandlesArea->setLargestChildHeight(60);
	d_rightHandlesArea->setMinimumHeight(m_plot->minimumHeight());
	d_rightHandlesArea->setBottomPadding(50);
}

PlotWidget::~PlotWidget() {

}

void PlotWidget::setupAxisScales() {
	for (unsigned int i = 0; i < 4; i++) {
		QwtScaleDraw::Alignment scale =
		    static_cast<QwtScaleDraw::Alignment>(i);
		auto scaleItem = new EdgelessPlotScaleItem(scale);

		scaleItem->scaleDraw()->setAlignment(scale);
		scaleItem->scaleDraw()->enableComponent(QwtAbstractScaleDraw::Backbone, false);
		scaleItem->scaleDraw()->enableComponent(QwtAbstractScaleDraw::Labels, false);
		scaleItem->setFont(m_plot->axisWidget(0)->font());

		QPalette palette = scaleItem->palette();
		palette.setBrush(QPalette::WindowText, QColor(0x6E6E6F));
		palette.setBrush(QPalette::Text, QColor(0x6E6E6F));
		scaleItem->setPalette(palette);
		scaleItem->setBorderDistance(0);
		scaleItem->attach(m_plot);
		m_scaleItems.push_back(scaleItem);
		scaleItem->setZ(200);
	}
}

void PlotWidget::setupOpenGLCanvas()
{
	bool useOpenGLCanvas = Preferences::GetInstance()->get("general_use_opengl").toBool();
	if(useOpenGLCanvas) {
		QwtPlotOpenGLCanvas* plotCanvas = qobject_cast< QwtPlotOpenGLCanvas* >( m_plot->canvas() );
		if ( plotCanvas == NULL )
		{
			plotCanvas = new QwtPlotOpenGLCanvas(m_plot);
			plotCanvas->setPaintAttribute(QwtPlotAbstractGLCanvas::BackingStore );
			m_plot->setCanvas( plotCanvas );
		} else {
			;
		}
	} else {
		QwtPlotCanvas *plotCanvas = qobject_cast<QwtPlotCanvas *>( m_plot->canvas() );
		plotCanvas->setPaintAttribute(QwtPlotCanvas::BackingStore, true);
	}
}

void PlotWidget::setAxisScalesVisible(bool visible) {
	for(QwtPlotScaleItem* scale : qAsConst(m_scaleItems)){
		if(visible){
			scale->attach(m_plot);
		} else {
			scale->detach();
		}
	}
}

void PlotWidget::addPlotChannel(PlotChannel *ch)
{
	m_plotChannels.append(ch);
}

void PlotWidget::removePlotChannel(PlotChannel *ch)
{
	m_plotChannels.removeAll(ch);
}

void PlotWidget::addPlotAxis(PlotAxis *ax)
{
	if(ax->isHorizontal())
		m_horizontalPlotAxis.append(ax);
	else
		m_verticalPlotAxis.append(ax);
}

bool PlotWidget::getDisplayGraticule() const
{
	return displayGraticule;
}

void PlotWidget::setDisplayGraticule(bool newDisplayGraticule)
{
	displayGraticule = newDisplayGraticule;
	setAxisScalesVisible(!displayGraticule);
	graticule->enableGraticule(displayGraticule);
	m_plot->replot();
}

bool PlotWidget::eventFilter(QObject *object, QEvent *event)
{
	if (object == m_plot->canvas()) {
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

const QList<PlotAxis *> &PlotWidget::horizontalPlotAxis() const
{
	return m_horizontalPlotAxis;
}

const QList<PlotAxis *> &PlotWidget::verticalPlotAxis() const
{
	return m_verticalPlotAxis;
}

QwtPlot *PlotWidget::plot() const
{
	return m_plot;
}

void PlotWidget::replot()
{
	m_plot->replot();
}

