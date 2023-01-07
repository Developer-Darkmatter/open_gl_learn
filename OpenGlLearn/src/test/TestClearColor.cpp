﻿#include "TestClearColor.h"

#include "../Renderer.h"

test::TestClearColor::TestClearColor() : m_ClearColor{ 0.2f, 0.4f, 0.8f, 1.0f}
{
}

test::TestClearColor::~TestClearColor()
{
}

void test::TestClearColor::OnUpdate(float deltaTime)
{
    Test::OnUpdate(deltaTime);
}

void test::TestClearColor::OnRender()
{
    Test::OnRender();
    GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void test::TestClearColor::OnImGui()
{
    Test::OnImGui();
}
