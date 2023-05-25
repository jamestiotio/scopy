#ifndef GRPROXYBLOCK_H
#define GRPROXYBLOCK_H

#include <QObject>
#include <QSet>
#include <gnuradio/blocks/add_const_ff.h>
#include <gnuradio/blocks/multiply_const.h>
#include <gnuradio/blocks/short_to_float.h>
#include <gnuradio/blocks/float_to_complex.h>
#include <gnuradio/iio/device_source.h>
#include <gnuradio/analog/sig_source.h>
#include "scopy-gr-util_export.h"
#include <iio.h>

namespace scopy {

class GRTopBlock;

class SCOPY_GR_UTIL_EXPORT GRProxyBlock : public QObject
{
	Q_OBJECT
public:
	GRProxyBlock(QObject *parent = nullptr);
	virtual ~GRProxyBlock() {};

	virtual void build_blks(GRTopBlock *top);
	virtual void destroy_blks(GRTopBlock *top);
	virtual void connect_blk(GRTopBlock *top, GRProxyBlock *src);
	virtual void disconnect_blk(GRTopBlock *top);

	void setEnabled(bool v);
	bool enabled();
	bool built();
	virtual gr::basic_block_sptr getGrStartPoint(); // QList of (?)
	virtual gr::basic_block_sptr getGrEndPoint();

Q_SIGNALS:
	void requestRebuild();

protected:
	gr::basic_block_sptr start_blk; // QList of (?)
	gr::basic_block_sptr end_blk;
	bool m_enabled;
	bool m_built;
};



class SCOPY_GR_UTIL_EXPORT GRSignalSrc : public GRProxyBlock {
	Q_OBJECT
public:
	GRSignalSrc(QObject *parent = nullptr);
	void setWaveform(gr::analog::gr_waveform_t waveform);
	void setSamplingFreq(double sr);
	void setFreq(double f);
	void setPhase(double ph);
	void setAmplitude(double ampl);
	void setOffset(double off);

	void build_blks(GRTopBlock *top);
	void destroy_blks(GRTopBlock *top);

protected:
	gr::analog::sig_source_f::sptr sig;
	gr::analog::gr_waveform_t m_waveform;
	double m_sampling_frequency;
	double m_freq;
	double m_phase;
	double m_amplitude;
	double m_offset;
};

class SCOPY_GR_UTIL_EXPORT GRScaleOffsetProc : public GRProxyBlock {
public:
	GRScaleOffsetProc(QObject *parent = nullptr);
	void setScale(double sc);
	void setOffset(double off);
	void build_blks(GRTopBlock *top);
	void destroy_blks(GRTopBlock *top);

protected:

	gr::blocks::add_const_ff::sptr add;
	gr::blocks::multiply_const_ff::sptr mul;
	double m_scale;
	double m_offset;
};

typedef GRProxyBlock GRIIOChannel;


class SCOPY_GR_UTIL_EXPORT GRIIODeviceSource : public QObject { // is this a proxy block
public:
	GRIIODeviceSource(iio_context *ctx, QString deviceName, QString phyDeviceName, QObject *parent = nullptr);
	void build_blks(GRTopBlock *top);
	void destroy_blks(GRTopBlock *top);
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
	void matchChannelToBlockOutputs();
};

class SCOPY_GR_UTIL_EXPORT GRIIOComplexChannelSrc : public GRIIOChannel {
public:
	GRIIOComplexChannelSrc(GRIIODeviceSource* dev, QString channelNameI, QString channelNameQ,QObject *parent = nullptr);

	void build_blks(GRTopBlock *top);
	void destroy_blks(GRTopBlock *top);

	const QString &getChannelNameI() const;
	const QString &getChannelNameQ() const;

protected:
	QString channelNameI;
	QString channelNameQ;
	GRIIODeviceSource* dev;
	gr::blocks::short_to_float::sptr s2f[2];
	gr::blocks::float_to_complex::sptr f2c;

};

class SCOPY_GR_UTIL_EXPORT GRIIOFloatChannelSrc : public GRIIOChannel {
public:
	GRIIOFloatChannelSrc(GRIIODeviceSource* dev,QString channelName,QObject *parent = nullptr);

	void build_blks(GRTopBlock *top);
	void destroy_blks(GRTopBlock *top);

	const QString &getChannelName() const;

protected:
	QString channelName;
	GRIIODeviceSource* dev;
	gr::blocks::short_to_float::sptr s2f;

};


}

#endif // GRPROXYBLOCK_H
