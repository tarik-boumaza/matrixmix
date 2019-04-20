
#include <QPushButton>
#include <QDebug>
#include "InverseWindow.h"
#include "Error.h"


InverseWindow::InverseWindow(MatrixLibrary* lib, QWidget* parent) : QDialog (parent)
{
    this->lib = lib;
    op1 = nullptr;

    resultImg = new ShowMatrixWidget(this);
    resultImg->hide();

    QLabel* title = new QLabel("Inverse");
    title->setStyleSheet("font-size: 30px;");
    title->setAlignment(Qt::AlignCenter);

    QVBoxLayout* op1ChoiceLayout = new QVBoxLayout;
    QLabel* op1Title = new QLabel("Choix de la matrice : ");
    op1View = new MatrixViewWidget(lib, this);
    op1View->setFixedSize(200, 200);
    op1ChoiceLayout->addWidget(op1Title);
    op1ChoiceLayout->addWidget(op1View);

    QHBoxLayout* formLayout = new QHBoxLayout;
    formLayout->addLayout(op1ChoiceLayout);

    op1Name = new QLabel("_");
    op1Name->setStyleSheet("font-size: 40px");
    op1Name->setAlignment(Qt::AlignCenter);
    op1Name->setContentsMargins(0,25,0, 25);

    QPushButton* calculer = new QPushButton("Calculer");

    QVBoxLayout* subLayout1 = new QVBoxLayout;
    subLayout1->addWidget(title);
    subLayout1->addLayout(formLayout);
    subLayout1->addWidget(op1Name);
    QWidget* subWidget1 = new QWidget(this);
    subWidget1->setLayout(subLayout1);
    subWidget1->setFixedHeight(400);

    mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(10);
    mainLayout->addWidget(subWidget1);
    mainLayout->addWidget(resultImg);
    mainLayout->addWidget(calculer);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);


    connect(op1View, &MatrixViewWidget::clicked,
            [this] () -> void
            {
                this->computeSelection();
            });
    connect(calculer, &QPushButton::clicked, this, &InverseWindow::computeOperation);

    op1View->refresh(
                [](const Matrix* a) -> bool
                {
                    return ((a->isSQMatrix()) && (a->determinant() != 0.0));
                });

    setLayout(mainLayout);
}


void InverseWindow:: computeSelection ()
{
    int selectedRow;
    QString selectedName;
    resultImg->hide();
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    updateGeometry();

    selectedRow = op1View->currentIndex().row();
    selectedName = op1View->model()->item(selectedRow)->data(2).toString();
    op1Name->setText(selectedName);
    op1 = lib->find(op1Name->text().toStdString());
}


void InverseWindow:: computeOperation ()
{
    if(op1 == nullptr)
    {
        showError("Opérande Manquante !", "Veuillez bien sélectionner votre Matrice !", this);
        return;
    }

    result = op1->inverse();
    resultImg->computeImgMatrix(&result, palette().color(backgroundRole()));
    resultImg->show();
}


InverseWindow:: ~InverseWindow ()
{
}

