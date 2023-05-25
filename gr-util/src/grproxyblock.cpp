#include "grproxyblock.h"
#include "grtopblock.h"
#include <memory>
#include <iio.h>

using namespace scopy;
GRProxyBlock::GRProxyBlock(QObject *parent) : QObject(parent), m_enabled(true) {}

void GRProxyBlock::build_blks(GRTopBlock *top)
{

}

void GRProxyBlock::destroy_blks(GRTopBlock *top)
{

}

void GRProxyBlock::connect_blk(GRTopBlock *top, GRProxyBlock *src) {
	if(src == nullptr) // block is a source
		return;
	int nrOfOutputs = src->getGrEndPoint()->output_signature()->max_streams();
	for(int i = 0; i < nrOfOutputs;i++) {
		top->connect(src->getGrEndPoint(),i,start_blk,i);
	}
}

void GRProxyBlock::disconnect_blk(GRTopBlock *top) {
	top->getGrBlock()->disconnect(start_blk);
}

void GRProxyBlock::setEnabled(bool v) {
	if(m_enabled == v)
		return;

	m_enabled = v;
	blockSignals(false); // make sure request rebuild is sent
	Q_EMIT requestRebuild();
	blockSignals(!v); // to prevent rebuilding from non-enabled blocks - maybe
}

bool GRProxyBlock::enabled() { return m_enabled; }

bool GRProxyBlock::built()
{
	return (start_blk != nullptr) || (end_blk != nullptr);
}

gr::basic_block_sptr GRProxyBlock::getGrStartPoint() { return start_blk; }
gr::basic_block_sptr GRProxyBlock::getGrEndPoint() { return end_blk; }

GRSignalSrc::GRSignalSrc(QObject *parent) : GRProxyBlock(parent) {}

void GRSignalSrc::setWaveform(gr::analog::gr_waveform_t waveform)
{
	m_waveform = waveform;
	if(sig)
		sig->set_waveform(waveform);
}

void GRSignalSrc::setSamplingFreq(double sr) {
	m_sampling_frequency = sr;
	if(sig)
		sig->set_sampling_freq(m_sampling_frequency);
}
void GRSignalSrc::setFreq(double f) {
	m_freq = f;
	if(sig)
		sig->set_frequency(m_freq);
}

void GRSignalSrc::setPhase(double ph) {
	m_phase = ph;
	if(sig)
		sig->set_phase(m_phase);
}

void GRSignalSrc::setAmplitude(double ampl) {
	m_amplitude = ampl;
	if(sig)
		sig->set_amplitude(m_amplitude);
}

void GRSignalSrc::setOffset(double off) {
	m_offset = off;
	if(sig)
		sig->set_offset(m_offset);
}

void GRSignalSrc::build_blks(GRTopBlock *top) {
	sig = gr::analog::sig_source_f::make(m_sampling_frequency, m_waveform, m_freq,m_amplitude,m_offset,m_phase);
	start_blk = sig;
	end_blk = start_blk;
}

void GRSignalSrc::destroy_blks(GRTopBlock *top) {
	end_blk = nullptr;
	start_blk = nullptr;
	sig = nullptr;

}

GRScaleOffsetProc::GRScaleOffsetProc(QObject *parent) : GRProxyBlock(parent) {}

void GRScaleOffsetProc::setScale(double sc) {
	m_scale = sc;
	if(mul)
		mul->set_k(m_scale);
}

void GRScaleOffsetProc::setOffset(double off) {
	m_offset = off;
	if(add)
		add->set_k(m_offset);
}

void GRScaleOffsetProc::build_blks(GRTopBlock *top) {
	mul = gr::blocks::multiply_const_ff::make(m_scale);
	add = gr::blocks::add_const_ff::make(m_offset);
	top->connect(mul,0,add,0);
	start_blk = mul;
	end_blk  = add;
}

void GRScaleOffsetProc::destroy_blks(GRTopBlock *top) {
	start_blk = end_blk = nullptr;
	mul = nullptr;
	add = nullptr;
}

GRIIOComplexChannelSrc::GRIIOComplexChannelSrc(GRIIODeviceSource *dev, QString channelNameI, QString channelNameQ, QObject *parent) :
      GRIIOChannel(parent), dev(dev), channelNameI(channelNameI), channelNameQ(channelNameQ)
{
}

void GRIIOComplexChannelSrc::build_blks(GRTopBlock *top)
{
	dev->addChannel(this);
	s2f[0] = gr::blocks::short_to_float::make();
	s2f[1] = gr::blocks::short_to_float::make();
	f2c = gr::blocks::float_to_complex::make();

	top->connect(s2f[0],0,f2c,0);
	top->connect(s2f[1],0,f2c,1);
	end_blk = f2c;
}

void GRIIOComplexChannelSrc::destroy_blks(GRTopBlock *top)
{
	dev->removeChannel(this);
	start_blk = end_blk = nullptr;
	s2f[0] = s2f[1] = nullptr;
	f2c = nullptr;
}

const QString &GRIIOComplexChannelSrc::getChannelNameI() const
{
	return channelNameI;
}

const QString &GRIIOComplexChannelSrc::getChannelNameQ() const
{
	return channelNameQ;
}

GRIIOFloatChannelSrc::GRIIOFloatChannelSrc(GRIIODeviceSource *dev, QString channelName, QObject *parent) :
      GRIIOChannel(parent), dev(dev), channelName(channelName)
{
}

void GRIIOFloatChannelSrc::build_blks(GRTopBlock *top)
{
	dev->addChannel(this);
	s2f = gr::blocks::short_to_float::make();
}

void GRIIOFloatChannelSrc::destroy_blks(GRTopBlock *top)
{
	dev->removeChannel(this);
	s2f = nullptr;
	end_blk = nullptr;
}

const QString &GRIIOFloatChannelSrc::getChannelName() const
{
	return channelName;
}


GRIIODeviceSource::GRIIODeviceSource(iio_context *ctx, QString deviceName, QString phyDeviceName, QObject *parent) :
      QObject(parent), ctx(ctx), deviceName(deviceName), phyDeviceName(phyDeviceName)

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
}

void GRIIODeviceSource::matchChannelToBlockOutputs(GRTopBlock *top) {
	for(GRIIOChannel* ch : qAsConst(list)) {
		GRIIOFloatChannelSrc* floatCh = dynamic_cast<GRIIOFloatChannelSrc*>(ch);
		if(floatCh) {

			top->connect(src, getOutputIndex(floatCh->getChannelName()),floatCh->start_blk,0);
		}

		GRIIOComplexChannelSrc* complexCh = dynamic_cast<GRIIOComplexChannelSrc*>(ch);
		if(complexCh) {
			addChannelAtIndex(iio_dev, complexCh->getChannelNameI());
			addChannelAtIndex(iio_dev, complexCh->getChannelNameQ());
		}
	}
}

void GRIIODeviceSource::build_blks(GRTopBlock *top)
{
	computeChannelNames();

	// create block
	src = gr::iio::device_source::make_from(ctx, deviceName.toStdString(), channelNames, phyDeviceName.toStdString(), gr::iio::iio_param_vec_t(), buffersize);
	// match channels with blocks
	matchChannelToBlockOutputs(top);



}

void GRIIODeviceSource::destroy_blks(GRTopBlock *top)
{

}

void GRIIODeviceSource::addChannel(GRIIOChannel *ch)
{
	list.append(ch);
}

void GRIIODeviceSource::removeChannel(GRIIOChannel *ch)
{
	list.removeAll(ch);
}
