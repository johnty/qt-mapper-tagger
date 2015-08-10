#ifndef TEXTVIEWTAB_H
#define TEXTVIEWTAB_H

#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>

class textviewtab: public QTabWidget
{
public:
    textviewtab(QTabWidget *parent);
    void setText(QString text);
private:
    QTextEdit *textBox;
};

#endif // TEXTVIEWTAB_H
