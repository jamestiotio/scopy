#include "grsignalpath.h"
#include "grtopblock.h"

using namespace scopy;

GRSignalPath::GRSignalPath(QString name, QObject *parent) : GRProxyBlock(parent), m_name(name) {}


void GRSignalPath::append(GRProxyBlock *p) {	
	list.append(p);
}

void GRSignalPath::build_blks() {


}

void GRSignalPath::destroy_blks() {


}

gr::basic_block_sptr GRSignalPath::getGrStartPoint()
{
	for(int i = 0;i<list.count();i++) {
		auto blk = list[i];
		if(blk->enabled()) {
			return blk->getGrStartPoint();
		}
	}
	return nullptr;
}

gr::basic_block_sptr GRSignalPath::getGrEndPoint()
{
	for(int i = list.count() - 1; i >= 0;i--) {
		auto blk = list[i];
		if(blk->enabled()) {
			return blk->getGrEndPoint();
		}
	}
	return nullptr;
}

void GRSignalPath::connect_blk(GRTopBlock *top, GRProxyBlock *src) {
	GRProxyBlock* prevBlk = src;
	for(GRProxyBlock* blk : list) {
		if(blk->enabled() && !blk->built()) {
			blk->build_blks(top);
			blk->connect_blk(top, prevBlk);
			prevBlk = blk;
		}
	}
}

void GRSignalPath::disconnect_blk(GRTopBlock *top) {
	for(GRProxyBlock* blk : list) {
		if(blk->built()) {
			blk->disconnect_blk(top);
			blk->destroy_blks(top);
		}
	}
}


QString GRSignalPath::name() const
{
	return m_name;
}
