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
		top->getGrBlock()->connect(src->getGrEndPoint(),i,start_blk,i);
	}
}

void GRProxyBlock::disconnect_blk(GRTopBlock *top) {
	top->getGrBlock()->disconnect(start_blk);
	destroy_blks(top);
}

void GRProxyBlock::setEnabled(bool v) { m_enabled = v; }

bool GRProxyBlock::enabled() { return m_enabled; }

gr::basic_block_sptr GRProxyBlock::getGrStartPoint() { return start_blk; }
gr::basic_block_sptr GRProxyBlock::getGrEndPoint() { return end_blk; }

void GRSignalSrc::build_blks(GRTopBlock *top) {
	start_blk = gr::analog::sig_source_f::make(m_sampling_frequency,gr::analog::GR_SIN_WAVE,m_freq,m_amplitude,m_offset,m_phase);
	end_blk = start_blk;
}

void GRSignalSrc::destroy_blks(GRTopBlock *top) {
	end_blk = start_blk = nullptr;

}

void GRScaleOffsetProc::build_blks(GRTopBlock *top) {
	mul = gr::blocks::multiply_const_ff::make(m_scale);
	add = gr::blocks::add_const_ff::make(m_offset);
	top->getGrBlock()->connect(mul,0,add,0);
	start_blk = mul;
	end_blk  = add;
}

void GRScaleOffsetProc::destroy_blks(GRTopBlock *top) {
	start_blk = end_blk = nullptr;
	mul = nullptr;
	add = nullptr;
}
