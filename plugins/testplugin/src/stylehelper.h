#ifndef STYLEHELPER_H
#define STYLEHELPER_H

#include <QMap>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QStyle>
#include <QPainter>

#include <scopy-testplugin_export.h>
#include <gui/utils.h>
#include <gui/dynamicWidget.h>
#include <gui/menu_anim.hpp>


namespace scopy {
class SCOPY_TESTPLUGIN_EXPORT StyleHelper : public QPushButton {
	Q_OBJECT
protected:
	StyleHelper(QObject *parent = nullptr);
	~StyleHelper();

public:
	// singleton
	StyleHelper(StyleHelper &other) = delete;
	void operator=(const StyleHelper &) = delete;
	static StyleHelper *GetInstance();
public:
	static void initColorMap();
	static QString getColor(QString id);
	static void SquareToggleButtonWithIcon(QPushButton* btn, QString objectName, bool checkable = false);
	static void BlueButton(QPushButton *btn, QString objectName, bool checkable, QSize size = QSize(128,48));
	static void BlueSquareCheckbox(QCheckBox *chk, QString objectName);
	static void ColoredCircleCheckbox(QCheckBox *chk, QColor color, QString objectName);
	static void ColoredSquareCheckbox(QCheckBox *chk, QColor color, QString objectName);
	static void MenuControlLabel(QLabel * lbl, QString objectName);
	static void MenuControlButton(QPushButton *btn, QString objectName, bool checkable = true);
	static void MenuControlWidget(QWidget* w, QColor color, QString objectName);

private:
	QMap<QString,QString> colorMap;
	static StyleHelper * pinstance_;
};

class SCOPY_TESTPLUGIN_EXPORT MenuControlButton : public QAbstractButton {
	Q_OBJECT
public:
	typedef enum {
		CS_CIRCLE,
		CS_SQUARE,
		CS_BLUESQUARE,
	} CheckboxStyle;

	MenuControlButton(QObject *parent = nullptr) {
		lay = new QHBoxLayout(this);
		lay->setMargin(16);
		lay->setSpacing(16);

		setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
		setCheckable(true);
		setLayout(lay);

		m_chk = new QCheckBox("",this);
		m_label = new QLabel("", this);
		m_btn = new QPushButton("",this);
		m_color = StyleHelper::getColor("ScopyBlue");
		m_cs = CS_SQUARE;

		lay->addWidget(m_chk);
		lay->addWidget(m_label);
		lay->addWidget(m_btn);
		applyStylesheet();
		connect(this, &QAbstractButton::toggled, this, [=](bool b){ setDynamicProperty(this,"selected",b);}); // Hackish - QStyle should be implemented
	}

	void setColor(QColor c) {
		m_color = c;
		applyStylesheet();
	}

	void setCheckBoxStyle(MenuControlButton::CheckboxStyle cs) {
		m_cs = cs;
		applyStylesheet();
	}

	void paintEvent(QPaintEvent *e) override {
		// https://forum.qt.io/topic/25142/solved-applying-style-on-derived-widget-with-custom-property-failes/2
		// https://doc.qt.io/qt-5/stylesheet-reference.html

		QStyleOption opt;
		opt.init(this);
		QPainter p(this);
		style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	}

	void setName(QString s) {
		m_label->setText(s);
	}

	QCheckBox* checkBox() { return m_chk;}
	QPushButton* button() { return m_btn;}

private:
	void applyStylesheet() {

		StyleHelper::MenuControlWidget(this,m_color,"controlButton");
		switch(m_cs) {
		case CS_CIRCLE:
			StyleHelper::ColoredCircleCheckbox(m_chk,m_color,"chk");
			break;
		case CS_SQUARE:
			StyleHelper::ColoredSquareCheckbox(m_chk,0xFFFFFF,"chk");
			break;
		default:
			StyleHelper::BlueSquareCheckbox(m_chk,"chk");
			break;
		}
		StyleHelper::MenuControlLabel(m_label,"name");
		StyleHelper::MenuControlButton(m_btn, "btn");
	}

