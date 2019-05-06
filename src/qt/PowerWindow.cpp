
#include <QPushButton>
#include <QDebug>
#include "PowerWindow.h"
#include "Error.h"


PowerWindow::PowerWindow(MatrixLibrary* lib, QWidget* parent) : QWidget (parent)
{
    this->lib = lib;
    op1Name = "_";
    op1 = nullptr;
    op2 = 1;

    resultImg = new ShowMatrixWidget(this);
    resultImg->hide();

    QLabel* title = new QLabel("Puissance");
    title->setStyleSheet("font-size: 30px; font:bold;");
    title->setAlignment(Qt::AlignCenter);
    title->setContentsMargins(0,0,0,10);

    QVBoxLayout* op1ChoiceLayout = new QVBoxLayout;
    QLabel* op1Title = new QLabel("Choix de la matrice : ");
    op1Title -> setAlignment(Qt::AlignCenter);
    op1View = new MatrixViewWidget(lib, this);
    op1ChoiceLayout->addWidget(op1Title);
    op1ChoiceLayout->addWidget(op1View);
    op1ChoiceLayout->setContentsMargins(0,0,20,0);

    QVBoxLayout* op2ChoiceLayout = new QVBoxLayout;
    QLabel* op2Title = new QLabel("Choix de la puissance :");
    op2Title -> setAlignment(Qt::AlignBottom);
    op2Edit = new QSpinBox;
    op2Edit->setRange(-1,100);
    op2Edit->setFixedSize(75,35);
    op2Edit->setStyleSheet("QSpinBox{border: 1px solid grey; border-radius: 3px; font-size: 15px;}"
                           "QSpinBox::down-button{width: 20px; background:"
                           "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 lightBlue, stop: 1 blue);}"
                           "QSpinBox::down-arrow{ image: url(:/img/minus.png); width: 12px; height: 30px;}"
                           "QSpinBox::up-button{width: 20px; background:"
                           "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 lightBlue, stop: 1 blue)}"
                           "QSpinBox::up-arrow { image: url(:/img/plus.png); width: 35px; height:35px;}");

    op2Edit->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    op2Edit->setValue(1);
    op2Edit->setAlignment(Qt::AlignHCenter);
    op2ChoiceLayout->addWidget(op2Title);
    op2ChoiceLayout->addWidget(op2Edit);
    op2ChoiceLayout->setContentsMargins(20,0,0,0);

    QHBoxLayout* formLayout = new QHBoxLayout;
    formLayout->addLayout(op1ChoiceLayout);
    formLayout->addLayout(op2ChoiceLayout);
    formLayout->setAlignment(Qt::AlignCenter);


    QWidget* formWidget = new QWidget;
    formWidget->setLayout(formLayout);
    formWidget->setMaximumWidth(1330);
    formWidget->setMaximumHeight(350);

    formula = new QLabel("_  ^  _");
    formula->setStyleSheet("font-size: 40px");
    formula->setAlignment(Qt::AlignCenter);

    QPushButton* calculer = new QPushButton("Calculer");
    calculer->setCursor(Qt::PointingHandCursor);
    calculer->setStyleSheet("QPushButton{ background-color: lightGrey } "
                            "QPushButton:hover{ background-color: lightBlue }");
    calculer->setFixedSize(300,30);

    QVBoxLayout* buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(calculer);
    buttonLayout->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    QVBoxLayout* subLayout1 = new QVBoxLayout;
    subLayout1->addWidget(title);
    subLayout1->addWidget(formWidget);
    subLayout1->addWidget(formula);

    QWidget* subWidget1 = new QWidget(this);
    subWidget1->setLayout(subLayout1);
    subWidget1->setFixedHeight(400);

    mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(10);
    mainLayout->addWidget(subWidget1);
    mainLayout->addWidget(resultImg);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);


    connect(op1View, &MatrixViewWidget::clicked,
            [this] () -> void
            {
                this->computeSelection(0);
            });

    connect(calculer, &QPushButton::clicked, this, &PowerWindow::computeOperation);

    connect(op2Edit, QOverload<int>::of(&QSpinBox::valueChanged),
            [this] () -> void
            {
                this->computeSelection(1);
            });



    op1View->refresh(
                [] (const Matrix* a) -> bool
                {
                    return a->isSQMatrix();
                });

    setLayout(mainLayout);
}


void PowerWindow:: computeSelection (bool op)
{
    if(!op)
    {
        int selectedRow;
        QString selectedName;
        resultImg->hide();
        mainLayout->setSizeConstraint(QLayout::SetFixedSize);
        updateGeometry();

        selectedRow = op1View->currentIndex().row();
        selectedName = op1View->model()->item(selectedRow)->data(2).toString();
        op1Name = selectedName;
        op1 = lib->find(selectedName.toStdString());
    }
    else
    {
        op2 = op2Edit->value();
    }

    formula->setText(op1Name + " ^ " + QString::number(op2));
}


void PowerWindow:: computeOperation ()
{
    if(op1 == nullptr)
    {
        showError("Opérande Manquante !", "Veuillez bien sélectionner"
                " vos 2 Opérandes !", this);
        return;
    }
    else if((op2 == -1) && (op1->determinant() == 0.0))
    {
        showError("Calcul Impossible !", "La Matrice " + op1Name +
                  " n'est pas inversible !", this);
        return;
    }

    result = (*op1) ^ op2;
    resultImg->computeImgMatrix(&result, palette().color(backgroundRole()));
    resultImg->show();
}


PowerWindow:: ~PowerWindow ()
{
}
