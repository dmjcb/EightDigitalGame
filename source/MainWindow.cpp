#include "include/MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 窗口透明度
    setWindowOpacity(0.97);
    //  禁止最大化按钮
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    //  设置拖动窗口大小,这样的操作会使页面无法最大化;x,y 宽,长
    setFixedSize(960, 825);

    // 将组件替换为数组以bian一会操作
    mStartLine[0] = ui->originLineEdit0;
    mStartLine[1] = ui->originLineEdit1;
    mStartLine[2] = ui->originLineEdit2;
    mStartLine[3] = ui->originLineEdit3;
    mStartLine[4] = ui->originLineEdit4;
    mStartLine[5] = ui->originLineEdit5;
    mStartLine[6] = ui->originLineEdit6;
    mStartLine[7] = ui->originLineEdit7;
    mStartLine[8] = ui->originLineEdit8;

    mEndLine[0] = ui->endLineEdit0;
    mEndLine[1] = ui->endLineEdit1;
    mEndLine[2] = ui->endLineEdit2;
    mEndLine[3] = ui->endLineEdit3;
    mEndLine[4] = ui->endLineEdit4;
    mEndLine[5] = ui->endLineEdit5;
    mEndLine[6] = ui->endLineEdit6;
    mEndLine[7] = ui->endLineEdit7;
    mEndLine[8] = ui->endLineEdit8;

    // 默认速度
    ui->label_7->setText(QString::number(ui->horizontalSlider->value()));

    // 禁用计算路径,自动执行,单步执行,清空按钮
    ui->calculatePathButton->setDisabled(true);
    ui->autoOutputPathButton->setDisabled(true);
    ui->horizontalSlider->setDisabled(true);

    // 禁用上一步下一步按钮
    ui->displayAfterPathButton->setDisabled(true);
    ui->displayNextPathButton->setDisabled(true);

    // 默认禁用所有LineEdit控件
    SetInputLineStatus(mStartLine, true);
    SetInputLineStatus(mEndLine, true);

    ui->pathLabel->setText("");

    connect(ui->autoOutputPathButton, SIGNAL(clicked()), this, SLOT(DoClickedAutoOutputButton()));
    connect(ui->calculatePathButton, SIGNAL(clicked()), this, SLOT(DoClickedCalculatePathButton()));
    connect(ui->displayAfterPathButton, SIGNAL(clicked()), this, SLOT(DoClickedDisplayAfterPathButton));
    connect(ui->displayNextPathButton, SIGNAL(clicked()), this, SLOT(DoClickedDisplayNextPathButton()));
    connect(ui->clearDisplayButton, SIGNAL(clicked()), this, SLOT(DoClickedAutoOutputButton()));
    connect(ui->createOriginStatusButton, SIGNAL(clicked()), this, SLOT(DoClickedCreateOriginStatusButton()));
    connect(ui->manualInputButton, SIGNAL(clicked()), this, SLOT(DoClickedManualInputButton()));
    connect(ui->closeAppButton, SIGNAL(clicked()), this, SLOT(DoClickedCloseAppButton()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


// 判断用户输入的合法性
bool MainWindow::JudgeInputValidity(QString s)
{
    if (s.size() < 9)
    {
        return false;
    }

    int a[10] = {-1};

    std::string v = s.toStdString();

    for (int i = 0; i < 9; i++)
    {
        // 若包含非数字
        if (v[i] < 48 || v[i] > 57)
        {
            return false;
        }
        a[v[i] - '0'] = 1;
    }
    // 判断0-9数字是否都出现了
    for (int i = 0; i < 9; i++)
    {
        if (a[i] == -1)
        {
            return false;
        }
    }
    return true;
}


//  设置LineEdit的状态
void MainWindow::SetInputLineStatus(QLineEdit *a[9], bool flag)
{
    for (int i = 0; i < 9; i++)
    {
        a[i]->setReadOnly(flag);
        a[i]->setMaxLength(1);
    }
}


void MainWindow::SetDefaultInput(QString s, QLineEdit *a[9])
{
    for (int i = 0; i < 9; i++)
    {
        if (s[i] != '0')
        {
            a[i]->setText(s.mid(i, 1));
        }
        else
        {
            a[i]->setText(" ");
        }
    }
}


std::string MainWindow::CreateRandomStr()
{
    std::string targetStr = "";
    bool isExist = false;

    while (targetStr.size() < 9)
    {
        char a = (rand() % 9) + '0';
        isExist = false;

        for (int i = 0; i < targetStr.size(); i++)
        {
            // 该字符已存在
            if (targetStr[i] == a)
            {
                isExist = true;
                break;
            }
        }

        if (isExist == false)
        {
            targetStr += a;
        }
    }
    return targetStr;
}

void MainWindow::WaitTime(int times)
{

    QElapsedTimer t;

    t.start();
    while (t.elapsed() < times)
    {
        QCoreApplication::processEvents();
    }
}


QString MainWindow::SpliceInputStr(QLineEdit *line[9])
{
    QString s = "";
    for (int i = 0; i < 9; i++)
    {
        if (line[i]->text() == " ")
        {
            s += '0';
        }
        else
        {
            s += line[i]->text();
        }
    }
    return s;
}


void MainWindow::OutputPath(int num)
{
    if (num < 0)
    {
        this->mRunningStep = this->mGame.mPath.size();
        // QMessageBox::warning(NULL, "警告", "已到达初始状态");
        return;
    }

    // 到达终点
    if (num >= this->mGame.mPath.size())
    {
        this->mRunningStep = 0;

        ui->autoOutputPathButton->setDisabled(false);
        return;
    }

    ClearLine(mStartLine);

    QString nowPath = QString::fromStdString(mGame.mPath[num]);

    SetDefaultInput(nowPath, mStartLine);

    std::stringstream stream;

    stream << "第" << num + 1 << "步, 共" << mGame.mPath.size() << "步";

    ui->pathLabel->setText(QString::fromStdString(stream.str()));


    stream << ": " << mGame.mPath[num] << std::endl;
    ui->pathTextBrowser->insertPlainText(QString::fromStdString(stream.str()));

    stream.str("");
    stream.clear();
}


// 清除输入的值
void MainWindow::ClearLine(QLineEdit *a[9])
{
    // 先清除输入
    for (int i = 0; i < 9; i++)
    {
        a[i]->clear();
    }
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 自动输出八数码求解路径
void MainWindow::DoClickedAutoOutputButton()
{
    if (mGame.mPath.size() == 0)
    {
        QMessageBox::warning(NULL, "警告", "请先生成路径");
        return;
    }

    ui->pathTextBrowser->clear();


    ui->clearDisplayButton->setDisabled(true);
    ui->calculatePathButton->setDisabled(true);
    ui->autoOutputPathButton->setDisabled(true);
    ui->displayAfterPathButton->setDisabled(true);
    ui->displayNextPathButton->setDisabled(true);

    for (uint32_t i = 0; i < mGame.mPath.size(); i++)
    {
        OutputPath(i);
        WaitTime(10 * (100 - ui->horizontalSlider->value()));
    }

    QMessageBox::warning(NULL, "警告", "已到达,共" + QString::number(mGame.mPath.size()) + "步");


    ui->clearDisplayButton->setDisabled(false);
    ui->displayAfterPathButton->setDisabled(false);
    ui->displayNextPathButton->setDisabled(false);
}

// 自动随机生成两个状态
void MainWindow::DoClickedCreateOriginStatusButton()
{
    SetInputLineStatus(mStartLine, true);
    SetInputLineStatus(mEndLine, true);

    ui->manualInputButton->setDisabled(true);
    ui->autoOutputPathButton->setDisabled(true);


    ui->calculatePathButton->setDisabled(false);

    srand((int)time(0));

    std::string startStr = CreateRandomStr();
    std::string endStr = CreateRandomStr();

    while (1)
    {
        if (mGame.IsOdevity(startStr, endStr))
        {
            break;
        }
        startStr = CreateRandomStr();
        endStr = CreateRandomStr();
    }

    ClearLine(mStartLine);
    ClearLine(mEndLine);


    SetDefaultInput(QString::fromStdString(startStr), mStartLine);
    SetDefaultInput(QString::fromStdString(endStr), mEndLine);
}

// 手动输入状态
void MainWindow::DoClickedManualInputButton()
{
    QMessageBox::warning(NULL, "警告", "请手动输入初始与结束状态");

    // 解禁LineEdit
    SetInputLineStatus(mStartLine, false);
    SetInputLineStatus(mEndLine, false);

    // 解禁计算路径
    ui->calculatePathButton->setDisabled(false);

    // 禁用自动生成
    ui->autoOutputPathButton->setDisabled(true);
}

// 计算路径
void MainWindow::DoClickedCalculatePathButton()
{
    this->mStartInputStr = SpliceInputStr(mStartLine);
    this->mEndInputStr = SpliceInputStr(mEndLine);

    // 判断合法性
    if (!JudgeInputValidity(this->mStartInputStr) || !JudgeInputValidity(this->mEndInputStr))
    {
        QMessageBox::warning(NULL, "警告", "请检查输入的合法性");
        return;
    }

    // 如果不可达
    if (!this->mGame.IsOdevity(this->mStartInputStr.toStdString(), this->mEndInputStr.toStdString()))
    {
        QMessageBox::warning(NULL, "警告", "不可达请重新输入");

        DoClickedClearDisplayButton();
        return;
    }
    // 禁用生成状态,计算路径按钮
    ui->calculatePathButton->setDisabled(true);
    ui->autoOutputPathButton->setDisabled(true);
    ui->manualInputButton->setDisabled(true);

    // 默认禁用所有LineEdit控件
    SetInputLineStatus(mStartLine, true);
    SetInputLineStatus(mEndLine, true);

    // 给状态f赋值
    this->mGame.mStartConfiguration = mStartInputStr.toStdString();
    this->mGame.mEndConfiguration = mEndInputStr.toStdString();

    // 开始计算八数码
    this->mGame.FindPath();

    QMessageBox::warning(NULL, "警告", "路径已经生成,共" + QString::number(this->mGame.mPath.size()) + "步");

    // 解禁自动执行,清空按钮,下一步按钮
    ui->autoOutputPathButton->setDisabled(false);
    ui->clearDisplayButton->setDisabled(false);
    ui->displayNextPathButton->setDisabled(false);

    // 解禁调速按钮
    ui->horizontalSlider->setDisabled(false);

    // 输出open与close表
    for (int i = 0, size = this->mGame.mOpenTable.size(); i < size; i++)
    {
        ui->open_textBrowser->insertPlainText(this->mGame.mOpenTable[i] + "\n\n");
    }

    for (int i = 0, size = this->mGame.mCloseTable.size(); i < size; i++)
    {
        ui->close_textBrowser->insertPlainText(this->mGame.mCloseTable[i] + "\n\n");
    }
}


// 清空输入
void MainWindow::DoClickedClearDisplayButton()
{
    ui->calculatePathButton->setDisabled(true);
    ui->autoOutputPathButton->setDisabled(true);
    ui->displayAfterPathButton->setDisabled(true);
    ui->displayNextPathButton->setDisabled(true);

    ui->horizontalSlider->setDisabled(true);


    ui->autoOutputPathButton->setDisabled(false);
    ui->manualInputButton->setDisabled(false);

    ui->horizontalSlider->setValue(50);

    ClearLine(mStartLine);
    ClearLine(mEndLine);

    SetInputLineStatus(mStartLine, true);
    SetInputLineStatus(mEndLine, true);

    ui->pathTextBrowser->clear();
    ui->open_textBrowser->clear();
    ui->close_textBrowser->clear();


    std::vector<State>().swap(this->mGame.mOpenState);
    std::vector<State>().swap(this->mGame.mCloseState);
    std::vector<std::string>().swap(this->mGame.mPath);


    ui->pathLabel->setText("");
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label_7->setText(QString::number(value));
}


void MainWindow::on_pathTextBrowser_sourceChanged(const QUrl &arg1)
{
    ui->pathTextBrowser->moveCursor(QTextCursor::End);
}


void MainWindow::DoClickedDisplayAfterPathButton()
{
    mRunningStep--;
    OutputPath(mRunningStep);
}


void MainWindow::DoClickedDisplayNextPathButton()
{
    // 解禁after按钮
    ui->displayAfterPathButton->setDisabled(false);

    if (this->mRunningStep == 0)
    {
        OutputPath(0);
    }

    this->mRunningStep++;

    OutputPath(mRunningStep);
}

// 关闭窗体
void MainWindow::DoClickedCloseAppButton()
{
    QMessageBox msgBox;

    msgBox.setText("提示");
    msgBox.setInformativeText("确实要退出吗?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if (msgBox.exec() == QMessageBox::Ok)
    {
        QApplication::exit();
    }
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
