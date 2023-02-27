#include "testplugin.h"
#include <QBoxLayout>
#include <QJsonDocument>
#include <QLabel>
#include <QPushButton>
#include <QLoggingCategory>
#include <QUuid>
#include <QFile>

Q_LOGGING_CATEGORY(CAT_TESTPLUGIN,"TestPlugin");
using namespace adiscope;

bool TestPlugin::compatible(QString uri) {
	qDebug(CAT_TESTPLUGIN)<<"compatible";
	return true;
}

bool TestPlugin::loadIcon()
{
	SCOPY_PLUGIN_ICON(":/icons/adalm.svg");
	return true;
}

void TestPlugin::postload() {
	qDebug(CAT_TESTPLUGIN)<<"Loaded plugin";
}

bool TestPlugin::loadPage()
{
	m_page = new QWidget();
	QVBoxLayout *lay = new QVBoxLayout(m_page);
	lay->addWidget(new QLabel("TestPage",m_page));
	QPushButton* restartBtn = new QPushButton("restartPlugin",m_page);
	lay->addWidget(restartBtn);
	connect(restartBtn,SIGNAL(clicked()),this,SIGNAL(restartDevice()));
	return true;

}

void TestPlugin::loadToolList()
{
	m_toolList.append(SCOPY_NEW_TOOLMENUENTRY("FirstPlugin",":/icons/scopy-default/icons/tool_home.svg"));
	m_toolList.append(SCOPY_NEW_TOOLMENUENTRY("Alexandra",":/icons/scopy-default/icons/tool_io.svg"));
}

void TestPlugin::unload() {
}

QString TestPlugin::about()
{
	QFile f(":/about.md");
	f.open(QFile::ReadOnly);
	QString content = f.readAll();
	return content;
}


bool TestPlugin::onConnect()
{
	qDebug(CAT_TESTPLUGIN)<<"connect";
	qDebug(CAT_TESTPLUGIN)<<m_toolList[0]->id()<<m_toolList[0]->name();

	m_toolList[0]->setEnabled(true);
	m_toolList[0]->setName("TestPlugin");
	m_toolList[0]->setRunBtnVisible(true);

	tool = new QWidget();
	QVBoxLayout *lay = new QVBoxLayout(tool);
	QLabel *lbl = new QLabel("TestPlugin", tool);
	QLabel *pic = new QLabel("Picture",tool);
	pic->setStyleSheet("border-image: url(\":/testImage.png\") ");
	lay->addWidget(lbl);
	lay->addWidget(pic);
	m_toolList[0]->setTool(tool);

	return true;
}

bool TestPlugin::onDisconnect()
{
	qDebug(CAT_TESTPLUGIN)<<"disconnect";
	for(auto &tool : m_toolList) {
		tool->setEnabled(false);
		tool->setRunBtnVisible(false);
		if(tool->tool()) {
			delete tool->tool();
			tool->setTool(nullptr);
		}
	}
	delete tool;
	return true;
}

void TestPlugin::initMetadata()
{
	loadMetadata(
R"plugin(
	{
	   "priority":2,
	   "category":[
	      "test"
	   ]
	}
)plugin");
}