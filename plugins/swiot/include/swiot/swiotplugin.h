#ifndef SWIOTPLUGIN_H
#define SWIOTPLUGIN_H

#define SCOPY_PLUGIN_NAME SWIOTPlugin
#define SCOPY_PLUGIN_PRIO 1

#include <pluginbase/plugin.h>
#include "pluginbase/pluginbase.h"
#include "scopyswiot_export.h"
#include <pluginbase/contextprovider.h>

namespace Ui {
class SWIOTInfoPage;
class SWIOTConfigTool;
class SWIOTRuntime;
}

namespace adiscope {
class SCOPYSWIOT_EXPORT SWIOTPlugin : public QObject, public PluginBase
{
	Q_OBJECT
	SCOPY_PLUGIN
	bool load(QString uri) override;
	void unload() override;
	bool compatible(QString uri) override;

public Q_SLOTS:
	bool connectDev() override;
	bool disconnectDev() override;

private:
	Ui::SWIOTInfoPage *infoui;
	Ui::SWIOTConfigTool *configui;
	Ui::SWIOTRuntime *rungui;
	QWidget *config ;
	QWidget *runtime;

};
}

#endif // SWIOTPLUGIN_H
