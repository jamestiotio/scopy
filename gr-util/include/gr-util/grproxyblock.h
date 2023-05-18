#ifndef GRPROXYBLOCK_H
#define GRPROXYBLOCK_H

#include <QObject>
#include <gnuradio/blocks/add_const_ff.h>
#include <gnuradio/blocks/multiply_const.h>
#include <gnuradio/analog/sig_source.h>
#include "scopy-gr-util_export.h"

namespace scopy {

class GRTopBlock;

class SCOPY_GR_UTIL_EXPORT GRProxyBlock : public QObject
{
	Q_OBJECT
public:
	GRProxyBlock(QObject *parent = nullptr);;
	virtual ~GRProxyBlock() {};

	virtual void build_blks(GRTopBlock *top);
	virtual void destroy_blks(GRTopBlock *top);
	virtual void connect_blk(GRTopBlock *top, GRProxyBlock *src);
	virtual void disconnect_blk(GRTopBlock *top);

	void setEnabled(bool v);
	bool enabled();
	bool built();
	virtual gr::basic_block_sptr getGrStartPoint();
	virtual gr::basic_block_sptr getGrEndPoint();
protected:
	gr::basic_block_sptr start_blk;
	gr::basic_block_sptr end_blk;
	bool m_enabled;
	bool m_built;
};



class SCOPY_GR_UTIL_EXPORT GRSignalSrc : public GRProxyBlock {
	Q_OBJECT
public:
	GRSignalSrc(QObject *parent = nullptr) : GRProxyBlock(parent) {};
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
	GRScaleOffsetProc(QObject *parent = nullptr) : GRProxyBlock(parent) {};
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
}

#endif // GRPROXYBLOCK_H
