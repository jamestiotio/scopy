#include "griiodevicesource.h"
#include "griiofloatchannelsrc.h"
#include "griiocomplexchannelsrc.h"
#include "grtopblock.h"

using namespace scopy::grutil;
GRIIODeviceSource::GRIIODeviceSource(iio_context *ctx, QString deviceName, QString phyDeviceName, QObject *parent) :
      GRProxyBlock(parent), ctx(ctx), deviceName(deviceName), phyDeviceName(phyDeviceName)
{

}

void GRIIODeviceSource::addChannelAtIndex(iio_device* iio_dev, QString channelName) {
	std::string channel_name = channelName.toStdString();
	iio_channel* iio_ch = iio_device_find_channel(iio_dev, channel_name.c_str(), false);
	int idx = iio_channel_get_index(iio_ch);
	channelNames[idx] = channel_name;
}

void GRIIODeviceSource::computeChannelNames() {

	iio_device* iio_dev = iio_context_find_device(ctx,deviceName.toStdString().c_str());
	channelNames.reserve(iio_device_get_channels_count(iio_dev));

	for(GRIIOChannel* ch : qAsConst(list)) {
		GRIIOFloatChannelSrc* floatCh = dynamic_cast<GRIIOFloatChannelSrc*>(ch);
		if(floatCh) {
			addChannelAtIndex(iio_dev, floatCh->getChannelName());
		}

		GRIIOComplexChannelSrc* complexCh = dynamic_cast<GRIIOComplexChannelSrc*>(ch);
		if(complexCh) {
			addChannelAtIndex(iio_dev, complexCh->getChannelNameI());
			addChannelAtIndex(iio_dev, complexCh->getChannelNameQ());
		}
	}



	channelNames.erase(std::remove_if(
			       channelNames.begin(),
			       channelNames.end(),
			       [=](std::string x){return x.empty();}),
			   channelNames.end()); // clear empty channels

}

int GRIIODeviceSource::getOutputIndex(QString ch) {
	for(int i = 0;i < channelNames.size();i++) {
		if(ch.toStdString() == channelNames[i])
			return i;
	}
	return -1;
}

void GRIIODeviceSource::matchChannelToBlockOutputs(GRTopBlock *top) {
	for(GRIIOChannel* ch : qAsConst(list)) {
		GRIIOFloatChannelSrc* floatCh = dynamic_cast<GRIIOFloatChannelSrc*>(ch);
		if(floatCh) {
			auto start_sptr = floatCh->getGrStartPoint();
			top->connect(src, getOutputIndex(floatCh->getChannelName()),start_sptr[0],0);
		}

		GRIIOComplexChannelSrc* complexCh = dynamic_cast<GRIIOComplexChannelSrc*>(ch);
		if(complexCh) {
			auto start_sptr = complexCh->getGrStartPoint();
			top->connect(src, getOutputIndex(complexCh->getChannelNameI()),start_sptr[0],0);
			top->connect(src, getOutputIndex(complexCh->getChannelNameQ()),start_sptr[1],0);
		}
	}
}

void GRIIODeviceSource::build_blks(GRTopBlock *top)
{
	if(list.count() == 0)
		return;

	computeChannelNames();
	// create block
	src = gr::iio::device_source::make_from(ctx, deviceName.toStdString(), channelNames, phyDeviceName.toStdString(), gr::iio::iio_param_vec_t(), buffersize);
	// match channels with blocks

	end_blk = src;

}

void GRIIODeviceSource::destroy_blks(GRTopBlock *top)
{
	src = nullptr;
	end_blk = nullptr;
}

void GRIIODeviceSource::connect_blk(GRTopBlock *top, GRProxyBlock *)
{
	matchChannelToBlockOutputs(top);
}

void GRIIODeviceSource::disconnect_blk(GRTopBlock *top)
{
	top->getGrBlock()->disconnect(src);
}

void GRIIODeviceSource::addChannel(GRIIOChannel *ch)
{
	list.append(ch);
}

void GRIIODeviceSource::removeChannel(GRIIOChannel *ch)
{
	list.removeAll(ch);
}
