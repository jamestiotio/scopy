#include "grtopblock.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(TOPBLOCK, "GRTopBlock")

using namespace scopy;
GRTopBlock::GRTopBlock(QString name, QObject *parent) : running(false), built(false)
{

}

GRTopBlock::~GRTopBlock()
{

}

GRSignalPath *GRTopBlock::addSignalPath(QString name) {
	GRSignalPath* sig = new GRSignalPath(name, this);
	m_signalPaths.append(sig);
	QObject::connect(sig,SIGNAL(requestRebuild()),this,SLOT(rebuild()));
	return sig;
}

void GRTopBlock::build() {
	top = gr::make_top_block(m_name.toStdString());

	for(GRSignalPath* sig : m_signalPaths) {
		if(sig->enabled() ) {
			sig->connect_blk(this, nullptr);
		}
	}
	Q_EMIT builtSignalPaths();
	built = true;
}

void GRTopBlock::teardown() {
	built = false;
	for(GRSignalPath* sig : m_signalPaths) {
		sig->disconnect_blk(this);
	}
	top->disconnect_all();
}

void GRTopBlock::start()
{
	running = true;
	top->start();
}

void GRTopBlock::stop()
{
	running = false;
	top->stop();
	top->wait(); // ??
}

void GRTopBlock::rebuild() {
	if(running)
		stop();

	if(built) {
		teardown();
		build();
	}
}

void GRTopBlock::connect(gr::basic_block_sptr src, int srcPort, gr::basic_block_sptr dst, int dstPort)
{
	qDebug(TOPBLOCK) << "Connecting " << QString::fromStdString(src->name()) << ":" << srcPort
					 << "to" << QString::fromStdString(dst->name()) << ":" << dstPort;
	top->connect(src,srcPort,dst,dstPort);
}

gr::top_block_sptr GRTopBlock::getGrBlock() { return top; }
/////////////////////////////
