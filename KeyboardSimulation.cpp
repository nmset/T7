/* 
 * File:   KeyboardSimulation.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * Licence : LGPL  2.1
 * Copyright Saleem Edah-Tally, M.D. - © 2022
 * 
 * Created on October 8, 2022, 3:43 PM
 */

#include "KeyboardSimulation.h"

KeyboardSimulation::KeyboardSimulation(PedalEVH * evh) {
    m_pedalEVH = evh;
    if (!m_pedalEVH)
        return;
    m_pedalEVH->m_owner->Bind(wxEVT_CHAR_HOOK, &KeyboardSimulation::OnKeyDown, this);
}

KeyboardSimulation::~KeyboardSimulation() {
}

void KeyboardSimulation::OnKeyDown(wxKeyEvent& evt) {
    const int keycode = evt.GetKeyCode();
    if (keycode != WXK_ESCAPE && keycode != WXK_F5
            && keycode != WXK_F6 && keycode != WXK_F7)
        return;
    // Stop current.
    if (m_currentLeftStatus == PedalEvent::PRESSED) {
        m_currentLeftStatus = PedalEvent::RELEASED;
        m_pedalEVH->Left(m_currentLeftStatus);
    }
    if (m_currentMiddleStatus == PedalEvent::PRESSED) {
        m_currentMiddleStatus = PedalEvent::RELEASED;
        m_pedalEVH->Middle(m_currentMiddleStatus);
    }
    if (m_currentRightStatus == PedalEvent::PRESSED) {
        m_currentRightStatus = PedalEvent::RELEASED;
        m_pedalEVH->Right(m_currentRightStatus);
    }
    // Start new or stop all..
    switch (keycode) {
        case WXK_F5:
            m_currentLeftStatus = PedalEvent::PRESSED;
            m_pedalEVH->Left(m_currentLeftStatus);
            break;
        case WXK_F6:
            m_currentMiddleStatus = PedalEvent::PRESSED;
            m_pedalEVH->Middle(m_currentMiddleStatus);
            break;
        case WXK_F7:
            m_currentRightStatus = PedalEvent::PRESSED;
            m_pedalEVH->Right(m_currentRightStatus);
            break;
        case WXK_ESCAPE:
            m_currentLeftStatus = PedalEvent::RELEASED;
            m_pedalEVH->Left(m_currentLeftStatus);
            m_currentMiddleStatus = PedalEvent::RELEASED;
            m_pedalEVH->Middle(m_currentMiddleStatus);
            m_currentRightStatus = PedalEvent::RELEASED;
            m_pedalEVH->Right(m_currentRightStatus);
            break;
        default:
            break;
    }
}
