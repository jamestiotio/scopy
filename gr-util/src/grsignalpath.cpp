#include "grsignalpath.h"
#include "grtopblock.h"

using namespace scopy;

GRSignalPath::GRSignalPath(QString name, QObject *parent) : GRProxyBlock(parent), name(name) {}

GRProxyBlock *GRSignalPath::endPoint() {
	QListIterator<GRProxyBlock*> i(list);
	i.toBack();
	while (i.hasPrevious()) {
		if(i.previous()->enabled())
			return i.previous();
	}
	return nullptr;
}

void GRSignalPath::append(GRProxyBlock *p) {
	list.append(p);
}

void GRSignalPath::build_blks() {


}

void GRSignalPath::destroy_blks() {


}

void GRSignalPath::connect_blk(GRTopBlock *top, GRProxyBlock *src) {
	GRProxyBlock* prevBlk = src;
	for(GRProxyBlock* blk : list) {
		if(blk->enabled()) {
			blk->build_blks(top);
			blk->connect_blk(top, prevBlk);
			prevBlk = blk;
		}
	}
}

void GRSignalPath::disconnect_blk() {}
