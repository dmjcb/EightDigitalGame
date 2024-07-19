#include "include/Game.h"

Game::Game(std::string startConfiguration, std::string endConfiguration)
{
    this->mStartConfiguration = std::move(startConfiguration);
    this->mEndConfiguration = std::move(endConfiguration);
}

// 判断两个字符的奇偶性
bool Game::IsOdevity(std::string startConfiguration, std::string endConfiguration)
{
    int oss = 0;
    int fss = 0;
    for (int i = 1; i < 9; ++i)
    {
        int a = 0, b = 0;
        for (int j = 0; j < i; j++)
        {
            if (startConfiguration[j] > startConfiguration[i] && startConfiguration[i] != '0')
            {
                a++;
            }
            if (endConfiguration[j] > endConfiguration[i] && endConfiguration[i] != '0')
            {
                b++;
            }
        }
        oss += a;
        fss += b;
    }
    return oss % 2 == fss % 2;
}

// 更新状态
void Game::UpdateState(State state)
{
    std::stringstream stream;

    // 空白位置下标
    int blackPos = state.GetNowNode().find('0');
    for (int i = 0; i < 4; i++)
    {
        // 该方向可达
        if (this->mRemoveablePos[blackPos][i] != -1)
        {
            std::string next_str = state.GetNowNode();
            // 交换空白位置与它可达位置的元素
            std::swap(next_str[mRemoveablePos[blackPos][i]], next_str[blackPos]);
            // 若交换后的字符串未在close表中
            if (GetNodePos(next_str, this->mCloseState) == -1)
            {
                int n = GetNodePos(next_str, this->mOpenState);
                // 若该字符串也未在open表中
                if (n == -1)
                {
                    // 写入QT控件
                    stream << (mOpenTable.size() + 1) << ":" << next_str << "不在open表中,加入";

                    this->mOpenTable.push_back(QString::fromStdString(stream.str()));

                    stream.str("");
                    stream.clear();

                    this->mOpenState.emplace_back(next_str, state.GetNowNode(), state.GetGValue() + 1, this->SetWValue(next_str));
                }

                // 否则,若经过当前状态可以使路径更优
                else if (state.GetGValue() + 1 < mOpenState[n].GetGValue())
                {
                    // 将当前状态的节点设为交换后状态的父节点,并更新g值
                    this->mOpenState[n].UpdateFatherAndGValue(state.GetNowNode(), state.GetGValue() + 1);

                    stream << (mOpenTable.size() + 1) << ":" << next_str << "在open表中,g值更新为" << (state.GetGValue() + 1);
                    this->mOpenTable.push_back(QString::fromStdString(stream.str()));
                    stream.str("");
                    stream.clear();
                }
            }

            if (next_str == this->mEndConfiguration)
            {
                this->mFlag = true;
                return;
            }
        }
    }

    // 将当前状态移出open表
    this->mOpenState.erase(find(mOpenState.begin(), mOpenState.end(), state));
    this->mCloseState.push_back(state);

    // 写入QT控件
    stream << (mOpenTable.size() + 1) << ":" << state.GetNowNode() << "被移除open表";
    this->mOpenTable.push_back(QString::fromStdString(stream.str()));

    stream.str("");
    stream.clear();

    stream << (mCloseTable.size() + 1) << ":" << state.GetNowNode() << "被加入close表";
    this->mCloseTable.push_back(QString::fromStdString(stream.str()));

    stream.str("");
    stream.clear();
}

// 给定一个字符串返回它在容器的下标
int Game::GetNodePos(const std::string &s, const std::vector<State> &v)
{
    for (int i = 0, size = v.size(); i < size; ++i)
    {
        if (v[i].GetNowNode() == s)
        {
            return i;
        }
    }
    return -1;
}

// 生成w值
int Game::SetWValue(std::string node)
{
    // 即求出当前字符串str中的每个元素str[i]在str中的下标i与str[i]在strend中的下标之差
    int sum = 0;
    for (int i = 0; i < 9; i++)
    {
        if (i != int(node.find('0')))
        {
            sum += abs(i - int(this->mEndConfiguration.find(node[i])));
        }
    }
    return sum;
}

// 生成整个转移过程的路径
void Game::FindPath()
{
    if (!IsOdevity(this->mStartConfiguration, this->mEndConfiguration))
    {
        return;
    }
    // 加入初始状态
    this->mOpenState.emplace_back(this->mStartConfiguration, " ", 0, 0);

    UpdateState(this->mOpenState[0]);

    this->mFlag = false;

    while (!this->mOpenState.empty())
    {
        if (this->mFlag)
        {
            break;
        }
        // 按f值排序
        std::sort(this->mOpenState.begin(), this->mOpenState.end());

        UpdateState(this->mOpenState[this->mOpenState.size() - 1]);
    }

    // 生成路径
    std::vector<State> path;

    path.insert(path.end(), this->mOpenState.begin(), this->mOpenState.end());
    path.insert(path.end(), this->mCloseState.begin(), this->mCloseState.end());

    // 结束节点状态的下标
    int endNodePos = GetNodePos(this->mEndConfiguration, path);

    while (path[endNodePos].GetFatherNode() != " ")
    {
        this->mPath.push_back(path[endNodePos].GetNowNode());
        // 找寻下一个节点
        endNodePos = GetNodePos(path[endNodePos].GetFatherNode(), path);
    }

    // 加入起始节点
    this->mPath.push_back(this->mStartConfiguration);
    reverse(this->mPath.begin(), this->mPath.end());
}
