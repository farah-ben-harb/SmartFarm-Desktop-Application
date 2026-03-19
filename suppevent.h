#ifndef SUPPEVENT_H
#define SUPPEVENT_H

#include <QDialog>

#include <QDialog>

namespace Ui {
class SuppEvent;
}

class SuppEvent : public QDialog
{
    Q_OBJECT

public:
    explicit SuppEvent(QWidget *parent = nullptr);
    ~SuppEvent();

signals:
    void suppressionDemandee(int id);

private slots:
    void on_pushButton_event_supp_clicked();

private:
    Ui::SuppEvent*ui;
};

#endif // SUPPEVENT_H