	int width;
	QColor m_color;
	QHBoxLayout *lay;
	QCheckBox *m_chk;
	QLabel *m_label;
	QPushButton *m_btn;
	CheckboxStyle m_cs;
};

//class MenuControlWidget :

class SCOPY_TESTPLUGIN_EXPORT PrintBtn : public QPushButton {
	Q_OBJECT
public:
	PrintBtn(QWidget *parent = nullptr) : QPushButton(parent) {
		StyleHelper::BlueButton(this,"print_btn",false);
		setText("Print");
	}
};

class SCOPY_TESTPLUGIN_EXPORT OpenLastMenuBtn : public QPushButton {
	Q_OBJECT
public:
	OpenLastMenuBtn(MenuAnim *menu, bool opened, QWidget *parent = nullptr) : QPushButton(parent), m_menu(menu) {		QIcon icon1;
		icon1.addPixmap(Util::ChangeSVGColor(":/gui/icons/scopy-default/icons/setup3_unchecked_hover.svg","white",1));
		setIcon(icon1);
		StyleHelper::SquareToggleButtonWithIcon(this,"open_last_menu_btn",true);
		setChecked(opened);
		connect(this, &QPushButton::toggled, m_menu, &MenuAnim::toggleMenu);
	}

private:
	MenuAnim *m_menu;
};

class SCOPY_TESTPLUGIN_EXPORT GearBtn : public QPushButton {
	Q_OBJECT
public:
	GearBtn(QWidget *parent = nullptr) : QPushButton(parent) {
		QIcon icon1;
		icon1.addPixmap(Util::ChangeSVGColor(":/gui/icons/scopy-default/icons/gear_wheel_hover.svg","white",1));
		StyleHelper::SquareToggleButtonWithIcon(this,"gear_btn",true);
		setIcon(icon1);
	}
};

class SCOPY_TESTPLUGIN_EXPORT InfoBtn : public QPushButton {
	Q_OBJECT
public:
	InfoBtn(QWidget *parent = nullptr) : QPushButton(parent) {
		QIcon icon1;
		icon1.addPixmap(Util::ChangeSVGColor(":/gui/icons/scopy-default/icons/info.svg","white",1));
		StyleHelper::SquareToggleButtonWithIcon(this,"info_btn",false);
		setIcon(icon1);
	}
};


class SCOPY_TESTPLUGIN_EXPORT RunBtn : public QPushButton {
	Q_OBJECT
public:
	RunBtn (QWidget *parent = nullptr) : QPushButton(parent) {
		setObjectName("run_btn");
		setCheckable(true);
		setChecked(false);
		setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
		setText("Run");
		connect(this,&QPushButton::toggled,this,[=](bool b){ setText(b ? "Stop" : "Run");});
		QString style = QString(R"css(
			QPushButton {
				width: 88px;
				height: 48px;
					border-radius: 2px;
					text-align: left;
					padding-left: 20px;
					padding-right: 20px;
				color: white;
					font-weight: 700;
					font-size: 14px;
			}

			QPushButton:!checked {
				background-color: #27b34f;
			}

			  QPushButton:checked {
				background-color: #F45000;
			}

			QPushButton:disabled {
				background-color: grey;
			})css");

			setStyleSheet(style);
		QIcon icon1;
		icon1.addPixmap(Util::ChangeSVGColor(":/gui/icons/play.svg","white",1), QIcon::Normal, QIcon::Off);
		icon1.addPixmap(Util::ChangeSVGColor(":/gui/icons/scopy-default/icons/play_stop.svg","white",1), QIcon::Normal, QIcon::On);
		setIcon(icon1);
		setLayoutDirection(Qt::RightToLeft);
	}
};

class SCOPY_TESTPLUGIN_EXPORT SingleShotBtn : public QPushButton {
	Q_OBJECT
public:
	SingleShotBtn (QWidget *parent = nullptr) : QPushButton(parent) {
		setObjectName("single_btn");
		setCheckable(true);
		setChecked(false);
		setText("Single");
		connect(this,&QPushButton::toggled,this,[=](bool b){ setText( b ? "Stop" : "Single");});
		QString style = QString(R"css(
			QPushButton {
				width: 88px;
				height: 48px;
					border-radius: 2px;
					text-align: left;
					padding-left: 20px;
					padding-right: 20px;
				color: white;
					font-weight: 700;
					font-size: 14px;
				opacity: 80;
			}

			QPushButton:!checked {
				background-color: #ff7200;
			}

			  QPushButton:checked {
				background-color: #F45000;
			}

			QPushButton:disabled {
				background-color: grey;
			}
			)css");

			setStyleSheet(style);
		QIcon icon1;
		icon1.addPixmap(Util::ChangeSVGColor(":/gui/icons/play_oneshot.svg","white",1), QIcon::Normal, QIcon::Off);
		icon1.addPixmap(Util::ChangeSVGColor(":/gui/icons/scopy-default/icons/play_stop.svg","white",1), QIcon::Normal, QIcon::On);
		setIcon(icon1);
		setLayoutDirection(Qt::RightToLeft);
	}
};
}

#endif // STYLEHELPER_H
