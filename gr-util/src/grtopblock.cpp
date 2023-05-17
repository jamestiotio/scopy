#include "grtopblock.h"

using namespace scopy;
GRTopBlock::GRTopBlock(QString name, QObject *parent)
{

}

GRTopBlock::~GRTopBlock()
{

}

GRSignalPath *GRTopBlock::addSignalPath(QString name) {
	GRSignalPath* sig = new GRSignalPath(name, this);
	m_signalPaths.append(sig);
	return sig;
}

gr::top_block_sptr GRTopBlock::build() {
	top = gr::make_top_block(name.toStdString());

	for(GRSignalPath* sig : m_signalPaths) {
		if(sig->enabled() ) {
			sig->connect_blk(this, nullptr);
		}
	}
	return top;
}

// test vector with vector  / dump flattened
// add more blocks (?)
// implement math
// implement recursive build (?)
// implement multiple signalPath sources (?)
// test sources
// test enable disable
// test rebuild
// test cleanup



gr::top_block_sptr GRTopBlock::getGrBlock() { return top; }
