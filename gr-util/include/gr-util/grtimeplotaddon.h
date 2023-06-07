#ifndef GRTIMEPLOTADDON_H
#define GRTIMEPLOTADDON_H

#include "scopy-gr-util_export.h"
#include "tooladdon.h"
#include <gui/oscilloscope_plot.hpp>

#include <QGridLayout>

namespace scopy::grutil {
class SCOPY_GR_UTIL_EXPORT GRTimePlotAddon : public QObject, public ToolAddon {
	Q_OBJECT
public:
	GRTimePlotAddon(QString name, QObject *parent = nullptr);
	~GRTimePlotAddon();

	QString getName() override;
	QWidget *getWidget() override;
	CapturePlot *plot();

public Q_SLOTS:
	void enable() override;
	void disable() override;
	void onStart() override;
	void onStop() override;
	void onAdd() override;
	void onRemove() override;
	void onChannelAdded(ToolAddon* t) override;
	void onChannelRemoved(ToolAddon*) override;


private:
	QString name;
	QWidget *widget;
	CapturePlot *m_plot;
};
}

#endif // GRTIMEPLOTADDON_H
