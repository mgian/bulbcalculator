#ifndef DOWNLOADDATA_H
#define DOWNLOADDATA_H


#include "../ui_DownloadData.h"


#include <QtWebKit/QWebView>

class DownloadManager;
class WebView;

class DownloadDataDlg : public QDialog, private Ui::D_DwlData
{
    Q_OBJECT

    public:
        DownloadDataDlg();
        static DownloadManager *downloadManager();


    public slots:
        void LoadUrl(void);
        void ResetProgress(void);
        void SetProgress(int value);
        void DownLoad(const QNetworkRequest&);
        void LoadEnded(void);

    private:
        static DownloadManager *s_downloadManager;
        QNetworkAccessManager *m_manager;


    private slots:

    protected:



};
/*
class WebView : public QWebView {
    Q_OBJECT

    protected:
        void contextMenuEvent(QContextMenuEvent * event);


};
*/
#endif // DOWNLOADDATA_H
