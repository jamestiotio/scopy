#ifndef GRIIODEVICESOURCE_H
#define GRIIODEVICESOURCE_H

#include "grproxyblock.h"
#include "scopy-gr-util_export.h"
#include <gnuradio/iio/device_source.h>
#include <iio.h>

namespace scopy::grutil {

typedef GRProxyBlock GRIIOChannel;

class SCOPY_GR_UTIL_EXPORT GRIIODeviceSource : public GRProxyBlock { // is this a proxy block
public:
	GRIIODeviceSource(iio_context *ctx, QString deviceName, QString phyDeviceName, QObject *parent = nullptr);
	void build_blks(GRTopBlock *top) override;
	void destroy_blks(GRTopBlock *top) override;

	void connect_blk(GRTopBlock *top, GRProxyBlock*) override;
	void disconnect_blk(GRTopBlock *top);

	void addChannel(GRIIOChannel*);
	void removeChannel(GRIIOChannel*);

protected:
	QList<GRIIOChannel*> list;
	std::vector<std::string> channelNames;
	QString deviceName;
	QString phyDeviceName;
	QString phy;
	iio_context *ctx;
	int buffersize;

	gr::iio::device_source::sptr src;

private:
	void computeChannelNames();
	void addChannelAtIndex(iio_device *dev, QString channelName);
	void matchChannelToBlockOutputs(GRTopBlock* top);
	int getOutputIndex(QString ch);
};
}
#endif // GRIIODEVICESOURCE_H
