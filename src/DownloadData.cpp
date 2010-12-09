#include "../include/DownloadData.h"
#include "../include/BulbCalculator.h"
#include "../include/downloadmanager.h"

#include "QtGui"

#include <QtWebKit/QWebHitTestResult>

DownloadManager *DownloadDataDlg::s_downloadManager = 0;


DownloadDataDlg::DownloadDataDlg() {

    setupUi(this);
    this->PB_Progress->hide();
    this->CB_Url->addItem(QString("http://www.ae.illinois.edu/m-selig/ads/coord_database.html"));
    this->connect(BB_Apply->button(QDialogButtonBox::Open), SIGNAL(clicked()), this, SLOT(LoadUrl()));
    connect(webView->page(), SIGNAL(downloadRequested(const QNetworkRequest&)), this, SLOT(DownLoad(const QNetworkRequest&)));
    connect(webView->page(), SIGNAL(loadStarted()), this, SLOT(ResetProgress()));
    connect(webView->page(), SIGNAL(loadProgress(int)), this, SLOT(SetProgress(int)));
    connect(webView->page(), SIGNAL(loadFinished(bool)), this, SLOT(LoadEnded()));


}


DownloadManager *DownloadDataDlg::downloadManager() {
    if (!s_downloadManager) {
        s_downloadManager = new DownloadManager();
    }
    return s_downloadManager;
}


void DownloadDataDlg::LoadEnded(void) {

    this->PB_Progress->hide();

}

void DownloadDataDlg::DownLoad(const QNetworkRequest& request) {

    DownloadDataDlg::downloadManager()->download(request);

}


void DownloadDataDlg::LoadUrl(void) {

    webView->load(QUrl(this->CB_Url->currentText()));
    this->PB_Progress->show();

}

void DownloadDataDlg::ResetProgress(void) {

    this->PB_Progress->show();
    this->PB_Progress->setValue(0);

}

void DownloadDataDlg::SetProgress(int value) {

    this->PB_Progress->setValue(value);

}

