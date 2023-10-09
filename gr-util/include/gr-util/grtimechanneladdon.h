#ifndef GRTIMECHANNELADDON_H
#define GRTIMECHANNELADDON_H

#include "measurementcontroller.h"
#include "tooladdon.h"
#include "grsignalpath.h"
#include "grtimeplotaddon.h"
#include "griiofloatchannelsrc.h"

#include <QLabel>
#include "scopy-gr-util_export.h"
#include <gui/plotaxis.h>
#include <gui/plotaxishandle.h>
#include <gui/plotchannel.h>
#include "grscaleoffsetproc.h"
#include <gui/spinbox_a.hpp>

#include <gui/widgets/menucombo.h>
#include "timechanneladdon.h"
#include "time_yautoscale.h"
#include "time_ycontrol.h"

namespace scopy {
class MenuOnOffSwitch;

namespace grutil {
class GRDeviceAddon;
class SCOPY_GR_UTIL_EXPORT GRTimeChannelAddon : public TimeChannelAddon, public GRTopAddon {
	Q_OBJECT
public:

	typedef enum {
		YMODE_COUNT,
		YMODE_FS,
		YMODE_SCALE
	} YMode;
	GRTimeChannelAddon(QString ch, GRDeviceAddon* dev, GRTimePlotAddon* plotAddon, QPen pen, QObject *parent = nullptr);
	~GRTimeChannelAddon();

	void setDevice(GRDeviceAddon *d);
	GRDeviceAddon* getDevice();

	GRSignalPath *signalPath() const;	
	GRIIOFloatChannelSrc *grch() const;
	bool sampleRateAvailable() override;
	double sampleRate() override;
	MeasureManagerInterface* getMeasureManager() override;

public Q_SLOTS:
	void enable() override;
	void disable() override;
	void onStart() override;
	void onStop() override;
	void onInit() override;
	void onDeinit() override;
	void preFlowBuild() override;

	void onNewData(const float* xData, const float* yData, int size);

	void toggleAutoScale();
	void setYMode(YMode mode);
	void setSingleYMode(bool) override;

private:
	GRDeviceAddon* m_dev;
	GRScaleOffsetProc* m_scOff;
	GRSignalPath *m_signalPath;
	GRIIOFloatChannelSrc *m_grch;
	TimeMeasureManager *m_measureMgr;
	TimeYControl *m_yCtrl;
	TimeYAutoscale *m_autoscale;
	MenuCombo *m_ymodeCb;
	MenuOnOffSwitch *m_autoscaleBtn;

	bool m_scaleAvailable;
	bool m_sampleRateAvailable;
	bool m_autoscaleEnabled;
	bool m_running;

	QString m_unit;
	QWidget *createMenu(QWidget *parent = nullptr);
	QWidget *createYAxisMenu(QWidget *parent);
};
}
}
#endif // GRTIMECHANNELADDON_H
