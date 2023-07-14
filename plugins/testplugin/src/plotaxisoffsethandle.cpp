#include "plotaxisoffsethandle.h"
#include "plotaxis.h"
#include "gui/symbol.h"
#include "gui/symbol_controller.h"

using namespace scopy;
PlotAxisOffsetHandle::PlotAxisOffsetHandle(QColor color, PlotAxis* ax, PlotWidget *p, QObject *parent) : QObject(parent)
{
	QColor chnColor = color;
	m_symbolCtrl = p->symbolCtrl();
	/* Channel offset widget */
	m_chOffsetBar = new HorizBar(p->plot());
	m_symbolCtrl->attachSymbol(m_chOffsetBar);
	m_chOffsetBar->setCanLeavePlot(true);
	m_chOffsetBar->setVisible(true);
	m_chOffsetBar->setMobileAxis(ax->axisId());

	m_chOffsetHdl = new RoundedHandleV(
	    QPixmap(":/gui/icons/handle_right_arrow.svg"),
	    QPixmap(":/gui/icons/handle_up_arrow.svg"),
	    QPixmap(":/gui/icons/handle_down_arrow.svg"),
	    p->leftHandlesArea(), true);
	m_chOffsetHdl->setRoundRectColor(chnColor);
	m_chOffsetHdl->setPen(QPen(chnColor, 2, Qt::SolidLine));
	m_chOffsetHdl->setVisible(true);

	/* When bar position changes due to plot resizes update the handle */
	connect(p->leftHandlesArea(), &HandlesArea::sizeChanged, m_chOffsetHdl, [=](){
		m_chOffsetHdl->updatePosition();
	});

	connect(m_chOffsetBar, &HorizBar::pixelPositionChanged,
		[=](int pos) {
			m_chOffsetHdl->setPositionSilenty(pos);
		});

	connect(m_chOffsetHdl, &RoundedHandleV::positionChanged,
		[=](int pos) {
			QwtScaleMap yMap = p->plot()->canvasMap(ax->axisId());
			double offset = yMap.invTransform(pos);
			double min = ax->min() + offset;
			double max = ax->max() + offset;
			ax->setInterval(min, max);
			p->replot();
		}
		);


}

PlotAxisOffsetHandle::~PlotAxisOffsetHandle()
{
	m_symbolCtrl->detachSymbol(m_chOffsetBar);
	delete m_chOffsetHdl;
}

RoundedHandleV *PlotAxisOffsetHandle::offsetHdl() const
{
	return m_chOffsetHdl;
}
