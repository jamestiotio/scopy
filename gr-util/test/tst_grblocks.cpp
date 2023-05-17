#include <QTest>
#include "gr-util/grtopblock.h"
#include "gr-util/grsignalpath.h"
#include "gr-util/grproxyblock.h"

using namespace scopy;

class TST_GRBlocks : public QObject
{
	Q_OBJECT
private Q_SLOTS:
	void test();
};

void TST_GRBlocks::test() {
	GRTopBlock top("aa");
	GRSignalPath *ch1;
	GRSignalSrc *sin1;
	GRScaleOffsetProc *scale_offset;
	ch1 = top.addSignalPath("iio1");
	sin1 = new GRSignalSrc(ch1);
	scale_offset = new GRScaleOffsetProc(ch1);
	ch1->append(sin1);
	ch1->append(scale_offset);
	top.build();

}


QTEST_MAIN(TST_GRBlocks)

#include "tst_grblocks.moc"


