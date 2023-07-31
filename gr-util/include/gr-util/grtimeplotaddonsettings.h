#ifndef GRTIMEPLOTADDONSETTINGS_H
#define GRTIMEPLOTADDONSETTINGS_H

#include "tooladdon.h"
#include "scopy-gr-util_export.h"
#include <QLabel>
#include "grtimeplotaddon.h"
#include <gui/spinbox_a.hpp>

namespace scopy::grutil {
class SCOPY_GR_UTIL_EXPORT GRTimePlotAddonSettings : public QObject, public ToolAddon {
	Q_OBJECT
public:
	typedef enum {
		XMODE_SAMPLES,
		XMODE_TIME,
		XMODE_OVERRIDE
	} XMode;
	GRTimePlotAddonSettings(GRTimePlotAddon *p, QObject *parent = nullptr);
	~GRTimePlotAddonSettings();

	QString getName() override;
	QWidget* getWidget() override;

public Q_SLOTS:
	void enable() override {}
	void disable() override {}
	void onStart() override {}
	void onStop() override {}
	void onAdd() override {}
	void onRemove() override {}
	void onChannelAdded(ToolAddon*) override {}
	void onChannelRemoved(ToolAddon*) override {}

private:
	QWidget* createMenu(QWidget* parent = nullptr);
	QWidget* createXAxisMenu(QWidget* parent = nullptr);

private:
	GRTimePlotAddon* p;
	QString name;
	QWidget *widget;
	QPen m_pen;

	ScaleSpinButton *m_bufferSizeSpin;
	ScaleSpinButton *m_plotSizeSpin;

	PositionSpinButton *m_xmin;
	PositionSpinButton *m_xmax;
	PositionSpinButton *m_sampleRateSpin;
	bool m_sampleRateAvailable;
};
}

#endif // GRTIMEPLOTADDONSETTINGS_H
