/*
 * @Description:
 * @Version: 1.0
 * @Author: dmjcb
 * @Email: dmjcb@outlook.com
 * @Date: 2022-09-09 22:58:24
 * @LastEditors: dmjcb
 * @LastEditTime: 2024-07-19 18:04:00
 */

#include "include/State.h"

State::State(std::string mNodeNode, std::string mFatherNode, int mGValue, int mHValue)
{
    this->mNodeNode = std::move(mNodeNode);
    this->mFatherNode = std::move(mFatherNode);
    this->mFValue = mGValue + mHValue;
    this->mGValue = mGValue;
    this->mHValue = mHValue;
}

const std::string State::GetNowNode() const
{
    return this->mNodeNode;
}

void State::SetNowNode(std::string mNodeNode)
{
    this->mNodeNode = mNodeNode;
}

const std::string State::GetFatherNode() const
{
    return this->mFatherNode;
}

void State::SetFatherNode(std::string mFatherNode)
{
    this->mFatherNode = mFatherNode;
}

const int State::GetFValue() const
{
    return this->mFValue;
}

void State::SetFValue(int mFValue)
{
    this->mFValue = mFValue;
}

const int State::GetGValue() const
{
    return this->mGValue;
}

void State::SetGValue(int mGValue)
{
    this->mGValue = mGValue;
}

const int State::GetHValue() const
{
    return this->mHValue;
}

void State::SetHValue(int mHValue)
{
    this->mHValue = mHValue;
}

bool State::operator<(const State &s) const
{
    return s.mFValue < mFValue;
}

bool State::operator==(const State &s) const
{
    return s.mNodeNode == mNodeNode;
}

// 更新父节点和g值
void State::UpdateFatherAndGValue(const std::string &mFatherNode, int mGValue)
{
    this->mFatherNode = mFatherNode;
    this->mFValue = mGValue + this->mHValue;
    this->mGValue = mGValue;
}
