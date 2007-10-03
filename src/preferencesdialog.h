#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include "ui_preferencesdialog.h"
#include <QDialog>
#include <QDir>
#include <QFontDialog>
#include <QHeaderView>
#include <QItemDelegate>
#include <QKeyEvent>
#include <QLocale>

class PreferencesDialogPrivate;

class PreferencesDialog : public QDialog, private Ui::PreferencesDialog 
{
	Q_OBJECT
	
private:
	void accept();
	void initCategoryList();
	void initLookNFeelPage();
	
private slots:
	void on_closeButton_clicked();
	void on_categoryList_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
	
public:
	PreferencesDialog(QWidget *);
/*
public slots:
	void updateIconSet();
	void updateTranslation();

private slots:
	void on_addButton_clicked();
	void on_categoryList_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
	void on_deleteButton_clicked();
	void on_downButton_clicked();
	void on_fontButton_clicked();
	void on_patternLine_textChanged(const QString &);
	void on_testLine_textChanged(const QString &);
	void on_upButton_clicked();
	void crossfadeChanged();
	void iconsetChanged(QListWidgetItem *);
	void localeChanged(QListWidgetItem *);
	void notifierChanged(int);
	void outputChanged(QTreeWidgetItem *, int);
	void serverSelectionChanged();
	void styleChanged(QListWidgetItem *);

private:
	void initServerPage();
	void initConnectionPage();
	void initLookAndFeelPage();
	void initIconSetPage();
	void initLibraryPage();
	void initDirectoriesPage();
	void initPlaylistPage();
	void initStylePage();
	void initShortcutPage();
	void initLanguagePage();
	void initDynamicPlaylistPage();
	void initNotificationsPage();
	void initTagGuesserPage();
	void initTrayIconPage();
	void setFontString(const QFont &);
*/	
};

#endif
