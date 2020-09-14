#include "calculatorfx.h"
#include "ui_calculatorfx.h"
//#include <QToolButton>
//#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QtMath>
CalculatorFX::CalculatorFX(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CalculatorFX)
{
    ui->setupUi(this);
    ui->lineEdit_Input->installEventFilter(this);
//给函数toolButton添加菜单
    QMenu *menu_function=new QMenu();
    QAction *action1 = menu_function->addAction("                      1                      ");//to be improved
    QAction *action2 = menu_function->addAction("                      2                      ");
    ui->tBtn_function->setMenu(menu_function);
    ui->lineEdit_Input->setFocus();
    calPattern_current="科学计算";
    tri_current="角度制";
    cal_complete=false;
    Ans="@";
    pi.setNum(qAcos(-1));
    e.setNum(qExp(1));
    connectAllSlots();
}

CalculatorFX::~CalculatorFX()
{
    delete ui;
}

bool CalculatorFX::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->lineEdit_Input)
    {
        if(event->type()==QEvent::MouseButtonRelease&&cal_complete)
        {
            ui->lineEdit_Result->clear();
            cal_complete=false;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void CalculatorFX::connectAllSlots(){
    QPushButton *opNormal_restart[25]={
        ui->pBtn_0,     ui->pBtn_1,         ui->pBtn_2,         ui->pBtn_3,         ui->pBtn_4,
        ui->pBtn_5,     ui->pBtn_6,         ui->pBtn_7,         ui->pBtn_8,         ui->pBtn_9,
        ui->pBtn_A,     ui->pBtn_B,         ui->pBtn_C,         ui->pBtn_D,         ui->pBtn_E,
        ui->pBtn_F,     ui->pBtn_pi,        ui->pBtn_e,         ui->pBtn_lBracket,  ui->pBtn_rbracket,
        ui->pBtn_dot,   ui->pBtn_not,       ui->pBtn_comma,     ui->pBtn_0b,        ui->pBtn_0x
    };

    QPushButton *opNormal_continue[13]={
        ui->pBtn_Add,   ui->pBtn_sub,       ui->pBtn_mul,       ui->pBtn_div,       ui->pBtn_mod,
        ui->pBtn_power, ui->pBtn_square,    ui->pBtn_percent,   ui->pBtn_and,       ui->pBtn_or,
        ui->pBtn_xor,   ui->pBtn_sll,       ui->pBtn_sra
    };

    QPushButton *opFunction[11]={
        ui->pBtn_sin,       ui->pBtn_cos,   ui->pBtn_tan,   ui->pBtn_arcsin,    ui->pBtn_arccos,
        ui->pBtn_arctan,    ui->pBtn_log,   ui->pBtn_lg,    ui->pBtn_ln,        ui->pBtn_sqrt,
        ui->pBtn_abs
    };

    QRadioButton *optionCal[5]={
        ui->rBtn_sciCal,    ui->rBtn_bin,   ui->rBtn_oct,   ui->rBtn_hex,   ui->rBtn_mix
    };

    QRadioButton *optionTri[2]={
        ui->rBtn_angle,     ui->rBtn_radian
    };

    for(auto btn : opNormal_restart){
        connect(btn, &QPushButton::clicked, this, &CalculatorFX::opNormal_restart_clicked);
    }

    for(auto btn : opNormal_continue){
        connect(btn, &QPushButton::clicked, this, &CalculatorFX::opNormal_continue_clicked);
    }

    for(auto btn : opFunction){
        connect(btn, &QPushButton::clicked, this, &CalculatorFX::opFunction_clicked);
    }

    connect(ui->pBtn_equal, &QPushButton::clicked, this, &CalculatorFX::equal_clicked);

    connect(ui->pBtn_Ans, &QPushButton::clicked, this, &CalculatorFX::Ans_clicked);

    connect(ui->pBtn_DEL, &QPushButton::clicked, this, &CalculatorFX::DEL_clicked);

    connect(ui->pBtn_AC, &QPushButton::clicked, this, &CalculatorFX::AC_clicked);

    for(auto btn : optionCal){
        connect(btn, &QRadioButton::toggled, this, &CalculatorFX::optionCal_toggled);
    }

    for(auto btn : optionTri){
        connect(btn, &QRadioButton::toggled, this, &CalculatorFX::optionTri_toggled);
    }
}

void CalculatorFX::opNormal_restart_clicked(){
    QPushButton* pBtn = qobject_cast<QPushButton*>(sender());
    if(cal_complete==true){
        ui->lineEdit_Input->clear();
        ui->lineEdit_Result->clear();
    }
    int temp_position=ui->lineEdit_Input->cursorPosition();
    QString temp=ui->lineEdit_Input->text().insert(temp_position,pBtn->text());
    ui->lineEdit_Input->setText(temp);
    ui->lineEdit_Input->setCursorPosition(temp_position+pBtn->text().size());
    ui->lineEdit_Input->setFocus();
    cal_complete=false;
}

void CalculatorFX::opNormal_continue_clicked(){
    QPushButton* pBtn = qobject_cast<QPushButton*>(sender());
    if(cal_complete==true){
        ui->lineEdit_Input->setText("Ans");
        ui->lineEdit_Result->clear();
    }
    int temp_position=ui->lineEdit_Input->cursorPosition();
    QString temp=ui->lineEdit_Input->text().insert(temp_position,pBtn->text());
    ui->lineEdit_Input->setText(temp);
    ui->lineEdit_Input->setCursorPosition(temp_position+pBtn->text().size());
    ui->lineEdit_Input->setFocus();
    cal_complete=false;
}

void CalculatorFX::opFunction_clicked(){
    QPushButton* pBtn = qobject_cast<QPushButton*>(sender());
    if(cal_complete==true){
        ui->lineEdit_Input->clear();
        ui->lineEdit_Result->clear();
    }
    int temp_position=ui->lineEdit_Input->cursorPosition();
    QString addition=pBtn->text()+"(";
    QString temp=ui->lineEdit_Input->text().insert(temp_position,addition);
    ui->lineEdit_Input->setText(temp);
    ui->lineEdit_Input->setCursorPosition(temp_position+addition.size());
    ui->lineEdit_Input->setFocus();
    cal_complete=false;
}

void CalculatorFX::equal_clicked(){
    QString sys_temp=CalculatorFX::sysPre(calPattern_current,ui->lineEdit_Input->text());
    CalculatorFX::preprocessing(sys_temp,semi_stack);
    QString result_dec=CalculatorFX::calculate(semi_stack);
    QString result_done=CalculatorFX::sysConversion("科学计算", calPattern_current, result_dec);
    ui->lineEdit_Result->setText(result_done);
    Ans=result_dec;
    cal_complete=true;
}

void CalculatorFX::Ans_clicked(){
    if(Ans=="@"){
        QMessageBox::warning(this,"warning","暂无Ans");
        return;
    }
    if(cal_complete==true){
        ui->lineEdit_Input->clear();
        ui->lineEdit_Result->clear();
    }
    int temp_position=ui->lineEdit_Input->cursorPosition();
    QString temp=ui->lineEdit_Input->text().insert(temp_position,ui->pBtn_Ans->text());
    ui->lineEdit_Input->setText(temp);
    ui->lineEdit_Input->setCursorPosition(temp_position+ui->pBtn_Ans->text().size());
    ui->lineEdit_Input->setFocus();
    cal_complete=false;
}
//to be improved//del sin//键盘//方向
void CalculatorFX::DEL_clicked(){
    if(cal_complete==true){
        return;
    }
    ui->lineEdit_Input->backspace();
    ui->lineEdit_Input->setFocus();
}

void CalculatorFX::AC_clicked(){
    ui->lineEdit_Input->clear();
    ui->lineEdit_Result->clear();
    ui->lineEdit_Input->setFocus();
    cal_complete=false;
}
//to be improved//toggled
void CalculatorFX::optionCal_toggled(){
    QRadioButton* rBtn = qobject_cast<QRadioButton*>(sender());
    if(rBtn->isChecked()){
        if(cal_complete){
            ui->lineEdit_Result->setText(CalculatorFX::sysConversion(calPattern_current,rBtn->text(),ui->lineEdit_Result->text()));
        }
        else{
            ui->lineEdit_Input->setFocus();
        }
        calPattern_current=rBtn->text();
    }   
}

void CalculatorFX::optionTri_toggled(){
    QRadioButton* rBtn = qobject_cast<QRadioButton*>(sender());
    if(rBtn->isChecked()){
        if(cal_complete&&ui->lineEdit_Input->text().contains("arc")){
            ui->lineEdit_Result->setText(CalculatorFX::triConversion(rBtn->text(),ui->lineEdit_Result->text()));
        }
        else if(!cal_complete){
            ui->lineEdit_Input->setFocus();
        }
        tri_current=rBtn->text();
    }   
}

QString CalculatorFX::sysConversion(QString sys_current,QString sys_target,QString num){
    if(sys_current==sys_target)
        return num;
    int temp;
    bool ok;
    QString newNum;
    if(sys_current=="科学计算")
        temp=num.toInt(&ok,10);
    if(sys_current=="二进制")
        temp=num.toInt(&ok,2);
    if(sys_current=="八进制")
        temp=num.toInt(&ok,8);
    if(sys_current=="十六进制")
        temp=num.toInt(&ok,16);
    if(sys_current=="混合进制"){
        if(num.startsWith("0b"))
            temp=num.toInt(&ok,2);
        else
            temp=num.toInt(&ok,0);
    }
    if(ok==false)
        return "error";
    if(sys_target=="科学计算")
        newNum.setNum(temp,10);
    if(sys_target=="二进制")
        newNum.setNum(temp,2);
    if(sys_target=="八进制")
        newNum.setNum(temp,8);
    if(sys_target=="十六进制")
        newNum.setNum(temp,16);
    if(sys_target=="混合进制"){
        if(sys_current=="科学计算")
            return num;
        if(sys_current=="二进制")
            return "0b"+num;
        if(sys_current=="八进制")
            return "0"+num;
        if(sys_current=="十六进制")
            return "0x"+num;
    }
}

QString CalculatorFX::triConversion(QString tri_target,QString num){
    bool ok;
    double temp=num.toDouble(&ok);
    if(ok==false)
        return "error";
    double pi=qAcos(-1);
    if(tri_target=="角度制"){
        temp=temp*180/pi;
    }
    if(tri_target=="弧度制"){
        temp=temp*pi/180;
    }
    QString newNum;
    newNum.setNum(temp);
    return newNum;
}

QString CalculatorFX::sysPre(QString sys_current,QString expression){
    if(sys_current=="科学计算")
        return expression;
    for(int i=0;expression[i]!='\0';){
        int i1;
        if(expression[i].isDigit()||(expression[i]>='A'&&expression[i]<='F')||expression[i]=='b'||expression[i]=='x'){
            for(i1=i+1;expression[i1].isDigit()||(expression[i]>='A'&&expression[i]<='F')||expression[i]=='b'||expression[i]=='x';++i1);
            QString exp_temp=expression.mid(i,i1-i);
            exp_temp=CalculatorFX::sysConversion(sys_current, "科学计算", exp_temp);
            expression.replace(i,i1-i,exp_temp);
            i=i1;
        }
        else{
            ++i;
        }
    }
    return expression;
}

void CalculatorFX::preprocessing(QString expression, QVector<QString>& semi){
    expression.replace("Ans", Ans);
    expression.replace("π",pi);
    expression.replace("e",e);
    while(expression.contains("-+")&&expression.contains("+-")&&expression.contains("++")&&expression.contains("--")){
        expression.replace("+-","-");
        expression.replace("-+","-");
        expression.replace("++","+");
        expression.replace("--","+");
    }
    CalculatorFX::stack_pre(expression, semi);
    CalculatorFX::sign_pre(semi);
    CalculatorFX::omittedMul_pre(semi);
}

QString CalculatorFX::calculate(QVector<QString>& semi){
    CalculatorFX::toPost(semi);
    return CalculatorFX::calculatePost(semi);
}

void CalculatorFX::sign_pre(QVector<QString>& semi){
    ;

}

void CalculatorFX::omittedMul_pre(QVector<QString>& semi){
    ;
}

void CalculatorFX::stack_pre(QString expression, QVector<QString>& semi){
    QString singleOp="+-*/^%(),";
    for(int i=0;expression[i]!='\0';){
        if(expression[i].isDigit()){
            int i1;
            for(i1=i+1;expression[i1].isDigit()||expression[i1]=='.';++i1);
            semi.push_back(expression.mid(i,i1-i));
            i=i1;
        }
        if(singleOp.contains(expression[i])){
            semi.push_back(expression.mid(i,1));
            ++i;
        }

    }

}



void CalculatorFX::toPost(QVector<QString>& semi){
    ;
}

QString CalculatorFX::calculatePost(QVector<QString>& semi){
    ;
}





















