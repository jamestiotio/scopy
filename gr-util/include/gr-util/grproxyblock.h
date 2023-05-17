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
	gr::basic_block_sptr getGrStartPoint();
	gr::basic_block_sptr getGrEndPoint();
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
	void setSamplingFreq(double sr) { m_sampling_frequency = sr;}
	void setFreq(double f) { m_freq = f;}
	void setPhase(double ph) { m_phase = ph;}
	void setAmplitude(double ampl) { m_amplitude = ampl; }
	void setOffset(double off) { m_offset = off;}

	void build_blks(GRTopBlock *top);
	void destroy_blks(GRTopBlock *top);

protected:
	double m_sampling_frequency;
	double m_freq;
	double m_phase;
	double m_amplitude;
	double m_offset;
};

class SCOPY_GR_UTIL_EXPORT GRScaleOffsetProc : public GRProxyBlock {
public:
	GRScaleOffsetProc(QObject *parent = nullptr) : GRProxyBlock(parent) {};
	void setScale(double sc) { m_scale = sc;}
	void setOffset(double off) { m_offset = off;}
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
