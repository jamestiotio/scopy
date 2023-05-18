#include "grproxyblock.h"
#include "grtopblock.h"

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

void GRProxyBlock::setEnabled(bool v) { m_enabled = v; }

bool GRProxyBlock::enabled() { return m_enabled; }

bool GRProxyBlock::built()
{
	return (start_blk != nullptr) || (end_blk != nullptr);
}

gr::basic_block_sptr GRProxyBlock::getGrStartPoint() { return start_blk; }
gr::basic_block_sptr GRProxyBlock::getGrEndPoint() { return end_blk; }

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
