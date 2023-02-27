#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "pluginbase/toolmenuentry.h"
#include "scopycore_export.h"

namespace adiscope {
class SCOPYCORE_EXPORT Device {
public:
	virtual ~Device(){};
	virtual QString name() = 0;
	virtual QString uri() = 0;;
	virtual QWidget *icon() = 0;;
	virtual QWidget *page() = 0;;
	virtual QList<ToolMenuEntry*> toolList() = 0;
	virtual void loadCompatiblePlugins() = 0;
	virtual void loadPlugins() = 0;
	virtual void unloadPlugins() = 0;

public Q_SLOTS:
	virtual void connectDev() = 0;
	virtual void disconnectDev() = 0;
	virtual void showPage() = 0;
	virtual void hidePage() = 0;

Q_SIGNALS:
	virtual void toolListChanged() = 0;
	virtual void connected() = 0;
	virtual void disconnected()  = 0;
	virtual void requestedRestart() = 0;
	virtual void requestTool(QString) = 0;

};
}

#endif // DEVICE_H