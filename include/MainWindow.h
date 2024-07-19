/*
 * @Description:
 * @Version: 1.0
 * @Author: dmjcb
 * @Email: dmjcb@outlook.com
 * @Date: 2022-09-08 23:03:10
 * @LastEditors: dmjcb
 * @LastEditTime: 2024-07-19 19:54:01
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "include/Game.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QTextBrowser>
#include <QMessageBox>
#include <QDebug>
#include <QTime>

#include <vector>
#include <windows.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    // 清除输入
    void ClearLine(QLineEdit *a[9]);

    // 设置默认输入
    void SetDefaultInput(QString str, QLineEdit *a[9]);

    // 设置输入框状态
    void SetInputLineStatus(QLineEdit *a[9], bool flag);

    // 将九宫格输入转化为字符串
    QString SpliceInputStr(QLineEdit *a[9]);

    // 产生随机字符串
    std::string CreateRandomStr();

    // 延时函数
    void WaitTime(int times);

    // 输出路径
    void OutputPath(int num);

    // 输出open与close表
    void OuputTable(QTextBrowser text, std::vector<std::string> v);

    // 判断LineEdit的合法性
    bool JudgeInputValidity(QString s);

private slots:
    void DoClickedAutoOutputButton();

    void DoClickedCreateOriginStatusButton();

    void DoClickedClearDisplayButton();

    void DoClickedCalculatePathButton();

    void DoClickedManualInputButton();

    void DoClickedDisplayAfterPathButton();

    void DoClickedDisplayNextPathButton();

    void DoClickedCloseAppButton();

    void on_horizontalSlider_valueChanged(int value);

    void on_pathTextBrowser_sourceChanged(const QUrl &arg1);
protected:
    Ui::MainWindow *ui;

    QLineEdit *mStartLine[9], *mEndLine[9];

    Game mGame;

    QString mStartInputStr;

    QString mEndInputStr;

    // 记录当前时第几步
    int mRunningStep = 0;
};

#endif
