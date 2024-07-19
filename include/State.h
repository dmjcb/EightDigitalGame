/*
 * @Description:
 * @Version: 1.0
 * @Author: dmjcb
 * @Email: dmjcb@outlook.com
 * @Date: 2022-09-09 22:57:33
 * @LastEditors: dmjcb
 * @LastEditTime: 2024-07-19 18:09:13
 */

#ifndef STATE_H
#define STATE_H

#include <iostream>

// 状态类
typedef class State
{
public:
    State() = default;

    State(std::string nodeNode, std::string fatherNode, int mGValue, int mHValue);

    const std::string GetNowNode() const;

    const std::string GetFatherNode() const;

    const int GetFValue() const;

    const int GetGValue() const;

    const int GetHValue() const;

    void SetNowNode(std::string nodeNode);

    void SetFatherNode(std::string fatherNode);

    void SetFValue(int mFValue);

    void SetGValue(int mGValue);

    void SetHValue(int mHValue);

    // 更新父节点与g值
    void UpdateFatherAndGValue(const std::string &fatherNode, int mGValue);

    bool operator<(const State &s) const;

    bool operator==(const State &s) const;

protected:
    // 当前节点
    std::string mNodeNode;

    // 父节点
    std::string mFatherNode;

    int mFValue;

    int mGValue;

    int mHValue;

} State;

#endif
