
#ifndef ADDMATRIXWIDGET_H
#define ADDMATRIXWIDGET_H

#include <QWidget>
#include <QList>
#include <QLineEdit>
#include <QGridLayout>
#include <QSpinBox>
#include <QStandardItem>
#include "MatrixLibrary.h"

class AddMatrixWidget : public QWidget
{
    Q_OBJECT


    private:

    MatrixLibrary* library;
    QLineEdit* nameMatrix;
    QSpinBox* nbRowsSelector;
    QSpinBox* nbColsSelector;
    QList<QLineEdit*> lineEditsTab;
    QGridLayout* lineEditsLayout;
    unsigned int lrows;
    unsigned int lcols;

    bool controlKeyboardInput () const;


    public:

    explicit AddMatrixWidget(QWidget *parent = nullptr);
    AddMatrixWidget(MatrixLibrary* library, QWidget* parent = nullptr);
    ~AddMatrixWidget();

    signals:

    void matrixAdded(const QString name, const Matrix m);

    public slots:

    private slots:

    void compute_add();
    void update_EditSize();
};

#endif
