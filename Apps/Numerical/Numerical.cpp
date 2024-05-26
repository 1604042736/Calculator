#include <QDebug>
#include <QString>
#include <QProcess>
#include <QDir>
#include <QDateTime>
#include <QFile>
#include <QPainter>
#include <QSvgRenderer>

#include "Numerical.h"
#include "Lexer.h"
#include "Parser.h"
#include "Common.h"

Numerical::Numerical(QWidget *parent) : QWidget(parent), ui(new Ui_Numerical)
{
    ui->setupUi(this);

    connect(ui->pb_cal, &QPushButton::clicked, this, &Numerical::calculate);
}

void Numerical::calculate()
{
    std::string code = ui->le_input->text().toStdString();
    objptr_t result = exec(code, "<numerical>");
    if (result == nullptr)
        return;
    print(result->toPrettyString());

    QString temppath = QDir::tempPath();
    QString filename = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
    QString texfilepath = temppath + "/" + filename + ".tex";
    QString dvifilepath = temppath + "/" + filename + ".dvi";
    QString svgfilepath = temppath + "/" + filename + ".svg";

    qDebug() << "Writing " << texfilepath;
    QFile texfile(texfilepath);
    texfile.open(QIODevice::WriteOnly);
    texfile.write("\\documentclass{standalone}\n");
    texfile.write("\\begin{document}\n");
    texfile.write(("$" + result->toLateX() + "$").data());
    texfile.write("\n\\end{document}");
    texfile.close();

    QProcess p;
    QStringList args;
    args << "-output-directory=" + temppath << texfilepath;
    p.execute("latex", args);

    args.clear();
    args << dvifilepath << "-o" << svgfilepath;
    p.execute("dvisvgm", args);

    QSvgRenderer svg_render(svgfilepath);
    QPixmap pixmap(ui->l_result->size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    svg_render.render(&painter);
    ui->l_result->setPixmap(pixmap);
}