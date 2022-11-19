/* 
 * File:   KeyboardSimulation.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * Licence : LGPL  2.1
 * Copyright Saleem Edah-Tally, M.D. - © 2022
 *
 * Created on October 8, 2022, 3:43 PM
 */

#ifndef KEYBOARDSIMULATION_H
#define KEYBOARDSIMULATION_H

#include "XT7Main.h"

class PedalEVH;

class KeyboardSimulation {
public:
    KeyboardSimulation(PedalEVH * evh);
    virtual ~KeyboardSimulation();
private:
    PedalEvent::PedalStatus m_currentLeftStatus = PedalEvent::RELEASED;
    PedalEvent::PedalStatus m_currentMiddleStatus = PedalEvent::RELEASED;
    PedalEvent::PedalStatus m_currentRightStatus = PedalEvent::RELEASED;
    
    PedalEVH * m_pedalEVH;
    void OnKeyDown(wxKeyEvent& evt);
};

#endif /* KEYBOARDSIMULATION_H */

