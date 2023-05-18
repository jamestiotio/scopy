#include "grtopblock.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(TOPBLOCK, "GRTopBlock")

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
	top = gr::make_top_block(m_name.toStdString());

	for(GRSignalPath* sig : m_signalPaths) {
		if(sig->enabled() ) {
			sig->connect_blk(this, nullptr);
		}
	}
	return top;
}

void GRTopBlock::unbuild() {
	for(GRSignalPath* sig : m_signalPaths) {
		sig->disconnect_blk(this);
	}
	top->disconnect_all();
}

void GRTopBlock::connect(gr::basic_block_sptr src, int srcPort, gr::basic_block_sptr dst, int dstPort)
{
	qDebug(TOPBLOCK) << "Connecting " << QString::fromStdString(src->name()) << ":" << srcPort
					 << "to" << QString::fromStdString(dst->name()) << ":" << dstPort;
	top->connect(src,srcPort,dst,dstPort);
}

gr::top_block_sptr GRTopBlock::getGrBlock() { return top; }
