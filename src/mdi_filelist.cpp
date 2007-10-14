#include "mdi_filelist.h"

FileListDlg::FileListDlg(QWidget *parent) : MdiChild(parent)
{	
	setupUi(this);
//	setWindowTitle(tr("")); // set window title in format: "FileList - John" to ./filelist/john.tar.bz2
}
