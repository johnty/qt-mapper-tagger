#include "textviewtab.h"

textviewtab::textviewtab(QTabWidget *parent)
{
    textBox = new QTextEdit(this);
    textBox->setText(QString("This displays the JSON mapping file in text format..."));
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(textBox);
}

void textviewtab::setText(QString text)
{
    textBox->setText(text);
}

