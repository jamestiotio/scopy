#ifndef TIMECHANNELADDON_H
#define TIMECHANNELADDON_H

#include <QObject>
#include "scopy-gr-util_export.h"
#include "tooladdon.h"
#include <plotchannel.h>
#include "grtimeplotaddon.h"
#include "measurementcontroller.h"

namespace scopy {
namespace grutil {
class SCOPY_GR_UTIL_EXPORT TimeChannelAddon : public QObject, public ToolAddon {
	Q_OBJECT
public:

	TimeChannelAddon(QString ch, PlotAddon* plotAddon, QPen pen, QObject *parent = nullptr);
	virtual ~TimeChannelAddon();

	QString getName() override;
	QWidget* getWidget() override;

	QPen pen() const;
	bool enabled() const;
	PlotChannel *plotCh() const;

	virtual MeasureManagerInterface* getMeasureManager();
	virtual QWidget *createCurveMenu(QWidget *parent);

protected:
	QString m_channelName;
	PlotChannel *m_plotCh;
	PlotAxis *m_plotAxis;
	PlotAxisHandle *m_plotAxisHandle;
	PlotAddon* m_plotAddon;
	QPen m_pen;
	QWidget *widget;

	bool m_enabled;


public Q_SLOTS:
	void enable() override;
	void disable() override;
	void onStart() override;
	void onStop() override;
	void onInit() override;
	void onDeinit() override;

	void onNewData(const float* xData, const float* yData, int size);

	void onChannelAdded(ToolAddon*) override;
	void onChannelRemoved(ToolAddon*) override;
};
}
}
#endif // TIMECHANNELADDON_H
