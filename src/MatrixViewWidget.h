
#ifndef MATRIXVIEWWIDGET_H
#define MATRIXVIEWWIDGET_H

#include <QTableView>
#include <QStandardItemModel>
#include "MatrixLibrary.h"

class MatrixViewWidget : public QTableView
{

    Q_OBJECT

public:

    MatrixViewWidget(MatrixLibrary* lib, QWidget* parent = nullptr);
    ~MatrixViewWidget ();
    QStandardItemModel* model () const;

private:

    QStandardItemModel* matrixModel;
    MatrixLibrary* lib;

public slots:

    void refresh(const std::function<bool(Matrix*)> filter =
    [](const Matrix*) -> bool {return true;});
    void addNewRow(const QString name, const Matrix matrix);
};

#endif
