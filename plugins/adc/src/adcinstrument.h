#ifndef ADCINSTRUMENT_H
#define ADCINSTRUMENT_H

#include <QWidget>
#include <adcplugin.h>
#include "gui/tooltemplate.h"
#include "measurementsettings.h"
#include "verticalchannelmanager.h"
#include <gui/widgets/toolbuttons.h>
#include <QPushButton>

namespace scopy {
class MenuControlButton;
class CollapsableMenuControlButton;

class AdcInstrument : public QWidget
{
	Q_OBJECT
public:
	AdcInstrument(PlotProxy *proxy, QWidget *parent = nullptr);
	~AdcInstrument();
	void init();
	void deinit();
	void startAddons();
	void stopAddons();

	bool running() const;
	void setRunning(bool newRunning);

public Q_SLOTS:
	void run(bool);
	void stop();
	void start();
	void restart();
	void showMeasurements(bool b);
	MenuControlButton* addChannel(ChannelAddon *ch, QWidget *parent);
	CollapsableMenuControlButton* addDevice(GRDeviceAddon *dev, QWidget *parent);
Q_SIGNALS:
	void runningChanged(bool);

private:
	bool m_running;
	RunBtn *runBtn;
	SingleShotBtn *singleBtn;
	ToolTemplate *tool;
	PlotProxy* proxy;
	QPushButton *openLastMenuBtn;

	MenuControlButton *channelsBtn;

	MeasurementsPanel* measure_panel;
	MeasurementSettings* measureSettings;
	StatsPanel* stats_panel;

	GRTimePlotAddon* plotAddon;
	GRTimePlotAddonSettings* plotAddonSettings;
	VerticalChannelManager *vcm;

	MapStackedWidget *channelStack;
	QButtonGroup* rightMenuBtnGrp;
	QButtonGroup *channelGroup;

	void setupCursorButtonHelper(MenuControlButton *cursor);
	void setupMeasureButtonHelper(MenuControlButton *measure);
	void setupChannelsButtonHelper(MenuControlButton *channelsBtn);
	void setupDeviceMenuControlButtonHelper(MenuControlButton *devBtn, GRDeviceAddon *dev);
	void setupChannelMenuControlButtonHelper(MenuControlButton *btn, ChannelAddon *ch);

	Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)

	int uuid = 0;
	const QString channelsMenuId = "channels";
	const QString measureMenuId = "measure";
	const QString statsMenuId = "stats";
	const QString verticalChannelManagerId = "vcm";
	void setupChannelMeasurement(ChannelAddon *ch);
};
}
#endif // ADCINSTRUMENT_H
