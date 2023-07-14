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

	m_selectedChannel = nullptr;
	m_plot = new QwtPlot(this);
	m_layout = new QGridLayout(this);
	m_layout->setSpacing(0);
	m_layout->setMargin(0);
	setLayout(m_layout);

	setupOpenGLCanvas();
	setupHandlesArea();
	//	m_zoomer = new OscPlotZoomer(m_plot->canvas(),false);
	//	m_plot->setMouseTracking(true);

	auto xAxis = new PlotAxis(QwtAxis::XBottom,this,this);
	auto yAxis = new PlotAxis(QwtAxis::YLeft,this,this);
	yAxis->setVisible(false);

	setupAxisScales();
	setAxisScalesVisible(true);

	// Plot needs a grid
	EdgelessPlotGrid *d_grid = new EdgelessPlotGrid();
	QColor majorPenColor("#353537");
	d_grid->setMajorPen(majorPenColor, 1.0, Qt::DashLine);
	d_grid->attach(m_plot);

	//	QwtPlotMarker *d_origin = new QwtPlotMarker();
	//	d_origin->setLineStyle( QwtPlotMarker::Cross );
	//	d_origin->setValue( 0, 0.0 );
	//	d_origin->setLinePen( Qt::gray, 0.0, Qt::DashLine );
	//	d_origin->attach( m_plot );

	graticule = new Graticule(m_plot);
	connect(this, SIGNAL(canvasSizeChanged()),graticule,SLOT(onCanvasSizeChanged()));
	setDisplayGraticule(false);

	m_plot->plotLayout()->setAlignCanvasToScales(true);
	m_plot->plotLayout()->setCanvasMargin(0);
	m_plot->plotLayout()->setSpacing(0);

}

void PlotWidget::setupHandlesArea() {
	m_symbolCtrl = new SymbolController(m_plot);

	/* Adjacent areas */
	m_bottomHandlesArea = new HorizHandlesArea(m_plot->canvas());
	m_rightHandlesArea = new VertHandlesArea(m_plot->canvas());
	m_topHandlesArea = new HorizHandlesArea(m_plot->canvas());
	m_leftHandlesArea = new VertHandlesArea(m_plot->canvas());

	//	m_bottomHandlesArea->setMinimumHeight(50);
	//	m_rightHandlesArea->setMinimumWidth(50);
	//	m_topHandlesArea->setMinimumHeight(50);
	m_leftHandlesArea->setMinimumWidth(50);
	m_leftHandlesArea->setBottomPadding(0);
	m_leftHandlesArea->setTopPadding(0); /// Why ?

	m_layout->addWidget(m_bottomHandlesArea,2,1);
	m_layout->addWidget(m_rightHandlesArea,1,2);
	m_layout->addWidget(m_leftHandlesArea,1,0);
	m_layout->addWidget(m_topHandlesArea,0,1);
	m_layout->addWidget(m_plot,1,1);
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

void PlotWidget::addPlotAxisHandle(PlotAxisHandle *ax) {

	m_plotAxisHandles[ax->axis()->position()].append(ax);
}

void PlotWidget::removePlotAxisHandle(PlotAxisHandle *ax) {
	m_plotAxisHandles[ax->axis()->position()].removeAll(ax);
}


void PlotWidget::addPlotAxis(PlotAxis *ax)
{
	m_plotAxis[ax->position()].append(ax);
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

QList<PlotAxis *> &PlotWidget::plotAxis(int position) {
	return m_plotAxis[position];
}

PlotAxis *PlotWidget::xAxis() {
	return m_plotAxis[QwtAxis::XBottom][0];
}

QwtPlot *PlotWidget::plot() const
{
	return m_plot;
}

void PlotWidget::replot()
{
	m_plot->replot();
}

void PlotWidget::selectChannel(PlotChannel *ch)
{
	if(m_selectedChannel != nullptr) {
		m_selectedChannel->yAxis()->setVisible(false);
	}
	m_selectedChannel = ch;

//	m_zoomer->setAxes(m_selectedChannel->xAxis()->axisId(), m_selectedChannel->yAxis()->axisId());
	m_selectedChannel->xAxis()->setVisible(false);
	m_selectedChannel->yAxis()->setVisible(false);

	if(m_selectedChannel->curve()) {
		m_selectedChannel->curve()->detach();
		m_selectedChannel->curve()->attach(m_plot);
	}

	if(m_selectedChannel->handle()) {
		m_selectedChannel->handle()->offsetHdl()->raise();
	}

	// bring handle to front - tied to
}

PlotChannel *PlotWidget::selectedChannel() const
{
	return m_selectedChannel;
}

VertHandlesArea *PlotWidget::leftHandlesArea() const
{
	return m_leftHandlesArea;
}

VertHandlesArea *PlotWidget::rightHandlesArea() const
{
	return m_rightHandlesArea;
}

HorizHandlesArea *PlotWidget::topHandlesArea() const
{
	return m_topHandlesArea;
}

HorizHandlesArea *PlotWidget::bottomHandlesArea() const
{
	return m_bottomHandlesArea;
}

SymbolController *PlotWidget::symbolCtrl() const
{
	return m_symbolCtrl;
}

