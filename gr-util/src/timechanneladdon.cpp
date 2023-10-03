#include "timechanneladdon.h"

#include <menusectionwidget.h>
#include <menucollapsesection.h>
#include <menucombo.h>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(CAT_TIME_CHANNEL, "TimeChannelAddon");

using namespace scopy;
using namespace scopy::grutil;

TimeChannelAddon::TimeChannelAddon(QString ch, PlotAddon *plotAddon, QPen pen, QObject *parent) :
	QObject(parent), m_channelName(ch), m_plotAddon(plotAddon), m_pen(pen)
{

}

TimeChannelAddon::~TimeChannelAddon()
{

}

QString TimeChannelAddon::getName() {return m_channelName;}

QWidget *TimeChannelAddon::getWidget() {return widget;}

void TimeChannelAddon::onStart()
{

}

void TimeChannelAddon::onStop()
{

}

void TimeChannelAddon::onInit()
{

}

void TimeChannelAddon::onDeinit()
{

}

void TimeChannelAddon::onNewData(const float *xData, const float *yData, int size)
{

}

void TimeChannelAddon::onChannelAdded(ToolAddon *)
{

}

void TimeChannelAddon::onChannelRemoved(ToolAddon *)
{

}

PlotChannel *TimeChannelAddon::plotCh() const
{
	return m_plotCh;
}

MeasureManagerInterface *TimeChannelAddon::getMeasureManager()
{
	return nullptr;
}

QPen TimeChannelAddon::pen() const {
	return m_pen;
}

bool TimeChannelAddon::enabled() const
{
	return m_enabled;
}

void TimeChannelAddon::enable() {
	qInfo(CAT_TIME_CHANNEL)<<m_channelName<<" enabled";
	m_enabled = true;
	m_plotCh->enable();
	m_plotAxisHandle->handle()->setVisible(true);
	m_plotAxisHandle->handle()->raise();
	//	m_grch->setEnabled(true);
	m_plotAddon->replot();
	//	m_plotAddon->plot()->replot();
}

void TimeChannelAddon::disable() {
	qInfo(CAT_TIME_CHANNEL)<<m_channelName<<" disabled";
	m_enabled = false;
	m_plotCh->disable();
	m_plotAxisHandle->handle()->setVisible(false);

	//	m_grch->setEnabled(false);
	m_plotAddon->replot();
	//	m_plotAddon->plot()->replot();
}

QWidget* TimeChannelAddon::createCurveMenu(QWidget* parent) {

	MenuSectionWidget *curvecontainer = new MenuSectionWidget(parent);
	MenuCollapseSection *curve = new MenuCollapseSection("CURVE",MenuCollapseSection::MHCW_NONE, curvecontainer);

	QWidget *curveSettings = new QWidget(curve);
	QHBoxLayout *curveSettingsLay = new QHBoxLayout(curveSettings);
	curveSettingsLay->setMargin(0);
	curveSettingsLay->setSpacing(10);
	curveSettings->setLayout(curveSettingsLay);

	MenuCombo *cbThicknessW = new MenuCombo("Thickness", curve);
	auto cbThickness = cbThicknessW->combo();
	cbThickness->addItem("1");
	cbThickness->addItem("2");
	cbThickness->addItem("3");
	cbThickness->addItem("4");
	cbThickness->addItem("5");

	connect(cbThickness, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int idx) {
		m_plotCh->setThickness(cbThickness->itemText(idx).toFloat());
	});
	MenuCombo *cbStyleW = new MenuCombo("Style", curve);
	auto cbStyle = cbStyleW->combo();
	cbStyle->addItem("Lines", PlotChannel::PCS_LINES);
	cbStyle->addItem("Dots", PlotChannel::PCS_DOTS);
	cbStyle->addItem("Steps", PlotChannel::PCS_STEPS);
	cbStyle->addItem("Sticks", PlotChannel::PCS_STICKS);
	cbStyle->addItem("Smooth", PlotChannel::PCS_SMOOTH);
	StyleHelper::MenuComboBox(cbStyle,"cbStyle");

	connect(cbStyle, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int idx) {
		m_plotCh->setStyle(cbStyle->itemData(idx).toInt());
	});

	curveSettingsLay->addWidget(cbThicknessW);
	curveSettingsLay->addWidget(cbStyleW);
	curve->contentLayout()->addWidget(curveSettings);
	curvecontainer->contentLayout()->addWidget(curve);

	return curvecontainer;
}
